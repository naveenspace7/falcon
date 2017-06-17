#include "master.h"

using namespace std;

void run(Engine eng)
{	
	kill(eng.pid,10); // Good to go
	/*
	cout << "Running this or what " << *(base+id) << endl;

	if(*(base+id) == pid) // Checking the status
		kill(pid,10); // Good to go
	else
		cout << "Looks like the engine is paused. INFO: PID=" << pid << " Status=" << *(base+id) << endl;		
		*/
}

Engine::Engine(int temp_id)
{
	pid = -1;
	id = temp_id;
	*(base + id) = 1; // Allows the engines to run only after this
	while(*(base+id) == 1);
	pid = *(base+id);
}

int main()
{	
	/*
	int pid;
	cin >> pid;
	for(int i = 0; i<10; i++)
	{
		kill(pid,10);
		usleep(3000000);
	}
	*/
	
	memset(base, 0, SIZE*sizeof(int));
	cout << "Clearing the SHM" << endl;
	Engine Arduino(1);
	int pid;
	cin >> pid;

	while(1) // Keep the process alive and do nothing until compute is called
	{
		//run(Arduino);
//		int p = Arduino.pid;
		kill(pid,10);
		usleep(1000000);
	}
 	return 0;
}
