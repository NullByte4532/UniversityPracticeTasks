#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREAD_NUM 2


typedef struct
	{
	long mtype;
	long returnId;
	int num;
	int msqid;
	int semid;
	} threadbuf;
void sop(int semid, int op){
	struct sembuf sb;
	sb.sem_op = op;
	sb.sem_flg = 0;
	sb.sem_num = 0;
	semop(semid, &sb, 1);
}
void* processRequest(void* buf){
	printf("Started calculation for %d\n", ((threadbuf*)buf)->returnId);
	typedef struct
	{
		long mtype;
		long returnId;
		int num;
	} mymsgbuf;
	mymsgbuf mybuf;
	mybuf.num = ((threadbuf*)buf)->num*100+32;
	mybuf.mtype = ((threadbuf*)buf)->returnId;
	mybuf.returnId = 1;
	sleep(5);
	if (msgsnd(((threadbuf*)buf)->msqid, (mymsgbuf*) &mybuf, sizeof(mybuf), 0) < 0){
		printf("Can\'t send message to queue\n");
		msgctl(((threadbuf*)buf)->msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}
	free(buf);
	printf("Finished calculation for %d\n", ((threadbuf*)buf)->returnId);
	sop(((threadbuf*)buf)->semid, 1);
	
	return NULL;
}
int main()
{
	int msqid, semid; 
	char pathname[] = "pisos"; 
	key_t key; 
	int len, maxlen;
	struct mymsgbuf
	{
		long mtype;
		long returnId;
		int num;
	} mybuf;

	if((key = ftok(pathname,0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}


	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	} 

	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get semid\n");
		exit(-1);
	} 
	semctl(semid, 0, IPC_RMID);
	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get semid\n");
		exit(-1);
	} 

	sop(semid, MAX_THREAD_NUM);
	while(1){
		mybuf.returnId = 0;
		if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), 1, 0) < 0)){
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}
		if(mybuf.returnId){
			threadbuf* buf;
			buf = malloc(sizeof(threadbuf));
			memcpy(buf, &mybuf,sizeof(mybuf)); 
			buf->msqid = msqid;
			buf->semid = semid;
			pthread_t garbage;
			sop(semid, -1);
			pthread_create(&garbage, NULL, processRequest, buf);
		}

	}
	return 0;
}
