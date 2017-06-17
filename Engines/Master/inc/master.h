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
#include <signal.h>

using namespace std;

class Engine
{
public:
	int status, pid, id;	
	void run(void);
	Engine(int);
};

int *base = get_base();
const int SIZE = 400;

#endif