#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>
int main(){
	int desc;
	int semid;
	char* sh;
	char s[128];
	struct sembuf sb;
	desc=ftok("pisos", 45);
	semid=semget(desc, 1, 0666| IPC_CREAT);
	desc=shmget(desc, 256, 0);
	sh=shmat(desc, NULL, 0);
	sb.sem_op=-1;
	sb.sem_flg=0;
	sb.sem_num=0;
	semop(semid, &sb, 1);
	strncpy(s, sh+4, *((int*) sh));
	printf("%s", s);
	shmdt(sh);
	return 0;
}
