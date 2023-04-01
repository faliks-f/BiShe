//
// Created by faliks on 3/10/23.
//
#include "Uart.h"
#include "Control.h"
#include "Camera.h"
#include "ThreadPool.h"

using namespace std;


void test1() {
    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    Control *control = new Control(uart, nullptr);
    control->init();
    while (1) {
        double x, y, z;
        scanf("%lf %lf %lf", &x, &y, &z);
        control->kinematicsMove(x, y, z);
    }
}

void test2() {
    auto *threadPool = new ThreadPool(10);
    auto *camera = new Camera("/dev/video0");
    threadPool->add([camera] { camera->doJob(); });
    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    auto *control = new Control(uart, camera);
    control->init();
    while (true) {
        control->fromImgCor2WorldCor(camera->getCenter());
    }
}

int main() {

    test2();
    return 0;
}

