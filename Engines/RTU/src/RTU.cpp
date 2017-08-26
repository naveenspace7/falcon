#include "RTU.h"

using namespace std;
using namespace pugi;

int main(int argc, char *argv[])
{
	//setlogmask(LOG_UPTO(LOG_NOTICE));
	#if DEBUG == 0
	openlog("RTU", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
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

	//syslog(LOG_INFO, "Starting RTU Engine");
	// Daemon code ends here and actual code start from here

	int* shm = get_base();

	// Signal address to signal name map
	map<int,string> sig_map; // address - name
	map<string, int> name_addr; // name - address

	signals::make_map(sig_map, name_addr);

	//thread TCPthread(background_monitor, cref(sig_map));	

	while(1)
	{		
		// Blocking socket - receive

		string buff(new_sock->sock_recv());
		if (buff == "ERROR")
			return -1;		

		
		//cout << "Req: " << buff << endl;
		//Deletecout << "Data Decoded: " << endl;

		int rec_cmd = stoi(buff); // Converting received string to integer
		int rec_addr = obtain_address(rec_cmd);		

		switch((int)rec_cmd & 7) //Checking the command bits
		{
			case 1: //Read command
			{
				cout << "Name-" << sig_map[rec_addr] << endl;
				cout << "Cmd-R"	<< endl;
				int temp_val = *(shm+rec_addr); //Reading the value from the SHM
				char temp_buff[10]; //Temp to convert the read value into string
				sprintf(temp_buff,"%d\0",temp_val);	//Converting the int to string
				new_sock->sock_send(string(temp_buff));	
				break;
			}
			case 2: // Write Command
			{
				cout << "Name-" << sig_map[rec_addr] << endl;
				cout << "Cmd-W"	<< endl;
				int temp_new_val = ((rec_cmd & VAL) >> VALOFF); //Obtaining the new value to be written from the payload
				cout << "New value: " << temp_new_val << endl;
				*(shm+rec_addr) = temp_new_val;	//writing the new value into SHM
				new_sock->sock_send(string("Done"));	
				break;
			}
		}

		cout << "\n" << endl;
	}

	closelog();
 	return 0;
}

// Used to obtain address from the Packet received
int obtain_address(int rec_cmd)
{
	int rec_addr = ((rec_cmd & ADR) >> ADDOFF); //Obtaining the Address from the Packet received
	//Deletecout << "Address: " << rec_addr << endl;
	return rec_addr;
}