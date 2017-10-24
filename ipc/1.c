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
	char* s="PISOS\n";
	struct sembuf sb;
	desc=ftok("pisos", 45);
	printf("%d\n",desc);
	semid=semget(desc, 1, 0666|IPC_CREAT);
	desc=shmget(desc, 256, 0666|IPC_CREAT);
	printf("%d\n", desc);
	sh=shmat(desc, NULL, 0);
	printf("%x\n", sh);
	sb.sem_op=1;
	sb.sem_flg=0;
	sb.sem_num=0;
	*((int*) sh)=strlen(s);
	strcpy(sh+4, s);
	semop(semid, &sb, 1);
	shmdt(sh);
	return 0;
}
