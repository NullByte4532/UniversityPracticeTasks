#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>
#include <pthread.h>	
#include <stdlib.h>
#include "config.h"
typedef struct{
	char** field;
	int h, w;
} Map;
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
	char code;
	char connected;
	Skill skills[2];
} client_info;
void setSkills(client_info* clientInfo){
	switch(clientInfo->job){
		case J_SWORDSMAN:
			clientInfo->skills[0]=S_SWORDSMAN_ATTACK;
			clientInfo->skills[1]=S_SWORDSMAN_SHIELD;
			break;
		case J_ARCHER:
			clientInfo->skills[0]=S_ARCHER_SHOOT;
			clientInfo->skills[1]=S_ARCHER_RAIN;
			break;
		case J_MAGE:
			clientInfo->skills[0]=S_MAGE_HEAL;
			clientInfo->skills[1]=S_MAGE_POISON;
			break;

	}


}
int main(int argc , char *argv[])
{
	


}
