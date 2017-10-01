#ifndef SIGNAL_H
#define SIGNAL_H

#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <map>

class signals
{
private:	
	int*base;

public:
	int address;
	signals(std::string,int*&);
	void set(int);
	int get();
	void force(int);
	static void make_map(std::map<int,std::string>&, std::map<std::string,int>&);
};

#endif
