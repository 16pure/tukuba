#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fstream>
#define JOY_DEV "/dev/input/js0"

using namespace std;

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

int joy_fd( -1 ) , num_of_axis( 0 ) , num_of_buttons( 0 );
char name_of_joystick[80];
vector<char> joy_button;
vector<int> joy_axis;
js_event js;

int set_controller()
{

	//js_event js;

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

}
