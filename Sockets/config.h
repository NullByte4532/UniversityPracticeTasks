typedef enum {J_SWORDSMAN, J_ARCHER, J_MAGE} Job;
typedef enum {S_SWORDSMAN_ATTACK, S_SWORDSMAN_SHIELD, S_ARCHER_ATTACK, S_ARCHER_SHOOT, S_MAGE_HEAL, S_MAGE_POISON} Skill;

typedef struct{
	char** field;
	int h, w;
} Map;
typedef struct{
	char name1[16];
	char name2[16];
	char name3[16];
	char name4[16];
	int health[4];
	int x[4];
	int y[4];
	Job job[4];
} LobbyInfo;

#define S_SWORDSMAN_ATTACK_RANGE 4
#define S_ARCHER_ATTACK_RANGE 2
#define S_ARCHER_SHOOT_RANGE 25
#define S_MAGE_HEAL_RANGE 9
#define S_MAGE_POISON_RANGE 16
