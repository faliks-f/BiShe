//
// Created by faliks on 3/29/23.
//

#ifndef BISHE_CAMERA_H
#define BISHE_CAMERA_H

#include "opencv2/opencv.hpp"
#include "string"
#include "mutex"
#include "HSVColor.h"

class Channel;

class Camera {
private:
    cv::VideoCapture *cap = nullptr;
    Channel *channel = nullptr;
    std::vector<int> center;
    cv::Mat drawImg;
    std::mutex centerMutex;
    std::mutex imgMutex;
    std::mutex indexMutex;
    const std::vector<int> matCenter{274, 234};
    ColorIndex colorIndex = ColorIndex::NO_COLOR;
public:

    explicit Camera(const std::string &device = "/dev/video0");

    Camera(Camera &&) = delete;

    Camera(Camera &) = delete;

    ~Camera();

    void calCenter(const std::vector<std::vector<cv::Point>> &c);

    void doJob();

    void setColorIndex(ColorIndex i);

    std::vector<int> getCenter();

    const uchar *getDrawImg();

    [[nodiscard]] const std::vector<int> &getMatCenter() const;
};

#endif //BISHE_CAMERA_H
