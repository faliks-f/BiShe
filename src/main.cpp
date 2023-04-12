//
// Created by faliks on 3/10/23.
//
#include "Uart.h"
#include "Control.h"
#include "Camera.h"
#include "ThreadPool.h"
#include "QApplication"
#include "MainWindow.h"

using namespace std;


void test1() {
    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    Control *control = new Control(uart, nullptr);
    control->init();
    while (1) {
        double x, y, z;
        cin >> x >> y >> z;
        bool _ = control->kinematicsMove(x, y, z);
        if (_) { break; };
    }
}

void test2() {
    auto *threadPool = new ThreadPool(10);
    auto *camera = new Camera("/dev/video0");
    camera->setColorIndex(ColorIndex::RED);
    threadPool->add([camera] { camera->doJob(); });

    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    auto *control = new Control(uart, camera);
    control->init();
    control->loosen();
//    threadPool->add([control, camera] { control->fromImgCor2WorldCor(camera->getCenter()); });
    while (true) {
        control->fromImgCor2WorldCor(camera->getCenter());
//        string s;
//        cin >> s;
//        if (s == "red") {
//            camera->setColorIndex(ColorIndex::RED);
//        } else if (s == "green") {
//            camera->setColorIndex(ColorIndex::GREEN);
//        } else if (s == "yellow") {
//            camera->setColorIndex(ColorIndex::RED);
//        } else {
//            camera->setColorIndex(ColorIndex::NO_COLOR);
//        }
    }
}

void test3() {

}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    auto threadPool = new ThreadPool(10);
    auto camera = new Camera("/dev/video0");
    threadPool->add([camera] { camera->doJob(); });
    Uart *uart = new Uart();
    uart->connect("/dev/ttyUSB0");
    auto *control = new Control(uart, camera);
    control->init();
    control->loosen();
    auto mainWindow = new MainWindow(nullptr, camera, control, threadPool);
    mainWindow->show();
    return QApplication::exec();
}

