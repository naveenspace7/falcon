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
				usleep(sample_time); // TODO: read this from the payload
				if(counter == 5)
					break;
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

	string recvd_str = "{1,[31,32,33,1,5,6],100,'temp.csv'}";
	payload = decode_string(recvd_str);
	
	//cout << payload.at(0) << endl; // Debug
	if(payload.at(0) == "1")
		recording = true;
	else
		recording = false;

	//while(1);
	usleep(1000000);
	
	return 0;
}