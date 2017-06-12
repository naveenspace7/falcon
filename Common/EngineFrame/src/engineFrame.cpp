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
	setLock(1);
	// TODO: Calculate the frequency and display here
	chrono::milliseconds time_taken = chrono::duration_cast<chrono::milliseconds>(start - start_old);
	
	vec_time.push_back(time_taken.count());

	if(vec_time.size() == 5)
		cout << engine_name <<" Running at frequency " << 1.0/average_time(vec_time) << " Hz."<< endl; // FIXME: Correct the frequency
}

int engineFrame::average_time(vector<int> time_vector)
{
	int temp_count = 0;
	for(auto each : time_vector)	
		temp_count += each;
	return temp_count/time_vector.size(); // TODO: Write this to SHM, but first create a SHM variable for that.
}

void engineFrame::compute_end()
{
	// Obtain the time at the end of compute function
	end = chrono::high_resolution_clock::now();	
	setLock(0);
	print_duration();	// Debug: Print the run-time duration
}

void engineFrame::print_duration()
{
	// Calculate the time taken to execute compute function
	chrono::milliseconds time_taken = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << engine_name << ": Ran for " << time_taken.count() << " ms" << endl;	
}