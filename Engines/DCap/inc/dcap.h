#ifndef DCAP_H
#define DCAP_H

#include <iostream>
#include "engineFrame.h"
#include "signals.h"
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <signal.h>
#include <sstream>
#include <fstream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fsock.h"
#include <syslog.h>
#include <chrono>

#define PORT 2018

using namespace std;

bool recording = false;
bool init_done = false;
string received_str = "";
vector<string> payload;

shared_ptr<make_fsock> new_sock = make_shared<make_fsock>(PORT);

chrono::system_clock::time_point start_recording, instant_point;
chrono::milliseconds time_taken;
int time_taken_val;

#endif