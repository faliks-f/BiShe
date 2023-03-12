//
// Created by faliks on 3/10/23.
//

#include "Control.h"
#include "Uart.h"
#include "Utils.h"
#include "cmath"

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

Control::Control(Uart *uart1) {
    commands = vector<string>{"#000P1500T0500!", "#001P1500T1000!", "#002P1500T1000!",
                              "#003P1500T1000!", "#004P1500T0500!", "#005P1500T0500!"};
    command = "#000P1500T0100!";
    uart = uart1;
    kinematics.L0 = 1100;
    kinematics.L1 = 1050;
    kinematics.L2 = 750;
    kinematics.L3 = 1900;
}

void Control::go() {
    Utils::errIf(uart == nullptr, "Uart didn't init");
    uart->append(command.c_str());
    uart->send();
    sleep(1);
}

void Control::test(int index, std::string pwm) {
    string i = to_string(index);
    for (int j = 0; j < i.length(); ++j) {
        commands[0][3 - j] = i[i.length() - 1 - j];
    }
    for (int j = 5; j < 9; ++j) {
        commands[0][j] = pwm[j - 5];
    }
    uart->append(commands[0].c_str());
    uart->send();
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
    Utils::errIf(uart == nullptr, "Uart didn't init");
    for (int i = 5; i >= 0; --i) {
        uart->append(commands[i].c_str());
        uart->send();
        sleep(1);
    }

}

void Control::kinematicsMove(double x, double y, double z) {
    int min = 0;
    bool flag = false;
    if (y < 0) {
        return;
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
        for (int j = 3; j >= 0; --j) {
            setPwm(j, kinematics.servoPWM[j]);
            go();
            if (j == 1) {
                sleep(2);
            }
        }
        setPwm(1, kinematics.servoPWM[1]);
        go();
    }
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


