#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "config.h"

void drawMap(char** map, int map_w, int map_h, char code){
	int x, y;
	printf("\033[19;0H");
	for (y=0; y<map_h; y++){
		for(x=0; x<map_w; x++){
			if(map[y][x]==code) printf("\e[1m");
			if(map[y][x]==0) printf("\e[2m.\e[22m");
			else if(map[y][x]==1){ 
				printf("\e[31m1\e[39m\e[21m");
			}
			else if(map[y][x]==3){
				printf("\e[34m3\e[39m\e[21m");
			}
			else if(map[y][x]==2){
				printf("\e[31m2\e[39m\e[21m");
			}
			else if(map[y][x]==4){
				printf("\e[34m4\e[39m\e[21m");
			}

		}
		printf("\n");
	}


}
void destroyMap(Map* map){
	int i;
	for (i=0; i<map->h; i++) free(map->field[i]);
	free(map->field);
	free(map);
}
void config(char* code, int conn, Job* job, char** jobTitles){
	int tmp=-1;
	char name[16];
	printf("\033[2J");
	puts("Enter your name:");
	printf(">");
	fgets(name, 15, stdin);
	if( send(conn , name , strlen(name)+1 , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
	while(tmp!=0&&tmp!=1&&tmp!=2){	
		printf("\033[2J");
		puts("Choose Class:");
		printf("1 %s\n", jobTitles[0]);
		printf("2 %s\n", jobTitles[1]);
		printf("3 %s\n", jobTitles[2]);
		printf(">");
		scanf("%d", &tmp);
		tmp--;
	}
	*job=tmp;
	if( send(conn , job , sizeof(Job) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
	if( recv(conn, code , 1 , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
}
void receiveMap(Map* map, int conn){
	int i;
	for(i=0; i<map->h; i++){
		if( recv(conn, map->field[i] , map->w , 0) < 0)
		{
		puts("Communication Error");
		exit(-1);
		}
	}
	

}
Map* generateMap(int map_h, int map_w){
	Map* map;
	int i;
	map=calloc(1, sizeof(Map));
	map->field=calloc(map_h, sizeof(char*));
	for (i=0; i<map_h; i++){
		map->field[i]=calloc(map_w, sizeof(char));
	}
	map->h=map_h;
	map->w=map_w;
	return map;

}
void redrawScreen(Map* map, LobbyInfo* lobbyInfo, char code, char** jobTitles, char** lastEvents, int turn){
	int i;
	printf("\033[2J");
	drawMap(map->field, map->w, map->h, code);
	printf("\e[31m");
	if(turn==1) printf("\e[5m");
	if(code==1) printf("\e[1m");
	printf("\033[1;0H1\033[1;4H%s\033[1;20H%s\033[1;36H%d\e[21m\e[25m", lobbyInfo->name1, jobTitles[lobbyInfo->job[0]], lobbyInfo->health[0]);
	if(turn==2) printf("\e[5m");
	if(code==2) printf("\e[1m");
	printf("\033[2;0H2\033[2;4H%s\033[2;20H%s\033[2;36H%d\e[21m\e[25m", lobbyInfo->name2, jobTitles[lobbyInfo->job[1]], lobbyInfo->health[1]);
	printf("\e[34m");
	if(turn==3) printf("\e[5m");
	if(code==3) printf("\e[1m");
	printf("\033[3;0H3\033[3;4H%s\033[3;20H%s\033[3;36H%d\e[21m\e[25m", lobbyInfo->name3, jobTitles[lobbyInfo->job[2]], lobbyInfo->health[2]);
	if(turn==4) printf("\e[5m");
	if(code==4) printf("\e[1m");
	printf("\033[4;0H4\033[4;4H%s\033[4;20H%s\033[4;36H%d\e[21m\e[25m", lobbyInfo->name4, jobTitles[lobbyInfo->job[3]], lobbyInfo->health[3]);
	printf("\e[39m");
	for(i=0; i<4; i++) printf("\033[%d;40H|", i+1);
	printf("\e[2m");
	for(i=0; i<4; i++){
		printf("\033[%d;42H%s", i+1, lastEvents[i]);
	}
	printf("\e[22m");
	fflush(stdout);
}
void receiveLobbyInfo(int conn, LobbyInfo* lobbyInfo){

	if( recv(conn, lobbyInfo , sizeof(LobbyInfo) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}


}

void doMove(Map* map, LobbyInfo* lobbyInfo, char code, char** jobTitles, char** lastEvents, int conn, int turn){
	int tmp=-1;
	while(tmp!=0&&tmp!=1&&tmp!=2&&tmp!=3){
		redrawScreen(map, lobbyInfo, code, jobTitles, lastEvents, turn);
		printf("\033[10;30HCoose direction:");
		if(lobbyInfo->y[code-1]>0) printf("\033[12;30H1) Up");
		if(lobbyInfo->y[code-1]<map->h-1) printf("\033[13;30H2) Down");
		if(lobbyInfo->x[code-1]>0) printf("\033[14;30H3) Left");
		if(lobbyInfo->x[code-1]<map->w-1) printf("\033[15;30H4) Right");
		printf("\033[16;30H>");
		scanf("%d", &tmp);
		tmp--;
		if(tmp==0 && lobbyInfo->y[code-1]<=0)tmp=-1;
		if(tmp==1 && lobbyInfo->y[code-1]>=map->h-1)tmp=-1;
		if(tmp==2 && lobbyInfo->x[code-1]<=0)tmp=-1;
		if(tmp==3 && lobbyInfo->x[code-1]>=map->w-1)tmp=-1;
	}
	if( send(conn , &tmp , sizeof(int) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
	tmp=-1;
	if( send(conn , &tmp , sizeof(int) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
}
void doShield(int conn){
	int tmp;
	if( send(conn , &tmp , sizeof(int) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
	tmp=1;
	if( send(conn , &tmp , sizeof(int) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
}
int getRange(Skill type){
	switch(type){
		case S_SWORDSMAN_ATTACK:
			return 4;
		case S_ARCHER_ATTACK:
			return 2;
		case S_ARCHER_SHOOT:
			return 25;
		case S_MAGE_HEAL:
			return 9;
		case S_MAGE_POISON:
			return 16;
	}

}
int square(int x){
	return x*x;
}
int distance(int a, int b, LobbyInfo* lobbyInfo){
	return square(lobbyInfo->x[a]-lobbyInfo->x[b])+square(lobbyInfo->y[a]-lobbyInfo->y[b]);
}
void doAttack(Map* map, LobbyInfo* lobbyInfo, char code, char** jobTitles, char** lastEvents, int conn, int lasttmp, Skill type, int turn){
	int tmp=-1;
	while(tmp!=0&&tmp!=1&&tmp!=2&&tmp!=3){
		redrawScreen(map, lobbyInfo, code, jobTitles, lastEvents, turn);
		printf("\033[10;30HChoose target:");
		if(distance(code-1, 0, lobbyInfo)<=getRange(type)&&(type!=S_ARCHER_SHOOT||distance(code-1, 0, lobbyInfo)>2)) printf("\033[12;30H1) 1");
		if(distance(code-1, 1, lobbyInfo)<=getRange(type)&&(type!=S_ARCHER_SHOOT||distance(code-1, 1, lobbyInfo)>2)) printf("\033[13;30H2) 2");
		if(distance(code-1, 2, lobbyInfo)<=getRange(type)&&(type!=S_ARCHER_SHOOT||distance(code-1, 2, lobbyInfo)>2)) printf("\033[14;30H3) 3");
		if(distance(code-1, 3, lobbyInfo)<=getRange(type)&&(type!=S_ARCHER_SHOOT||distance(code-1, 3, lobbyInfo)>2)) printf("\033[15;30H4) 4");
		printf("\033[16;30H>");
		scanf("%d", &tmp);
		tmp--;
		if(distance(code-1, tmp, lobbyInfo)>getRange(type)||(type==S_ARCHER_SHOOT&&distance(code-1, tmp, lobbyInfo)<=2))tmp=-1;

	}
	if( send(conn , &tmp , sizeof(int) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
	if( send(conn , &lasttmp , sizeof(int) , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
}
void doAction(Map* map, LobbyInfo* lobbyInfo, char code, char** jobTitles, char** lastEvents, int conn, Job job, int turn){
	int tmp=-1;
	while(tmp!=0&&tmp!=1&&tmp!=2){
		redrawScreen(map, lobbyInfo, code, jobTitles, lastEvents, turn);
		printf("\033[10;30HChoose action:");
		switch(job){
			case J_SWORDSMAN:
				printf("\033[12;30H1) Attack");
				printf("\033[13;30H2) Shield");
				break;
			case J_ARCHER:
				printf("\033[12;30H1) Attack");
				printf("\033[13;30H2) Shoot");
				break;
			case J_MAGE:
				printf("\033[12;30H1) Heal");
				printf("\033[13;30H2) Poison");
				break;
		}
		printf("\033[14;30H3) Walk");
		printf("\033[16;30H>");
		scanf("%d", &tmp);
		tmp--;
	}
	if(lobbyInfo->job[code-1]==J_SWORDSMAN&&tmp==1) doShield(conn);
	else if(tmp==2) doMove(map, lobbyInfo, code, jobTitles, lastEvents, conn, turn);
	else doAttack(map, lobbyInfo, code, jobTitles, lastEvents, conn, tmp, job*2+tmp, turn);
	
}	
int main(int argc , char *argv[])
{
	int socket_desc, i;
	struct sockaddr_in server;
	char server_reply[200];
	char code;
	LobbyInfo lobbyInfo;
	Job job;
	int turn;
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	Map* map;
	char* jobTitles[3];
	char* lastEvents[4];
	jobTitles[0]="Swordsman";
	jobTitles[1]="Archer";
	jobTitles[2]="Mage";
	for(i=0; i<4; i++) lastEvents[i]=calloc(24, sizeof(char));
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
		 
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(4532);
 
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("Connection error");
		return 1;
	}
	 
	puts("Connected\n");

	if( recv(socket_desc, server_reply , 200 , 0) < 0)
	{
		puts("Communication Error");
		exit(-1);
	}
	if(strcmp(server_reply, "INITCONFIG")){
		puts("Protocol Error");
		exit(-1);
	}
	config(&code, socket_desc, &job, jobTitles);
	printf("\033[2J");
	printf("\033[10;20HWaiting for other players...");
	fflush(stdout);
	map=generateMap(10,20);
	receiveMap(map, socket_desc);
	receiveLobbyInfo(socket_desc, &lobbyInfo);
	do{
		if( recv(socket_desc, &turn , sizeof(int) , 0) < 0)
		{
			puts("Communication Error");
			exit(-1);
		}
		redrawScreen(map, &lobbyInfo, code, jobTitles, lastEvents, turn);
		for(i=0; i<3; i++){
			if(turn==code){
				doAction(map, &lobbyInfo, code, jobTitles, lastEvents, socket_desc, job, turn);
			}
			receiveMap(map, socket_desc);
			receiveLobbyInfo(socket_desc, &lobbyInfo);
			redrawScreen(map, &lobbyInfo, code, jobTitles, lastEvents, turn);
			if(lobbyInfo.health[code-1]<=0){
				turn=0;
				break;
			}
		}
	}while(turn>0);
	close(socket_desc);
	destroyMap(map);
	return 0;
}
