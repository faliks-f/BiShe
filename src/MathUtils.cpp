//
// Created by faliks on 3/31/23.
//

#include "MathUtils.h"
#include "cmath"

using namespace std;


vector<int> MathUtils::rotate(int x, int y, int angle) {
    double newAngle = angle / 180.0 * M_PI;
    int newX = static_cast<int >(x * cos(newAngle) - y * sin(newAngle));
    int newY = static_cast<int >(x * sin(newAngle) + y * cos(newAngle));
    return {newX, newY};
}
