#include "RTU.h"

using namespace std;
using namespace pugi;

int main(int argc, char *argv[])
{
	//setlogmask(LOG_UPTO(LOG_NOTICE));
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

	syslog(LOG_INFO, "Starting RTU Engine");

	// Daemon code ends here and actual code start from here

	int* shm;

	// Signal address to signal name map
	map<int,string> sig_map; // address - name
	map<string, int> name_addr; // name - address

	signals::make_map(sig_map, name_addr);

	//thread TCPthread(background_monitor, cref(sig_map));

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
		// Blocking socket - receive
		if((recv_len=recvfrom(s,buff,sizeof(buff),0,(struct sockaddr*)&si_other,&slen))==-1)
			return -1;		

		syslog(LOG_INFO, "Request: %s", buff);
		//cout << "Req: " << buff << endl;
		//Deletecout << "Data Decoded: " << endl;

		int rec_cmd = atoi(buff); // Converting received string to integer
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
				//cout << "Fetched " << temp_buff << " from SHM" << endl; // DEBUG
				if(sendto(s,temp_buff,get_size(temp_buff),0,(struct sockaddr*)&si_other,slen)==-1) //Sending the value back to the Host
					return -1;		
				break;
			}
			case 2: // Write Command
			{
				cout << "Name-" << sig_map[rec_addr] << endl;
				cout << "Cmd-W"	<< endl;
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

	closelog();
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
	//Deletecout << "Address: " << rec_addr << endl;
	return rec_addr;
}