//
// Created by faliks on 3/29/23.
//

#ifndef BISHE_CAMERA_H
#define BISHE_CAMERA_H

#include "opencv2/opencv.hpp"
#include "string"
#include "mutex"

class Channel;

class Camera {
private:
    cv::VideoCapture *cap = nullptr;
    Channel *channel = nullptr;
    std::vector<int> center;
    cv::Mat drawImg;
    std::mutex centerMutex;
    std::mutex imgMutex;
    const std::vector<int> matCenter{274, 234};
public:

    explicit Camera(const std::string &device = "/dev/video0");

    Camera(Camera &&) = delete;

    Camera(Camera &) = delete;

    ~Camera();

    void calCenter(const std::vector<std::vector<cv::Point>> &c);

    void doJob();

    std::vector<int> getCenter();

    cv::Mat getDrawImg();

    [[nodiscard]] const std::vector<int> &getMatCenter() const;
};

#endif //BISHE_CAMERA_H
