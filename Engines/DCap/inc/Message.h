#ifndef MESS_H
#define MESS_H

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

using namespace std;

#define ADR 2040 //Mask for address
#define VALOFF 11 //New write value Offset
#define ADDOFF 3 //Address value Offset

class Message
{
private:
	string TrimString(const string&);
	vector<string> DecodeString(const string&);
	vector<int> recover(string&);
	int mCommand;
	int mRate;
	int mSamplingTime;
	vector<int> mVectorOfAddrs;
	int mNumberOfSignals;
	string mFileName;
	vector<int> signalAddresses;
	std::vector<pair<int,int>> decode(const std::string&);

public:
	bool mRecord;
	Message(const std::string&);
	vector<int> GetRecordAddrs();
	string GetFileName();
	int GetSamplingTime();

	int getNumberOfRequests();
	std::vector<pair<int,int>> requests;
	static int obtain_address(int);
};

#endif