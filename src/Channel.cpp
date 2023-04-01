//
// Created by faliks on 3/30/23.
//

#include "Channel.h"

using namespace std;
using namespace cv;


static bool compareArea(vector<Point> &a, vector<Point> &b) {
    return contourArea(a) > contourArea(b);
}

vector<vector<Point>> Channel::getContour(Mat &mat, ColorIndex i) {
    Mat hsv, mask;
    cvtColor(mat, hsv, COLOR_BGR2HSV);
    inRange(hsv, getColorLow(i), getColorHigh(i), mask);
    if (i == ColorIndex::RED) {
        Mat mask2;
        inRange(hsv, HSVColor::RED_LOW2, HSVColor::RED_HIGH2, mask2);
        bitwise_or(mask, mask2, mask);
    }
    Mat element = getStructuringElement(MORPH_RECT, {7, 7});
    erode(mask, mask, element);
    dilate(mask, mask, element);
    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    sort(contours.begin(), contours.end(), compareArea);
    for (auto &c: contours) {
        double area = contourArea(c);
        if (area < 500 && area > 1000) {
            break;
        }
        return {c};
    }
    return {};
}

void Channel::setGetColorLow(std::function<std::array<int, 3>(ColorIndex)> cb) {
    getColorLow = std::move(cb);
}

void Channel::setGetColorHigh(function<std::array<int, 3>(ColorIndex)> cb) {
    getColorHigh = std::move(cb);
}
