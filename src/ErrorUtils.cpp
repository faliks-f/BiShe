//
// Created by faliks on 3/10/23.
//

#include "Utils.h"
#include "cstdio"
#include "cstdlib"

void ErrorUtils::errIf(bool condition, const char *s) {
    if (condition) {
        perror(s);
//        abort();
    }
}
