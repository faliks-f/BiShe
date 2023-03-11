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
    std::vector<std::string> commands;

    std::string command;

    Uart *uart = nullptr;

    static std::string convert(int i, int n);

public:
    Control(Uart *);

    void setPwm(int index, int pwm);

    void init();

    void go();

    void test(int index, std::string pwm);
};


#endif //BISHE_CONTROL_H
