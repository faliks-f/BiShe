//
// Created by faliks on 3/10/23.
//

#ifndef BISHE_CONTROL_H
#define BISHE_CONTROL_H

#include "vector"
#include "string"

class Uart;
class Camera;

class Control {
private:
    class Kinematics {
    public:
        double L0;
        double L1;
        double L2;
        double L3;

        double servoAngle[6];
        double servoRange[6];
        int servoPWM[6];
    };

    constexpr const static double PI = 3.1415926535897;

    Kinematics kinematics;

    std::vector<std::string> commands;

    std::string command;

    Uart *uart = nullptr;

    Camera *camera = nullptr;

    static std::string convert(int i, int n);

    int kinematicsAnalysis(double x, double y, double z, double alpha);

    void setPwm(int index, int pwm);

    void go();



public:
    Control(Uart *, Camera *);

    ~Control();

    void init();

    void fromImgCor2WorldCor(const std::vector<int>&point);

    [[nodiscard()]] bool kinematicsMove(double x, double y, double z);
};


#endif //BISHE_CONTROL_H
