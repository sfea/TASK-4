#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "mylib.h"

#define FILE "/bin/sh"

int main()
{
	key_t key;
	int shmid;
	void* buf;

	if ((key = ftok(FILE, 0)) < 0)
        {
                perror("ftok");
                exit(1);
        }

	if ((shmid = shmget(key, 0, 0666)) < 0)
        {
                perror("shmget");
                exit(1);
        }

	if ((buf = shmat(shmid, 0, SHM_RDONLY)) < 0)
	{
		perror("shmat");
		exit(1);
	}

	new_write(buf, STDOUT_FILENO); 

	if (shmdt(buf) < 0)
	{
		perror("shmdt");
		exit(1);
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0)
	{
		perror("shmctl");
		exit(1);
	}

	printf("\nRead from memory successfully!\n");
	return 0;
}
