#include "RTU.h"

using namespace std;
using namespace pugi;

// Removes the trailing and preceding braces
string trim_string(string inp)
{
	int len = inp.length();
	string trimmed_string = inp.substr(1, len - 2);
	return trimmed_string;
}

/*input: "{234,456,678}"
output: vector[234,456,678]*/
vector<pair<int,int>> decode(string payload_str)
{	
	istringstream str_buff(trim_string(payload_str));
	vector<pair<int,int>> payload_pair; // first: element_no, second: request
	int count = 0; // element_no
	for (string each; getline(str_buff, each, ','); payload_pair.push_back(make_pair(count++,stoi(each))));
	return payload_pair;
}

// Actual code for handling the requests: will be a thread
void perform_action(pair<int, int> payload_pair)
{
	int payload_pointer = payload_pair.first;
	int command = payload_pair.second;
	int rec_addr = obtain_address(command);	
	
	mx.lock();
	switch ((int)command & 7) // Checking the command bits
	{
		case 1: //Read command
		{			
			read_flag = true;
			string log_msg;
			log_msg = "Cmd=R, Name:" + sig_map[rec_addr];
			syslog(LOG_INFO, "%s", log_msg.c_str());
			//cout << "Name-" << sig_map[rec_addr] << endl; // DEBUG
			//cout << "Cmd-R" << endl; // DEBUG
			int temp_val = *(shm + rec_addr); // Reading the value from the SHM
			read_values[payload_pointer] = to_string(temp_val); // Put it into the right place		
			break;
		}

		case 2: // Write Command
		{
			read_flag = false;
			string log_msg;
			log_msg = "Cmd=W, Name:" + sig_map[rec_addr] + ", ";
			//cout << "Name-" << sig_map[rec_addr] << endl; //DEBUG
			//cout << "Cmd-W" << endl; //DEBUG
			int temp_new_val = ((command & VAL) >> VALOFF); //Obtaining the new value to be written from the payload
			//cout << "New value: " << temp_new_val << endl;
			log_msg += "Value:" + to_string(temp_new_val);
			syslog(LOG_INFO, "%s", log_msg.c_str());
			*(shm + rec_addr) = temp_new_val;	//writing the new value into SHM
			break;
		}
	}
	mx.unlock();
}

// Pack the read values into a ready to be sent form
string pack_read_payload()
{
	string return_str = "{";
	for(auto& each_content: read_values)
		return_str += each_content + ",";
	return_str += "}";
	return return_str;
}

int main(int argc, char *argv[])
{
	//setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog("Falcon:RTU", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
	#if DEBUG == 0	// MAKE ME A DAEMON
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

	syslog(LOG_INFO, "Starting RTU Engine");
	// Daemon code ends here and actual code start from here

	shm = get_base(); // Obtain the base address of the shared memory
	
	signals::make_map(sig_map, name_addr); // Signal address to signal name map

	//thread TCPthread(background_monitor, cref(sig_map));	
	string buff; // Buffer to store the incoming request

	while(1)
	{		
		// Blocking socket - receive
		buff = new_sock->sock_recv();
		if (buff == "ERROR")
			return -1;	
		
		vector<pair<int,int>> str_buff(decode(buff));
		vector<thread> threads;

		read_values.resize(str_buff.size()); // Make the size to the received length

		string decoded = "Decoded:" + buff;
		//cout << "decoded:" << buff << endl;
		syslog(LOG_INFO, "%s", decoded.c_str());

		for (vector<pair<int, int>>::iterator ele = str_buff.begin(); ele != str_buff.end(); ele++)
		{
			threads.push_back(thread(perform_action, *ele));
		}

		for (auto& each_thr : threads)
		{
			each_thr.join();
		}
	
		if(read_flag)
			new_sock->sock_send(pack_read_payload());						
		else
			new_sock->sock_send(string("WDone"));
	}

 	return 0;
}

// Used to obtain address from the Packet received
int obtain_address(int rec_cmd)
{
	int rec_addr = ((rec_cmd & ADR) >> ADDOFF); //Obtaining the Address from the Packet received
	//Deletecout << "Address: " << rec_addr << endl;
	return rec_addr;
}