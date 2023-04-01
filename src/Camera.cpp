//
// Created by faliks on 3/29/23.
//

#include "Camera.h"
#include "opencv2/opencv.hpp"
#include "Utils.h"
#include "HSVColor.h"
#include "Channel.h"

using namespace cv;
using namespace std;


Camera::Camera(const string &device) {
    cap = new VideoCapture();
    cap->open("/dev/video0", CAP_ANY);
    ErrorUtils::errIf(!cap->isOpened(), "Camera open failed!");
    channel = new Channel();
    channel->setGetColorLow([](ColorIndex i) {
        if (i == ColorIndex::RED) {
            return HSVColor::RED_LOW1;
        } else if (i == ColorIndex::GREEN) {
            return HSVColor::GREEN_LOW;
        } else {
            return HSVColor::YELLOW_LOW;
        }
    });
    channel->setGetColorHigh([](ColorIndex i) {
        if (i == ColorIndex::RED) {
            return HSVColor::RED_HIGH1;
        } else if (i == ColorIndex::GREEN) {
            return HSVColor::GREEN_HIGH;
        } else {
            return HSVColor::YELLOW_HIGH;
        }
    });
    namedWindow("images");
    setMouseCallback("images", [](int event, int x, int y, int flags, void *parm) {
        if (event & EVENT_LBUTTONDBLCLK) {
            cout << x << " " << y << endl;
        }
    }, nullptr);
}

Camera::~Camera() {
    if (cap) {
        delete cap;
        cap = nullptr;
    }
    if (channel) {
        delete channel;
        channel = nullptr;
    }
}

void Camera::doJob() {
    Mat mat;
    namedWindow("images", 1);
    while (cap->read(mat)) {
        auto c = channel->getContour(mat, ColorIndex::GREEN);
        if (!c.empty()) {
            drawContours(mat, c, -1, Scalar(0, 0, 0), 2);
            calCenter(c);
        } else {
            unique_lock<mutex> uniqueLock(centerMutex);
            center = {-1, -1};
        }
        {
            unique_lock<mutex> uniqueLock(imgMutex);
            drawImg = mat;
            imshow("images", mat);
        }
        waitKey(1);
    }
}

void Camera::calCenter(const vector<std::vector<cv::Point>> &c) {
    Rect rect = boundingRect(c[0]);
    Point tl = rect.tl();
    Point br = rect.br();
    {
        unique_lock<mutex> uniqueLock(centerMutex);
        center = vector<int>{(tl.x + br.x) / 2, (tl.y + br.y) / 2};
    }
}

std::vector<int> Camera::getCenter() {
    unique_lock<mutex> uniqueLock(centerMutex);
    return center;
}

cv::Mat Camera::getDrawImg() {
    unique_lock<mutex> uniqueLock(imgMutex);
    return drawImg.clone();
}

const vector<int> &Camera::getMatCenter() const {
    return matCenter;
}


