//
// Created by faliks on 3/10/23.
//

#include "Uart.h"
#include "unistd.h"
#include "fcntl.h"
#include "Utils.h"
#include "Buffer.h"
#include "cstring"
#include "iostream"

using namespace std;

void Uart::connect(const char *device) {
    fd = open(device, O_RDWR | O_NONBLOCK | O_NDELAY);
    ErrorUtils::errIf(fd < 0, "Uart open error!");
    struct termios newtio, oldtio;
    /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    ErrorUtils::errIf(tcgetattr(fd, &oldtio) != 0, "Get config error!");
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    /*设置停止位*/
    newtio.c_cflag |= CS8;
    /*设置奇偶校验位*/
    /*奇数
    newtio.c_cflag |= PARENB;
    newtio.c_cflag |= PARODD;
    newtio.c_iflag |= (INPCK | ISTRIP);
    /*
    //偶数
    newtio.c_iflag |= (INPCK | ISTRIP);
    newtio.c_cflag |= PARENB;
    newtio.c_cflag &= ~PARODD;
    //无奇偶校验位
     */
    newtio.c_cflag &= ~PARENB;

    /*设置波特率*/
    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);
    /*设置停止位*/
    //newtio.c_cflag &= ~CSTOPB;//1
    newtio.c_cflag |= CSTOPB;//2
    /*设置等待时间和最小接收字符*/
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    /*处理未接收字符*/
    tcflush(fd, TCIFLUSH);
    ErrorUtils::errIf(-1 == tcsetattr(fd, TCSANOW, &newtio), "Uart set config error!");
    tcflush(fd, TCIOFLUSH);
}


Uart::~Uart() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
    if (buffer) {
        delete buffer;
        buffer = nullptr;
    }
}

Uart::Uart() {
    fd = -1;
    buffer = new Buffer();
}

void Uart::write() {
    const char *s = buffer->c_str();
    int len = (int) buffer->length();
    cout << "want to send:" << s << endl;
    int writeLen = ::write(fd, s, len);
    ErrorUtils::errIf(writeLen < 0, "Uart write error!");
}

void Uart::append(const char *s) {
    buffer->append(s);
}

void Uart::send() {
    write();
    buffer->clear();
}

// Just for test! Never use it!
void Uart::read() {
    char s[1024] = {0};
    int len = ::read(fd, s, 1024);
    ErrorUtils::errIf(len < 0, "Uart read error!");
    cout << s << endl;
}

