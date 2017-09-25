#include "dcap.h"

using namespace std;

// Trims the first and trailing braces in a string
string trim_string(string inp)
{
	int len = inp.length();
	string trimmed_string = inp.substr(1,len-2);
	return trimmed_string;
}

// Takes the request input string and makes it into vector of string
vector<string> decode_string(string inp)
{
	string trimmed_string(trim_string(inp));
	cout << trimmed_string << endl;
	vector<string> params;
	
	for (auto buff = strtok(&trimmed_string[0], ";"); buff != NULL; buff = strtok(NULL, ";"))
		params.push_back(buff);

	#if DEBUG==0 //Debug
	for (auto i : params)
		cout << i << endl;
	#endif

	return params;
}

// Converts addresses string into addresses vector
vector<int> recover(string inp)
{	
	inp = trim_string(inp);
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
			outfile.open("/home/pi/Falcon/" + payload[3]); // Opening the file here with the name received in the payload
			file_open = true;
			address = recover(payload.at(1));
			{
				{
					ostringstream row;
					row << "Timestamp (ms),";
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
			start_recording = chrono::system_clock::now();
		}
			
		else if(recording == true && init_done == true)
		{
			//write the data into the file
			{
				ostringstream row;
				instant_point = chrono::system_clock::now();
				time_taken = chrono::duration_cast<chrono::milliseconds>(instant_point - start_recording);
				time_taken_val = time_taken.count();

				row << time_taken_val << ','; // Timestamp
				for(int each_addr : address)
					row << *(base+each_addr) << ",";
				row << '\n';
				//cout << row.str(); // Debug
				outfile << row.str();
				counter++;
				usleep(sample_time*1000);
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
	openlog("Falcon:DCap", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	#if DEBUG==0
	pid_t pid, sid;
	pid = fork();

	if(pid < 0)
		exit(EXIT_FAILURE);
	if(pid > 0)
		exit(EXIT_SUCCESS);
	umask(0);	
	sid = setsid();

	if(sid < 0)
		exit(EXIT_FAILURE);
	if((chdir("/")) < 0)
		exit(EXIT_FAILURE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	#endif
	
	thread FileThread(thread_function);
	string received_string = "";

	syslog(LOG_INFO, "%s", "Started DCap engine");

	while(1)
	{
		string buff(new_sock->sock_recv());
		if (buff == "ERROR")
			return -1;

		string log_msg = "Request Rx:" + buff;
		syslog(LOG_INFO, "%s", log_msg.c_str());
		//cout << "Request Received : " << buff << endl;
		received_string = buff;
		payload = decode_string(received_string);

		//cout << payload.at(0) << endl; // Debug
		if(payload.at(0) == "1")
			recording = true;
		else
			recording = false;

		new_sock->sock_send(string("Done"));
	}
			
	//string received_string = "{1;[31,32,33,1,5,6];100;'temp.csv'}";
	return 0;
}