//
// Created by faliks on 3/10/23.
//

#include "Utils.h"
#include "cstdio"


void Utils::errIf(bool condition, const char *s) {
    if (condition) {
        perror(s);
    }
}
