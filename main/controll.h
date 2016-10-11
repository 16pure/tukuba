#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fstream>
#include "set_controller.h"
#include "controll.h"
#define JOY_DEV "/dev/input/js0"

using namespace std;

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

int controll ( float &L , float &R )
{

	read ( joy_fd , &js , sizeof ( js_event ) );

	switch ( js.type & ~JS_EVENT_INIT ) {
		case JS_EVENT_AXIS:
			joy_axis[( int )js.number] = js.value;
			break;
	}

	float x = - joy_axis[2] / 32768.0;
	float y = - joy_axis[3] / 32768.0;

	L = ( ( y - x ) + 1.0 ) * 500.0;
	R = ( ( y + x ) + 1.0 ) * 500.0;

	if ( L > 900.0 ) {
		L = 900.0;
	}
	if ( L < 100.0 ) {
		L = 100.0;
	}
	if ( R > 900.0 ) {
		R = 900.0;
	}
	if ( R < 100.0 ) {
		R = 100.0;
	}
		
	return 0;

}
