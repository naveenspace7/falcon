#include "util.h"

int* get_base()
{
	key_t thisKey = ftok("~/share_drive/master/key_file",'S');
	int shmid = shmget(thisKey, 800, 0666 | IPC_CREAT);
	int* shm = (int*)shmat(shmid, (void*)0, 0);
	return shm;
}