//
// Created by faliks on 4/2/23.
//

#include "MainWindow.h"
#include "QLabel"
#include "Camera.h"
#include "QImage"
#include "QLayout"
#include "QPixmap"
#include "QTimer"
#include "QPushButton"
#include "Control.h"
#include "ThreadPool.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent, Camera *camera1, Control *control1, ThreadPool *threadPool1)
        : QWidget(parent), camera(camera1), control(control1), threadPool(threadPool1) {
    initLabel();

    initButton();

    initLayout();

    flushImgTimer = new QTimer(this);
    connect(flushImgTimer, &QTimer::timeout, this, &MainWindow::getImage);
    flushImgTimer->start(100);
    this->setMinimumWidth(640);
    this->setMinimumHeight(480);
}

MainWindow::~MainWindow() {
    delete blackImage;
}

void MainWindow::getImage() {
    const uchar *data = camera->getDrawImg();
    if (data == nullptr) {
        data = blackImage;
    }
    QImage image(data, 640, 480, 1920, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::initButton() {
    clampButton = new QPushButton("抓取");
    connect(clampButton, &QPushButton::clicked, this, [this]() {
        threadPool->add([this] {
            control->fromImgCor2WorldCor(camera->getCenter());
        });
    });

    redButton = new QPushButton("识别红色物体");
    greenButton = new QPushButton("识别绿色物体");
    yellowButton = new QPushButton("识别黄色物体");
    connect(redButton, &QPushButton::clicked, this, [this]() {
        camera->setColorIndex(ColorIndex::RED);
    });
    connect(greenButton, &QPushButton::clicked, this, [this]() {
        camera->setColorIndex(ColorIndex::GREEN);
    });
    connect(yellowButton, &QPushButton::clicked, this, [this]() {
        camera->setColorIndex(ColorIndex::YELLOW);
    });
}

void MainWindow::initLabel() {
    blackImage = new uchar[640 * 480 * 3];
    memset(blackImage, 0, 640 * 480 * 3);
    label = new QLabel(this);
    label->setBaseSize(QSize(640, 480));
    getImage();
}

void MainWindow::initLayout() {
    auto layout1 = new QVBoxLayout();
    layout1->addWidget(redButton);
    layout1->addWidget(greenButton);
    layout1->addWidget(yellowButton);
    layout1->addWidget(clampButton);
    auto layout2 = new QHBoxLayout();
    layout2->addWidget(label);
    layout2->addLayout(layout1);
    this->setLayout(layout2);
}
