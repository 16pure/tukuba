#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#include <termios.h>
#include <strings.h>
#include <iterator>
#include <algorithm>
#include <sysexits.h>
#include<string.h>


#define JOY_DEV "/dev/input/js0"
#define SERIAL_PORT1 "/dev/ttyUSB0"
#define SERIAL_PORT2 "/dev/ttyUSB1"


using namespace std;

int button (int joy_fd,int num_of_axis,int num_of_buttons,int fd)
{
	char name_of_joystick[80];
	vector<char> joy_button;
	vector<int> joy_axis;
	
	if ( ( joy_fd = open( JOY_DEV, O_RDWR ) ) < 0 ) {
//		printf( "Failed to open %s" ,JOY_DEV );
//		cerr << "Failed to open " << JOY_DEV << endl;
		cout<<"kkkkk"<<endl;
		return -1;
	}

	ioctl( joy_fd , JSIOCGAXES , &num_of_axis );
	ioctl( joy_fd , JSIOCGBUTTONS , &num_of_buttons );
	ioctl( joy_fd , JSIOCGNAME(80) , &name_of_joystick );

	joy_button.resize( num_of_buttons , 0 );
	joy_axis.resize( num_of_axis , 0 );

//	printf( "Joystick: %s axis: %d buttons: %d\n" ,name_of_joystick ,num_of_axis ,num_of_buttons );

	fcntl( joy_fd, F_SETFL, O_NONBLOCK ); // using non-blocking mode
}
int button_a(int joy_fd , int num_of_axis , int num_of_buttons,int fd,vector<char>joy_button,vector<int> joy_axis){
	js_event js;
	read ( joy_fd , &js , sizeof ( js_event ) );

	switch ( js.type & ~JS_EVENT_INIT ) {
		case JS_EVENT_AXIS:
			joy_axis[( int )js.number] = js.value;
			break;
		case JS_EVENT_BUTTON:
			joy_button[( int )js.number] = js.value;
			//printf("%5d\n %5d\n",( int )js.number,js.value);
			break;
	}

	if ( joy_button[3] == 1 ) {
		cout<<"ok"<<endl;
		return 0;
	}
	else if ( joy_button[14] == 1 ) {
		return 1;
	}
	else if ( joy_button[12] == 1 ) {
		return 2;
	}
	else if ( joy_button[15] == 1 ) {
		return 3;
	}
	else if ( joy_button[13] == 1 ) {
		return 4;
	}
	else {	
		return 5;
	}

}

int main()
{
	vector<char> joy_button;
	vector<int> joy_axis;
	js_event js;
	int fd;
	int joy_fd=-1;
	int num_of_axis=0;
	int num_of_buttons=0;
	int b=button(joy_fd,num_of_axis,num_of_buttons,fd);
	int a;
	while(1){
		cout<<"a"<<endl;
		if(b<0)break;
		a=button_a (joy_fd ,num_of_axis ,num_of_buttons,fd,joy_button,joy_axis);
		if(a!=5){
			cout<<a<<endl;
		}
	}
	return 0;
}
