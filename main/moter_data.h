#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include <sys/ioctl.h>
#include <linux/joystick.h>

int joy_fd( -1 ) , num_of_axis( 0 ) , num_of_buttons( 0 );
char name_of_joystick[80];
vector<char> joy_button;
vector<int> joy_axis;
js_event js;
