//
// Created by faliks on 3/10/23.
//

#include "Control.h"
#include "Uart.h"
#include "Utils.h"

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
}

Control::Control(Uart *uart1) {
    commands = vector<string>{"#000P1500T0100!", "#001P1500T0100!", "#002P1500T0100!",
                              "#003P1500T0100!", "#004P1500T0100!", "#005P1500T0100!"};
    command = "#000P1500T0100!";
    uart = uart1;
}

void Control::go() {
    Utils::errIf(uart == nullptr, "Uart didn't init");
    uart->append(command.c_str());
    uart->send();
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
    for (auto &c: commands) {
        uart->append(c.c_str());
    }
    uart->send();
}
