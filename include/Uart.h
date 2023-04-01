//
// Created by faliks on 3/10/23.
//

#ifndef BISHE_UART_H
#define BISHE_UART_H

#include "termios.h"
#include "unistd.h"
#include "string"

class Buffer;

class Uart {
private:
    int fd;
    Buffer *buffer;
    void write();
public:
    Uart();

    ~Uart();

    void connect(const char * device);

    void append(const char *s);

    void send();

    void read();
};


#endif //BISHE_UART_H
