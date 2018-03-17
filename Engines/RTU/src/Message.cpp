#include "Message.h"
#include <iostream>
#include <sstream>

using namespace std;

Message::Message(const string& newRequest): numberOfRequests(0)
{
	requests = decode(newRequest);
	numberOfRequests = requests.size();
}

/*input: "{234,456,678}"
output: vector[234,456,678]*/
vector<pair<int,int>> Message::decode(const string& payload_str)
{	
  istringstream str_buff(trim_string(payload_str));
  vector<pair<int,int>> payload_pair; // first: element_no, second: request
  int count = 0; // element_no
  for (string each; getline(str_buff, each, ','); payload_pair.push_back(make_pair(count++,stoi(each))));
  return payload_pair;
}

// Returns the number of request received
int Message::getNumberOfRequests()
{
	return numberOfRequests;
}

// Removes the trailing and preceding braces
string Message::trim_string(string inp)
{
  int len = inp.length();
  string trimmed_string = inp.substr(1, len - 2);
  return trimmed_string;
}

// Used to obtain address from the Packet received
int Message::obtain_address(int rec_cmd)
{
  int rec_addr = ((rec_cmd & ADR) >> ADDOFF); //Obtaining the Address from the Packet received
  //cout << "Address: " << rec_addr << endl; // Debug
  return rec_addr;
}