//
// Created by faliks on 3/30/23.
//

#ifndef BISHE_CHANNEL_H
#define BISHE_CHANNEL_H

#include "functional"
#include "HSVColor.h"
#include "opencv2/opencv.hpp"

class Channel {
private:
    std::function<std::array<int, 3>(ColorIndex)> getColorLow;
    std::function<std::array<int, 3>(ColorIndex)> getColorHigh;
public:
    std::vector<std::vector<cv::Point>> getContour(cv::Mat &mat, ColorIndex i);

    void setGetColorLow(std::function<std::array<int, 3>(ColorIndex)> cb);

    void setGetColorHigh(std::function<std::array<int, 3>(ColorIndex)> cb);
};


#endif //BISHE_CHANNEL_H
