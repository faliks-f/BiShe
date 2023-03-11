//
// Created by faliks on 3/10/23.
//

#include "Buffer.h"

using namespace std;

Buffer::Buffer() {
    buf = "";
}

Buffer::Buffer(const char *s) {
    buf = string(s);
}

const char *Buffer::c_str() {
    return buf.c_str();
}

size_t Buffer::length() {
    return buf.length();
}

void Buffer::setBuf(const char *s) {
    buf = string(s);
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::append(const char *s) {
    buf += string(s);
}
