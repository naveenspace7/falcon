#ifndef MESS_H
#define MESS_H

#include <vector>
#include <string>

using namespace std;

#define ADR 2040 //Mask for address
#define VALOFF 11 //New write value Offset
#define ADDOFF 3 //Address value Offset

class Message
{
private:
	int numberOfRequests;
	std::vector<pair<int,int>> decode(const std::string&);
	string trim_string(string);

public:
	Message(const std::string&);
	int getNumberOfRequests();
	std::vector<pair<int,int>> requests;
	static int obtain_address(int);
};

#endif