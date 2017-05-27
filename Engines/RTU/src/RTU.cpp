#include "RTU.h"

using namespace std;
using namespace pugi;

void background_monitor(const map<int, string>& signal_map)
{
	vector<int> tcp_payload;

	// Start doing the search here and send the TCP
	// Make TCP connection here
	// Send mainly the engines' running status
}

int main(int argc, char *argv[])
{
	int* shm;

	// Signal address to signal name map
	map<int,string> sig_map; // address - name
	map<string, int> name_addr; // name - address

	make_map(sig_map, name_addr);

	thread TCPthread(background_monitor, cref(sig_map));

	//+++ Start of Socket config code +++
	struct sockaddr_in si_me, si_other;
	int s,i, recv_len;
	socklen_t slen = sizeof(si_other);
	char buff[1024];
	
	if((s=socket(AF_INET,SOCK_DGRAM,0))==-1) //Creating socket
		return -1;
	memset((char *)&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(s,(struct sockaddr*)&si_me, sizeof(si_me))==-1) //Binding the IP address and the socket
		return -1;
	//--- End of Socket config code ---

	shm = get_base();

	while(1)
	{
		cout << "************" << endl;
		cout << "Waiting for client..." << endl;
		// Blocking socket - receive
		if((recv_len=recvfrom(s,buff,sizeof(buff),0,(struct sockaddr*)&si_other,&slen))==-1)
			return -1;		

		cout << "Request Received : " << buff << endl;
		cout << "Data Decoded: " << endl;

		int rec_cmd = atoi(buff); // Converting received string to integer
		int rec_addr = obtain_address(rec_cmd);
		
		cout << "Signal Name: " << sig_map[rec_addr] << endl;

		switch((int)rec_cmd & 7) //Checking the command bits
		{
			case 1: //Read command
			{			
				cout << "Command: Read"	<< endl;
				int temp_val = *(shm+rec_addr); //Reading the value from the SHM
				char temp_buff[10]; //Temp to convert the read value into string
				sprintf(temp_buff,"%d\0",temp_val);	//Converting the int to string
				cout << "Fetched " << temp_buff << " from SHM" << endl;		
				if(sendto(s,temp_buff,get_size(temp_buff),0,(struct sockaddr*)&si_other,slen)==-1) //Sending the value back to the Host
					return -1;		
				break;
			}
			case 2: // Write Command
			{
				cout << "Command: Write"	<< endl;
				int temp_new_val = ((rec_cmd & VAL) >> VALOFF); //Obtaining the new value to be written from the payload
				cout << "New value: " << temp_new_val << endl;
				*(shm+rec_addr) = temp_new_val;	//writing the new value into SHM
				if(sendto(s,"Done",4,0,(struct sockaddr*)&si_other,slen)==-1) //Sending response back to the Host
					return -1;
				break;
			}
		}

		cout << "\n" << endl;
	}

	if(TCPthread.joinable())
		TCPthread.join();

 	return 0;
}


// Takes the input of char array and return the valid length
int get_size(char *s)
{
	int count(0);
	for(int i=0; i<=10; i++)
	{
		if(s[i] != '\0')
			count ++;
		else
			break;
	}
	return count;
}

// Used to obtain address from the Packet received
int obtain_address(int rec_cmd)
{
	int rec_addr = ((rec_cmd & ADR) >> ADDOFF); //Obtaining the Address from the Packet received
	cout << "Address: " << rec_addr << endl;
	return rec_addr;
}

// Populate the map with values read from the XML file
void make_map(map<int,string>& sig_map, map<string,int>& sig_map2)
{
	/*
	Create something like this:
	sig_map[21] = "usr_right";
	sig_map[22] = "usr_left";	
	sig_map[52] = "ir_rear";*/

	int address;
	string name;

	// Open XML file
	xml_document doc;
	xml_parse_result result = doc.load_file("signals.xml");

	if(!result)
		cout << "Error reading the XML signals file" << endl;

	xml_node signals = doc.child("robot").child("signals");
	for(xml_node signal = signals.first_child(); signal; signal = signal.next_sibling())
	{
		// Operate on signal
		address = atoi(signal.attribute("address").value());
		name = signal.attribute("name").value();
		sig_map[address] = name;
		sig_map2[name] = address;
	}
}