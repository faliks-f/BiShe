//
// Created by faliks on 3/10/23.
//

#include "Control.h"
#include "Uart.h"
#include "Utils.h"
#include "cmath"
#include "Camera.h"
#include "MathUtils.h"

using namespace std;

void Control::setPwm(int index, int pwm) {
    string indexStr = convert(index, 3);
    string pwmStr = convert(pwm, 4);
    for (int i = 1; i < 4; ++i) {
        command[i] = indexStr[i - 1];
    }
    for (int i = 5; i < 9; ++i) {
        command[i] = pwmStr[i - 5];
    }
    if (index == 1 || index == 2 || index == 3) {
        command[10] = '1';
        command[11] = '0';
    } else {
        command[10] = '0';
        command[11] = '5';
    }
}


void Control::go() {
    ErrorUtils::errIf(uart == nullptr, "Uart didn't init");
    uart->append(command.c_str());
    uart->send();
    sleep(1);
}

std::string Control::convert(int i, int n) {
    string res(n, '0');
    int index = n - 1;
    while (i) {
        res[index] = i % 10 + '0';
        i /= 10;
        index--;
    }
    return res;
}

void Control::init() {
    ErrorUtils::errIf(uart == nullptr, "Uart didn't init");
    for (int i = 0; i <= 5; ++i) {
        uart->append(commands[i].c_str());
        uart->send();
        sleep(1);
    }

}

bool Control::kinematicsMove(double x, double y, double z) {
    int min = 0;
    bool flag = false;
    if (y < 0) {
        return false;
    }
    //寻找最佳角度
    for (int i = 0; i >= -135; i--) {
        if (0 == kinematicsAnalysis(x, y, z, i)) {
            if (i < min) {
                min = i;
            }
            flag = true;
        }
    }
    //用3号舵机与水平最大的夹角作为最佳值
    if (flag) {
        kinematicsAnalysis(x, y, z, min);
        for (int j = 0; j <= 3; ++j) {
            setPwm(j, kinematics.servoPWM[j]);
            go();
        }
        setPwm(1, kinematics.servoPWM[1]);
        go();
    }
    return flag;
}

int Control::kinematicsAnalysis(double x, double y, double z, double alpha) {
    double theta3, theta4, theta5, theta6;
    double l0, l1, l2, l3;
    double aaa, bbb, ccc, zfFlag;
    l0 = kinematics.L0;
    l1 = kinematics.L1;
    l2 = kinematics.L2;
    l3 = kinematics.L3;
    if (x == 0) {
        theta6 = 0.0;
    } else {
        theta6 = atan(x / y) * 180.0 / PI;
    }

    y = sqrt(x * x + y * y);
    y = y - l3 * cos(alpha * PI / 180.0);
    z = z - l0 - l3 * sin(alpha * PI / 180.0);
    if (z < -l0) {
        return 1;
    }
    if (sqrt(y * y + z * z) > (l1 + l2)) {
        return 2;
    }
    ccc = acos(y / sqrt(y * y + z * z));
    bbb = (y * y + z * z + l1 * l1 - l2 * l2) / (2 * l1 * sqrt(y * y + z * z));
    if (bbb > 1 || bbb < -1) {
        return 5;
    }
    if (z < 0) {
        zfFlag = -1;
    } else {
        zfFlag = 1;
    }
    theta5 = ccc * zfFlag + acos(bbb);
    theta5 = theta5 * 180.0 / PI;
    if (theta5 > 180.0 || theta5 < 0.0) {
        return 6;
    }
    aaa = -(y * y + z * z - l1 * l1 - l2 * l2) / (2 * l1 * l2);
    if (aaa > 1 || aaa < -1) {
        return 3;
    }
    theta4 = acos(aaa);
    theta4 = 180.0 - theta4 * 180.0 / PI;
    if (theta4 > 135.0 || theta4 < -135.0) {
        return 4;
    }
    theta3 = alpha - theta5 + theta4;
    if (theta3 > 90.0 || theta3 < -90.0) {
        return 7;
    }
    kinematics.servoAngle[0] = theta6;
    kinematics.servoAngle[1] = theta5 - 90;
    kinematics.servoAngle[2] = theta4;
    kinematics.servoAngle[3] = theta3;

    kinematics.servoPWM[0] = (int) (1500 - 2000.0 * kinematics.servoAngle[0] / 270.0);
    kinematics.servoPWM[1] = (int) (1500 - 2000.0 * kinematics.servoAngle[1] / 270.0);
    kinematics.servoPWM[2] = (int) (1500 - 2000.0 * kinematics.servoAngle[2] / 270.0);
    kinematics.servoPWM[3] = (int) (1500 - 2000.0 * kinematics.servoAngle[3] / 270.0);
    return 0;
}

Control::Control(Uart *uart1, Camera *camera1) {
    this->uart = uart1;
    this->camera = camera1;
    commands = vector<string>{"#000P1500T0500!", "#001P1500T1000!", "#002P1500T1000!",
                              "#003P1500T1000!", "#004P1500T0500!", "#005P1300T0500!"};
    command = "#000P1500T0100!";
    kinematics.L0 = 1100;
    kinematics.L1 = 1050;
    kinematics.L2 = 750;
    kinematics.L3 = 1900;
}

Control::~Control() {
    if (!uart) {
        delete uart;
        uart = nullptr;
    }
    if (!camera) {
        delete camera;
        camera = nullptr;
    }
}

void Control::fromImgCor2WorldCor(const vector<int> &point) {
    if (point[0] < 0 || point[1] < 0) {
        return;
    }
    const vector<int> &matCenter = camera->getMatCenter();
    int x = point[0] - matCenter[0];
    int y = point[1] - matCenter[1];
    vector<int> newPoint = MathUtils::rotate(x, y, -50);
    bool isMove = kinematicsMove(-(newPoint[1] * 2000 / 160), abs(newPoint[0] * 2000 / 160), 800);
    if (isMove) {
        getchar();
    }
}


