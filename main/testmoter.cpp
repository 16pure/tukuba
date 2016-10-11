#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
#include <iostream>



#include "controll_stick.h"
//#include "set_controller.h"


using namespace std;
#define SERIAL_PORT1 "/dev/ttyUSB0"
#define SERIAL_PORT2 "/dev/ttyUSB1"

void sirial1(float send){
    int fd;
    fd = open(SERIAL_PORT1, O_RDWR | O_NOCTTY);
    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, B115200);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    char str[10];
    sprintf(str, "%f", send);
    char buf[10];
    strcpy(buf,str);
    write(fd, buf , sizeof(buf));
}

void sirial2(float send){
    int fd;
    fd = open(SERIAL_PORT2, O_RDWR | O_NOCTTY);
    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, B115200);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    char str[10];
    sprintf(str, "%f", send);
    char buf[10];
    strcpy(buf,str);
    write(fd, buf , sizeof(buf));
}


void sirial3(void){
    int fd;
    fd = open(SERIAL_PORT1, O_RDWR | O_NOCTTY);
    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, B57600);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    char buf[10];
    strcpy(buf,"0xd");
    write(fd, buf , sizeof(buf));
}

void sirial4(void){
    int fd;
    fd = open(SERIAL_PORT2, O_RDWR | O_NOCTTY);
    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, B57600);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    char buf[10];
    strcpy(buf,"0xd");
    write(fd, buf , sizeof(buf));
}
main()
{	
	float L,R;
	int joy_fd( -1 ) , num_of_axis( 0 ) , num_of_buttons( 0 );
	char name_of_joystick[80];
	vector<char> joy_button;
	vector<int> joy_axis;
	set_controller();
	while(1){
		

		controll ( L , R ,joy_fd(-1) ,joy_axis,js));
		cout<<"L="<<L<<"R="<<R<<endl;
/*
		sirial1(L);
		sirial3();
		sirial2(R);
		sirial4();
*/
	}
}

