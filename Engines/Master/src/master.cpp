#include "master.h"

using namespace std;

int Engine::run()
{
	//check the status
	//kill(pid,signal);
}

Engine::Engine(int id)
{
	*(base + id) = 1; // Allows the engines to run only after this
	pid = -1;

}


int main()
{	
	memset(base, 0, SIZE*sizeof(int));
	Engine Arduino(1);

 		
 
 while(1); // Keep the process alive and do nothing until compute is called
 return 0;
}
