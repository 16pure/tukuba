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
#include "gps.h"

 
#define JOY_DEV "/dev/input/js0"
#define SERIAL_PORT1 "/dev/ttyUSB0"
#define SERIAL_PORT2 "/dev/ttyUSB1"


using namespace std;

// シリアルポートの初期化
void serial_init(int fd)
{
  struct termios tio;
  memset(&tio,0,sizeof(tio));
  tio.c_cflag = CS8 | CLOCAL | CREAD;
  tio.c_cc[VTIME] = 100;
  // ボーレートの設定
  cfsetispeed(&tio,B57600);
  cfsetospeed(&tio,B57600);
  // デバイスに設定を行う
  tcsetattr(fd,TCSANOW,&tio);
}


void sirial0(int fd,char* buf){
    if(fd<0){
	exit(1);
	cout<<"no"<<endl;
	}
    strcpy(buf,"350\r");
    write(fd, buf , sizeof(buf));
}

void sirial1(int fd,char* buf,char* str,int send){
    if(fd<0){
	cout<<"no"<<endl;
	exit(1);
	cout<<"no"<<endl;
	}
    sprintf(str, "%d\r", send);
    strcpy(buf,str);
    write(fd, buf , sizeof(buf));
}

int main ()
{
	int fd_L,fd_R;
	char buf1[10];
	char str1[10];
	char buf2[10];
	char str2[10];
	//set_TKK();
	//serial_init1(fd);
	//fd_L = open(SERIAL_PORT1,O_RDWR);
	fd_R = open(SERIAL_PORT2,O_RDWR);
	//serial_init(fd_L);
	serial_init(fd_R);	

	int L,R;
	int joy_fd( -1 ) , num_of_axis( 0 ) , num_of_buttons( 0 );
	char name_of_joystick[80];
	vector<char> joy_button;
	vector<int> joy_axis;
	int count_button[17];
	js_event js;
	if ( ( joy_fd = open( JOY_DEV, O_RDONLY ) ) < 0 ) {
		printf( "Failed to open %s" ,JOY_DEV );
		cerr << "Failed to open " << JOY_DEV << endl;
		return -1;
	}


	ioctl( joy_fd , JSIOCGAXES , &num_of_axis );
	ioctl( joy_fd , JSIOCGBUTTONS , &num_of_buttons );
	ioctl( joy_fd , JSIOCGNAME(80) , &name_of_joystick );

	joy_button.resize( num_of_buttons , 0 );
	joy_axis.resize( num_of_axis , 0 );

	printf( "Joystick: %s axis: %d buttons: %d\n" ,name_of_joystick ,num_of_axis ,num_of_buttons );

	fcntl( joy_fd, F_SETFL, O_NONBLOCK ); // using non-blocking mode



while(1){
	read ( joy_fd , &js , sizeof ( js_event ) );

	switch ( js.type & ~JS_EVENT_INIT ) {
		case JS_EVENT_AXIS:
			joy_axis[( int )js.number] = js.value;
			break;
		case JS_EVENT_BUTTON:
			joy_button[( int )js.number] = js.value;
			cout <<(int)js.number<<endl;
			break;
	}


	float x = - joy_axis[0] / 32768.0;
	float y = - joy_axis[1] / 32768.0;

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

	if ( joy_button[3] == 1 ) {
		count_button[3]++;
		if ( count_button[3] == 10 ) {
			break;
		}
	}
	else if ( joy_button[14] == 1 ) {
		count_button[14]++;
		if ( count_button[14] == 10 ) {
			cout<<"ok"<<endl;
		}
	}
	else if ( joy_button[12] == 1 ) {
		count_button[12]++;
		if ( count_button[12] == 10 ) {
			cout<<"ok"<<endl;
		}
	}
	else if ( joy_button[15] == 1 ) {
			count_button[15]++;
		if ( count_button[15] == 10 ) {
			
			cout<<"ok"<<endl;
		}
	}
	else if ( joy_button[13] == 1 ) {
		count_button[13]++;
		if ( count_button[13] == 10 ) {
			while(gps()==1);
		}
	}
	else if( joy_button[13] == 0) count_button[13]=0;
	else if( joy_button[3] == 0) count_button[3]=0;
	else if( joy_button[12] == 0) count_button[12]=0;
	else if( joy_button[15] == 0) count_button[15]=0;
	else if( joy_button[14] == 0) count_button[14]=0;
	else {	
		
	}

	//sirial1(fd_L,buf1,str1,L);
	sirial1(fd_R,buf2,str2,R);


	
}
	close(fd_L);
	close(fd_R);		
	return 0;


}


