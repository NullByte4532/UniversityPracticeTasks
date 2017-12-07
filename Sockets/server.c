#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>
#include <pthread.h>	
#include <stdlib.h>
#include "config.h"
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
void destroyMap(Map* map){
	int i;
	for (i=0; i<map->h; i++) free(map->field[i]);
	free(map->field);
	free(map);
}
void clearMap(Map* map){
	int x,y;
	for(y=0; y<map->h; y++)
	for(x=0; x<map->w; x++)
	map->field[y][x]=0;
}
typedef struct{
	Job job;
	int x,y;
	int health;
	int conn;
	int sock;
	int shielded;
	char code;
	char connected;
	char name[16];
	Skill skills[2];
} client_info;
void setSkills(client_info* clientInfo){
	switch(clientInfo->job){
		case J_SWORDSMAN:
			clientInfo->skills[0]=S_SWORDSMAN_ATTACK;
			clientInfo->skills[1]=S_SWORDSMAN_SHIELD;
			break;
		case J_ARCHER:
			clientInfo->skills[0]=S_ARCHER_ATTACK;
			clientInfo->skills[1]=S_ARCHER_SHOOT;
			break;
		case J_MAGE:
			clientInfo->skills[0]=S_MAGE_HEAL;
			clientInfo->skills[1]=S_MAGE_POISON;
			break;

	}


}
void* waitForConnection(void *arg){
	int c;
	char* message;
	client_info* clientInfo;
	struct sockaddr_in client;
	clientInfo=arg;
	c = sizeof(struct sockaddr_in);
	clientInfo->conn=accept(clientInfo->sock, (struct sockaddr *)&client, (socklen_t*)&c);
	if(clientInfo->conn<0){
		puts("Connection Failed");
		exit(-1);
	}
	puts("Connection accepted");
	message = "INITCONFIG";
	write(clientInfo->conn , message , strlen(message)+1);
	if( recv(clientInfo->conn, clientInfo->name , 16 , 0) < 0)
	{
		puts("Failed to receive client name");
		exit(-1);
	}
	if( recv(clientInfo->conn, &clientInfo->job , sizeof(Job) , 0) < 0)
	{
		puts("Failed to receive client job pick");
		exit(-1);
	}
	write(clientInfo->conn , &clientInfo->code , 1);
	setSkills(clientInfo);
	clientInfo->health=100;
	clientInfo->connected=1;
	return NULL;
}
void mapPlayers(client_info* players, Map* map){
	map->field[players[0].y][players[0].x]=players[0].code;
	map->field[players[1].y][players[1].x]=players[1].code;
	map->field[players[2].y][players[2].x]=players[2].code;
	map->field[players[3].y][players[3].x]=players[3].code;

}
void setPlayersInitCoords(client_info* players){
	players[0].x=9;
	players[0].y=9;
	players[1].x=11;
	players[1].y=9;
	players[2].x=9;
	players[2].y=0;
	players[3].x=11;
	players[3].y=0;

}
void assignCodes(client_info* players){
	int client_id;
	for(client_id=0; client_id<4; client_id++){
	players[client_id].connected=0;
	players[client_id].shielded=0;
	players[client_id].code=client_id+1;
	}


}
void sendMap(client_info* players, Map* map){
	int i, j;
	for(j=0; j<4; j++)
	for(i=0; i<map->h; i++)
	if(players[j].connected) write(players[j].conn , map->field[i] , map->w);

}
void sendLobbyInfo(client_info* players, Map* map){
	LobbyInfo lobbyInfo;
	int i;
	strcpy(lobbyInfo.name1, players[0].name);
	strcpy(lobbyInfo.name2, players[1].name);
	strcpy(lobbyInfo.name3, players[2].name);
	strcpy(lobbyInfo.name4, players[3].name);
	for(i=0; i<4; i++){
		lobbyInfo.health[i]=players[i].health;
		lobbyInfo.job[i]=players[i].job;
		lobbyInfo.x[i]=players[i].x;
		lobbyInfo.y[i]=players[i].y;
	}
	for(i=0; i<4; i++) if(players[i].connected) write(players[i].conn , &lobbyInfo , sizeof(LobbyInfo));

}
void sendAll(int data, client_info* players){
	int i;
	for(i=0; i<4; i++) if(players[i].connected) write(players[i].conn , &data , sizeof(int));

}
int damage(Job job, Skill skill, int shielded){
	int decrement=1;
	if(skill!=S_MAGE_HEAL&&shielded) decrement=2;  
	switch(skill){
		case S_SWORDSMAN_ATTACK:
			if(job==J_SWORDSMAN) return 10/decrement;
			return 20/decrement;
		case S_ARCHER_ATTACK:
			if(job==J_SWORDSMAN) return 2/decrement;
			return 5/decrement;
		case S_ARCHER_SHOOT:
			if(job==J_SWORDSMAN) return 5/decrement;
			return 10/decrement;
		case S_MAGE_HEAL:
			return -5/decrement;
		case S_MAGE_POISON:
			return 10/decrement;
	}


}
void performAction(client_info* players, Map* map, int turn, int act, int arg){
		
	if(act<0){
		switch(arg){
			case 0:
			players[turn-1].y=players[turn-1].y-1;
			break;
			case 1:
			players[turn-1].y=players[turn-1].y+1;
			break;
			case 2:
			players[turn-1].x=players[turn-1].x-1;
			break;
			case 3:
			players[turn-1].x=players[turn-1].x+1;
			break;
		}
	}
	else if(players[turn-1].skills[act]==S_SWORDSMAN_SHIELD){
		players[turn-1].shielded++;
		return;
	}
	else{
		players[arg].health=players[arg].health-damage(players[arg].job, players[turn-1].skills[act], players[arg].shielded);
		if(players[arg].shielded) players[arg].shielded--;
	}
	
}
void checkDead(client_info* players){
	int i;
	for(i=0; i<4; i++) if(players[i].health<=0){
		close(players[i].conn);
		players[i].connected=0;
	}

}
int main(int argc , char *argv[])
{
	int socket_desc, err, client_id, i;
	client_info players[4];
	struct sockaddr_in server;
	char *message;
	Map* map;
	int turn=0;
	puts("Generating map...");
	map=generateMap(10,20);
	
	puts("Starting server...");
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		puts("Could not create socket");
		exit(-1);
	}
	 
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 4532 );

	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		exit(-1);
	}
	puts("bind done");
	 

	listen(socket_desc , 4);
	assignCodes(players);
	puts("Waiting for incoming connections...");
	pthread_t tid[4];
	for (client_id=0; client_id<4; client_id++){
		players[client_id].sock=socket_desc;
		err = pthread_create(&tid[client_id], NULL, &waitForConnection, &players[client_id]);
		if (err != 0)	printf("\ncan't create thread :[%s]", strerror(err));
	}
	for (client_id=0; client_id<4; client_id++){
		pthread_join(tid[client_id], NULL);
	}
	puts("Placing players on map");
	setPlayersInitCoords(players);
	mapPlayers(players, map);	
	puts("Sending map to clients");
	sendMap(players, map);
	sendLobbyInfo(players, map);
	while((players[0].health>0||players[1].health>0)&&(players[2].health>0||players[3].health>0)){
		turn++;
		sendAll(turn, players);
		for(i=0; i<3; i++){
			int act, arg;
			if( recv(players[turn-1].conn, &arg , sizeof(int) , 0) < 0)
			{
				puts("Failed to receive action parameter");
				exit(-1);
			}
			if( recv(players[turn-1].conn, &act , sizeof(int) , 0) < 0)
			{
				puts("Failed to receive action id");
				exit(-1);
			}
			performAction(players, map, turn, act, arg);
			checkDead(players);
			clearMap(map);
			mapPlayers(players, map);
			sendMap(players, map);
			sendLobbyInfo(players, map);
		}
		turn=turn%4;
	}
	return 0;
}
