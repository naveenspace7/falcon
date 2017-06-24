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
#include <termios.h>
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

#define PORT 2018

using namespace std;

bool recording = false;
bool init_done = false;
string received_str = "";
vector<string> payload;

#endif