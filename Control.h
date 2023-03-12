//
// Created by faliks on 3/10/23.
//

#ifndef BISHE_CONTROL_H
#define BISHE_CONTROL_H

#include "vector"
#include "string"

class Uart;

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

    static std::string convert(int i, int n);

    int kinematicsAnalysis(double x, double y, double z, double alpha);

public:
    Control(Uart *);

    void setPwm(int index, int pwm);

    void init();

    void go();

    void kinematicsMove(double x, double y, double z);

    void test(int index, std::string pwm);
};


#endif //BISHE_CONTROL_H
