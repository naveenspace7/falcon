#include "Message.h"
#include <iostream>
#include <sstream>

using namespace std;

Message::Message(const string& newRequest): mNumberOfSignals(0), mCommand(-1), mRecord(false)
{
	vector<string> firstSplit = DecodeString(newRequest);
  mCommand = stoi(firstSplit.at(0));

  if (mCommand == 1)
  {
    mRecord = true;
    mRate = stoi(firstSplit.at(2));
    mSamplingTime = 1000/mRate; // Get the rate in MS from frequency
    mFileName = firstSplit.at(3);
    mVectorOfAddrs = recover(firstSplit.at(1));
    mNumberOfSignals = mVectorOfAddrs.size();
  }
}
// Takes the request input string and makes it into vector of string
vector<string> Message::DecodeString(const string& newRequest)
{
  string trimmed_string(TrimString(newRequest));
  cout << trimmed_string << endl; // Debug
  vector<string> params;
  
  for (auto buff = strtok(&trimmed_string[0], ";"); buff != NULL; buff = strtok(NULL, ";"))
    params.push_back(buff);

  #if DEBUG==0 //Debug
  for (auto i : params)
    cout << i << endl;
  #endif

  return params;
}

// Trims the first and trailing braces in a string
string Message::TrimString(const string& inputStr)
{
  int len = inputStr.length();
  string trimmed_string = inputStr.substr(1,len-2);
  return trimmed_string;
}

int Message::GetSamplingTime()
{
  return mSamplingTime;
}

string Message::GetFileName()
{
  return mFileName;
}

// Converts addresses string into addresses vector
vector<int> Message::recover(string& addrs)
{ 
  addrs = TrimString(addrs);
  vector<int> temp;
  stringstream ss(addrs);
  int i;
  while(ss >> i)
  {
    temp.push_back(i);
    if(ss.peek() == ',')
      ss.ignore();
  }
  return temp;
}

vector<int> Message::GetRecordAddrs()
{
  return mVectorOfAddrs;
}