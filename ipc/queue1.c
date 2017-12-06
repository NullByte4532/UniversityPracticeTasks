#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int msqid; 
	char pathname[] = "pisos";
	key_t key; 
	int i,len; 
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
	mybuf.mtype = 1;
	mybuf.num = 45;
	mybuf.returnId = getpid();
	len = sizeof(mybuf);
	if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
		printf("Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}
	if(( len = msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), getpid(), 0) < 0)){
		printf("Can\'t receive message from queue\n");
		exit(-1);
	}
	printf("%d\n", mybuf.num);
	return 0;
}
