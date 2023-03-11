//
// Created by faliks on 3/10/23.
//

#ifndef BISHE_BUFFER_H
#define BISHE_BUFFER_H

#include "unistd.h"
#include "string"

class Buffer {
private:
    std::string buf;
public:
    Buffer();

    Buffer(const char *s);

    const char *c_str();

    size_t length();

    void setBuf(const char *s);

    void append(const char *s);

    void clear();
};


#endif //BISHE_BUFFER_H
