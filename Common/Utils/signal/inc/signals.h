#ifndef SIGNAL_H
#define SIGNAL_H

#include "pugixml.hpp"
#include <iostream>
#include <string>

class signals
{
private:
	int address;
	int*base;

public:
	signals(std::string,int*&);
	void set(int);
	int get();
};

#endif
