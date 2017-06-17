#include "master.h"

using namespace std;

void Engine::run()
{		
	cout << "Running this or what " << *(base+id) << endl;

	if(*(base+id) == pid) // Checking the status
		kill(pid,10); // Good to go
	else
		cout << "Looks like the engine is paused. INFO: PID=" << pid << " Status=" << *(base+id) << endl;			
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
	memset(base, 0, SIZE*sizeof(int));
	cout << "Clearing the SHM" << endl;
	Engine Arduino(1);
	usleep(1000000);	// Giving some time for the master to settle in

	while(1)
	{		
		Arduino.run();	
		usleep(1000000);
	}
 	return 0;
}
