//
// Created by faliks on 4/2/23.
//

#ifndef BISHE_MAINWINDOW_H
#define BISHE_MAINWINDOW_H

#include "QWidget"

class Camera;

class QPushButton;

class QLabel;

class Control;

class ThreadPool;

class MainWindow : public QWidget {

Q_OBJECT

private:
    QLabel *label;
    Camera *camera;
    Control *control;
    uchar *blackImage;
    QTimer *flushImgTimer;
    QPushButton *clampButton;
    QPushButton *redButton;
    QPushButton *greenButton;
    QPushButton *yellowButton;
    ThreadPool *threadPool;

    void initButton();

    void initLabel();

    void initLayout();

public:
    MainWindow(QWidget *parent, Camera *camera1, Control *control1, ThreadPool *threadPool1);

    ~MainWindow() override;

    void getImage();

};


#endif //BISHE_MAINWINDOW_H
