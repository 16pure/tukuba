#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <sysexits.h>
#include<string.h>




#define SERIAL_PORT "/dev/ttyUSB0"

using namespace std;

void sirial(float send){
    int fd;
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, B115200);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    char str[9];
    sprintf(str, "%f", send);
    char buf[9];
    strcpy(buf,str);
    write(fd, buf , sizeof(buf));
}


int main(){
	float send=4.000;
	sirial(send);
}
