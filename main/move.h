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

#define SERIAL_PORT "/dev/ttyUSB0"

//1=missile

void sirial1(float send){
    int fd;
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    newtio = oldtio;
    cfsetspeed(&newtio, B115200);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    char str[20];
    sprintf(str, "%.9g", send);
    char buf[20];
    strcpy(buf,str);
    write(fd, buf , sizeof(buf));
}

double integ(double v,double dv,double h,double sum){
        return sum+(v+dv)*h*0.5;
}

struct robo {
	float x;
	float y;
	float dvx;
	float dv;
	float dvy;
	float vx;
	float vy;
	float v;
	float th;
	float omg;
	float domg;
	float ramuda;
	float ramuda0;
	float dramuda;
	float zx;
	float zy;
	float z;
};

void move(float latitude_goal,float latitude,float longitude_goal,float longitude,float ramuda0,float h){
	float V_left;
	float V_right;
	float wide=602;
	float n=8.5;
	struct robo drone1[2];

	drone1[0].x=latitude_goal;
	drone1[1].x=latitude;		
	drone1[0].y=longitude_goal;
	drone1[1].y=longitude;
	drone1[1].ramuda0=ramuda0;
	drone1[1].v=200;//200mm/s
	
	drone1[1].ramuda=atan((drone1[0].y-drone1[1].y)/(drone1[0].x-drone1[1].x));
	drone1[1].omg=n*(drone1[1].ramuda-drone1[1].ramuda0)/h;

	V_left=drone1[1].v+drone1[1].omg*wide/2;			
	V_right=drone1[1].v-drone1[1].omg*wide/2;

	sirial1(V_left);
	sirial1(V_right);


}
		
	
	


