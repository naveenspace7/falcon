#include "fsock.h"

make_fsock::make_fsock(int PORT)
	{
		if((s=socket(AF_INET,SOCK_DGRAM,0))==-1) //Creating socket
			exit(1);
			//cout <<"error" << endl; // Make this a logger

		memset((char *)&si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(PORT);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		if(bind(s,(struct sockaddr*)&si_me, sizeof(si_me))==-1) //Binding the IP address and the socket
			exit(1);
	}

int make_fsock::sock_send(string nv)
{
	const char *st = nv.c_str();
	if(sendto(s,st,get_size(st),0,(struct sockaddr*)&si_other,slen)==-1) //Sending the value back to the Host
		return -1;
	return 0;
}

string make_fsock::sock_recv()
{
	// Blocking socket - receive
	if((recv_len=recvfrom(s,buff,sizeof(buff),0,(struct sockaddr*)&si_other,&slen))==-1)
		cout << "ERROR" << endl;
	return string(buff);
}

// Takes the input of char array and return the valid length
int make_fsock::get_size(const char *s)
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


/*int main()
{
	shared_ptr<make_fsock> new_sock = make_shared<make_fsock>(2567);
	//make_fsock new_sock(2567);

	while(1)
	{
		new_sock->sock_recv();
		cout << "done" << endl;
		new_sock->sock_send(string("naveen"));
	}
	
	return 0;
}*/