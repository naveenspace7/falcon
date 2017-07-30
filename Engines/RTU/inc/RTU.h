#ifndef RTU_H
#define RTU_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <map>
#include "engineFrame.h"
#include "pugixml.hpp"
#include <thread>
#include "signals.h"
#include <syslog.h>

#define PORT 2017
#define SHSIZE 400

//#define VAL 268433408 //Mask for value
#define VAL 134215680 //Mask for value 

#define ADR 2040 //Mask for address
#define VALOFF 11 //New write value Offset
#define ADDOFF 3 //Address value Offset

int get_size(char*);
int obtain_address(int);

#endif