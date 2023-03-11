//
// Created by faliks on 3/10/23.
//
#include "Uart.h"
#include "Control.h"

using namespace std;

int main() {
    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    Control *control = new Control(uart);
    control->init();
    while (1) {
        int i, j;
        scanf("%d %d", &i, &j);
        control->setPwm(i, j);
        control->go();
    }
    return 0;
}