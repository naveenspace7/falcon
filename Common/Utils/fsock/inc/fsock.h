#ifndef FSOCK_H
#define FSOCK_H

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

using namespace std;

class make_fsock
{
private:
	struct sockaddr_in si_me, si_other;
	int s, i, recv_len;
	socklen_t slen = sizeof(si_other);
	char buff[1024];

public:

	make_fsock(int);

	int sock_send(string);
	
	int sock_recv();
	
	// Takes the input of char array and return the valid length
	int get_size(const char *);	
};

#endif