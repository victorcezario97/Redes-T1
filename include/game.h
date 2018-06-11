#ifndef _GAME_H_
#define _GAME_H_

typedef struct Ship
{
	char *name;
	int size;
	int n;
} SHIP;

void printField(char **field);
char **getEmptyField();
void placeShips(char **field, SHIP s);
int check(char lin, char col, char dir, char **field, int size);
void readShips();

#endif