#include "dcap.h"

using namespace std;

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
		if(record == true && init_done == false)
		{
			sample_time = rxRequest->GetSamplingTime();
			outfile.open("/home/pi/Falcon/" + rxRequest->GetFileName()); // Opening the file here with the name received in the payload
			file_open = true;
			address = rxRequest->GetRecordAddrs();
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
			
		else if(record == true && init_done == true)
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
	StartUpOperations();

	thread FileThread(thread_function);

	string buff;

	while(1)
	{
		buff = new_sock->sock_recv();
		if (buff == "ERROR")
			return -1;

		string log_msg = "Request Rx:" + buff;
		syslog(LOG_INFO, "%s", log_msg.c_str());

		rxRequest = new Message(buff);

		//payload = decode_string(buff);

		cout << rxRequest->mRecord << endl; // Debug

		if(rxRequest->mRecord)
			record = true;
		else
			record = false;

		new_sock->sock_send(string("Done"));
	}
			
	//string received_string = "{1;[31,32,33,1,5,6];100;'temp.csv'}";
	return 0;
}

// Performs some basic logging and house keeping task
void StartUpOperations()
{
  openlog("Falcon:DCap", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
  

  #if DEBUG == 0 // MAKE ME A DAEMON
  RunAsDaemon();
  #endif

  syslog(LOG_INFO, "Starting DCap Engine");

  shm = get_base(); // Obtain the base address of the shared memory
}

// Forking done for creating a Daemon
void RunAsDaemon()
{
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
}