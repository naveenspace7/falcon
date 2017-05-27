#include "engineFrame.h"

using namespace std;

engineFrame::engineFrame(string name, int id)
{	
	base = get_base(); // Obtain the base address for this engine
	*(base+id) = getpid(); // Put the engine process's PID into the predefine SHM node
	engine_name = name;
}

void engineFrame::setLock(int lock)
{
	// Locks or unlocks the SHM variable defined at Lock location.
	*(base+Lock) = lock;
}

int engineFrame::getLock()
{
	// Gives the present lock condition - whether locked or unlocked
	return *(base+Lock);
}

engineFrame::~engineFrame()
{	
 	// TODO: make the status of the signal to stop
	cout << "Destroying the object" << endl;
}

int engineFrame::getState()
{
 int engAtt = *(base+id);
 state = (engAtt & 0xffff0000) >> 16;
}

void engineFrame::setState(int State)
{
 int engAtt = *(base+id);
 *(base+id) = (State << 16) | engAtt;
 state = State;
}

int* get_base()
{
	// Abstraction for obtaining the access to SHM, returns the base ptr
	key_t thisKey = ftok("~/share_drive/master/key_file",'S');
	int shmid = shmget(thisKey, 800, 0666 | IPC_CREAT);
	int* shm = (int*)shmat(shmid, (void*)0, 0);
	return shm;
}

void engineFrame::compute_start()
{
	// Obtain the time at the start of compute function
	start = chrono::high_resolution_clock::now();
}

void engineFrame::compute_end()
{
	// Obtain the time at the end of compute function
	end = chrono::high_resolution_clock::now();	
}

void engineFrame::print_duration()
{
	// Calculate the time taken to execute compute function
	chrono::milliseconds time_taken = chrono::duration_cast<chrono::milliseconds>(end - start);
	//vec_time.push_back(time_taken);	
	cout << engine_name << ": Ran for " << time_taken.count() << " ms" << endl;	
}