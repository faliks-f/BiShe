//
// Created by faliks on 3/29/23.
//

#ifndef BISHE_HSVCOLOR_H
#define BISHE_HSVCOLOR_H

#include "array"

class HSVColor {
public:
    constexpr static std::array<int, 3> GREEN_LOW = {35, 43, 46};
    constexpr static std::array<int, 3> GREEN_HIGH = {99, 255, 255};
    constexpr static std::array<int, 3> YELLOW_LOW = {26, 43, 46};
    constexpr static std::array<int, 3> YELLOW_HIGH = {34, 255, 255};
    constexpr static std::array<int, 3> RED_LOW1 = {0, 43, 46};
    constexpr static std::array<int, 3> RED_HIGH1 = {10, 255, 255};
    constexpr static std::array<int, 3> RED_LOW2 = {156, 43, 46};
    constexpr static std::array<int, 3> RED_HIGH2 = {180, 255, 255};
};

enum class ColorIndex {
    RED = 0,
    GREEN = 1,
    YELLOW = 2,
    NO_COLOR = 3
};


#endif //BISHE_HSVCOLOR_H
