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
#include "fsock.h"
#include <syslog.h>
#include <memory>
#include <sstream>
#include <string>
#include <mutex>
#include <bitset>

#define PORT 2017
#define SHSIZE 400

//#define VAL 268433408 //Mask for value
#define VAL 134215680 //Mask for value 
#define SIGN 27

#define ADR 2040 //Mask for address
#define VALOFF 11 //New write value Offset
#define ADDOFF 3 //Address value Offset

int obtain_address(int);
string trim_string(string);
vector<pair<int,int>> decode(string);
void perform_action(pair<int, int>);

shared_ptr<make_fsock> new_sock = make_shared<make_fsock>(PORT);

map<int, string> sig_map; // address - name
map<string, int> name_addr; // name - address

int* shm;

mutex mx;
bool read_flag = false;
vector<string> read_values;


#endif