#include "dcap.h"

using namespace std;

vector<string> decode_string(string inp)
{
	char temp[1024];
	vector<string> buffer;
	strcpy(temp,inp.c_str());
	pair<int,int> position;

	// TODO: Make this efficient
	ostringstream ost;
	ost << temp[1];
	buffer.push_back(ost.str());
	
	position = make_pair(inp.find('[') + 1, inp.find(']') - inp.find('[') - 1);
	buffer.push_back(inp.substr(position.first,position.second));
	
	position = make_pair(inp.find("],") + 2, inp.find(",\'") - inp.find("],") - 2);
	buffer.push_back(inp.substr(position.first,position.second));
	
	position = make_pair(inp.find(",\'") + 2, inp.find("\'}") - inp.find(",\'") - 2);
	buffer.push_back(inp.substr(position.first,position.second));
	
	return buffer;
}

// Converts string into vector
vector<int> recover(string inp)
{
	vector<int> temp;
	stringstream ss(inp);
	int i;
	while(ss >> i)
	{
		temp.push_back(i);
		if(ss.peek() == ',')
			ss.ignore();
	}
	return temp;
}

int thread_function()
{
	// One time running
	map<int,string> adr_name; // address - name
	map<string, int> name_adr; // name - address
	signals::make_map(adr_name, name_adr);
	//cout << "sig_map:" << adr_name[3] << endl; // Debug
	int* base = get_base();
	ofstream outfile;
	vector<int> address;
	int counter = 0;
	int sample_time = 0;
	bool file_open = false;

	while(1)
	{
		if(recording == true && init_done == false)
		{
			sample_time = 1000/stoi(payload.at(2)); // Get the rate in MS from frequency
			cout << "Sample_time" << endl;
			outfile.open(payload[3]); // Opening the file here with the name received in the payload
			file_open = true;
			address = recover(payload.at(1));
			{
				{
					ostringstream row;
					row << "Timestamp,";
					outfile << row.str();
					//cout << row.str(); // Debug
				}
				// Writing the headings of the CSV
				for(auto n: address)
				{
					ostringstream row;
					row << adr_name[n] << ',';
					outfile << row.str();
					//cout << row.str(); // Debug
				}
				outfile << '\n';
				//cout << '\n'; // Debug
			}						
			init_done = true;
		}
			
		else if(recording == true && init_done == true)
		{
			//write the data into the file
			{
				ostringstream row;
				row << counter*sample_time << ','; // Timestamp
				for(int each_addr : address)
					row << *(base+each_addr) << ",";
				row << '\n';
				//cout << row.str(); // Debug
				outfile << row.str();
				counter++;
				usleep(sample_time*1000); // TODO: read this from the payload				
			}
		}
		else
		{
			if(file_open)
			{
				init_done = false;
				outfile.close();
				counter = 0;
				file_open = false;
			}
			
		}

	}
}

int main()
{
	thread FileThread(thread_function);
	string received_string = "";

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


	while(1)
	{
		cout << "***** DCAP Engine *****" << endl;
		cout << "Waiting for DCap client..." << endl;
		if((recv_len=recvfrom(s,buff,sizeof(buff),0,(struct sockaddr*)&si_other,&slen))==-1)
			return -1;	

		cout << "Request Received : " << buff << endl;
		received_string = buff;
		payload = decode_string(received_string);

		//cout << payload.at(0) << endl; // Debug
		if(payload.at(0) == "1")
			recording = true;
		else
			recording = false;
	}
	
		
	//string received_string = "{1,[31,32,33,1,5,6],100,'temp.csv'}";
	//payload = decode_string(received_string);
		
	return 0;
}