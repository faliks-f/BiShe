//
// Created by faliks on 3/10/23.
//
#include "Uart.h"
#include "Control.h"

using namespace std;


void test1() {
    Control *control = new Control(nullptr);
    control->kinematicsMove(100, 100, 100);
}

int main() {
    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    Control *control = new Control(uart);
    control->init();
    while (1) {
        double x, y, z;
        scanf("%lf %lf %lf", &x, &y, &z);
        control->kinematicsMove(x, y, z);
    }
    return 0;
}

