#ifndef ARD_H
#define ARD_H
#include <iostream>
#include "engineFrame.h"
#include "signals.h"
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

class arduino: public engineFrame
{
private:
	int fd, baud;
public:
	arduino();
	// Serial attributes
	void serial(const char*, int);
	int Open(const char*);
	int Send(int);
	void Close();
	int len_of(char*);
	int ping();
	int query(int);
	int get_ser_data();
	void test();
	// Engine attributes
	static void compute(int);
	void init();
	
};

shared_ptr<signals> usr_rt;
shared_ptr<signals> usr_lt; 
shared_ptr<signals> usr_fr; 
shared_ptr<signals> usr_rr; 

shared_ptr<signals> wheel_r; 
shared_ptr<signals> wheel_l;

shared_ptr<signals> speed; 

shared_ptr<signals> power_l; 
shared_ptr<signals> power_r; 

shared_ptr<signals> ir_fr; 
shared_ptr<signals> ir_rr;

shared_ptr<signals> angle;
shared_ptr<signals> angle_cmd;

shared_ptr<signals> lock;

arduino Engine;

#endif
