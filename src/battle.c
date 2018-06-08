/*
Os tipos de navios são: porta-aviões (cinco quadrados) (1), 
navios-tanque (quatro quadrados) (2), contratorpedeiros (três quadrados) (3) e submarinos (dois quadrados) (4)

Campo: O => campo livre		N => navios 	X => pedaço afundado

Para cada player
	Para cada navio
		Printa tela do player
		Insere início e fim
	Aguarda adversário

Printa "Início"

Para player que atira
	Printa dois campos
	Insere coord

Para player que espera
	Printa dois campos
	Printa "aguarde"

Verifica se afundou
Verifica se terminou
Muda o player que ataca

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false 0
#define LENGTH 10
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9

//enum TURN

// Print the "name" of a column.
int getColumnIndex(char c){
	switch(c){
		case 'A':
			return 0;
			break;
		case 'B':
			return 1;
			break;
		case 'C':
			return 2;
			break;
		case 'D':
			return 3;
			break;
		case 'E':
			return 4;
			break;
		case 'F':
			return 5;
			break;
		case 'G':
			return 6;
			break;
		case 'H':
			return 7;
			break;
		case 'I':
			return 8;
			break;
		case 'J':
			return 9;
			break;
	}
}

// Get an empty field
char **getField(){
	int i, j;

	char **field = (char**)malloc(sizeof(char*) * LENGTH);

	for(i = 0; i < LENGTH; i++)
		field[i] = (char*)malloc(sizeof(char) * LENGTH);

	for(i = 0; i < LENGTH; i++){
		for(j = 0; j < LENGTH; j++)
			field[i][j] = 'O';
	}

	return field;
}

// Print a field
void printField(char **field, int player){
	printf("Campo player %d\n", player);
	printf("A B C D E F G H I J\n");
	for(int i = 0; i < LENGTH; i++){
		printf("%d ", i);
		for(int j = 0; j < LENGTH; j++)
			printf("%c ", field[i][j]);
		printf("\n");
	}
	printf("\n");
}


// Check if a ship can be placed in a given position. If yes, place it and return true. Else, return false.
bool placeShip(char **field, int lineBegin, int colBegin, char direction, int size){
	if(lineBegin >= 0 && lineBegin <= 9 && colBegin >= 0 && colBegin <= 9){
		int lim;
		switch(direction){
			case 'C':	// Up
				lim = lineBegin - size + 1;
				if(lim < 0)
					return false;

				for(int i = lineBegin; i >= lim; i--){
					if(field[i][colBegin] == 'N'){	// If the ship cannot be placed on this position and orientation.
						for(int j = i-1; j <= lineBegin; j++)	
							field[j][colBegin] = 'O';
						return false;
					}
					field[i][colBegin] = 'N';
				}

				return true;
				break;

			case 'B':	// Down
				lim = lineBegin + size - 1;
				if(lim > 9)
					return false;

				for(int i = lineBegin; i <= lim; i++){	
					if(field[i][colBegin] == 'N'){	// If the ship cannot be placed on this position and orientation, then cancels.
						for(int j = i; j >= lineBegin; j--)
							field[j][colBegin] = 'O';
						return false;
					}
					field[i][colBegin] = 'N';
				}

				return true;
				break;

			case 'E':	// Left
				lim = colBegin - size + 1;
				if(lim < 0)
					return false;

				for(int i = colBegin; i >= lim; i--){	
					if(field[lineBegin][i] == 'N'){	// If the ship cannot be placed on this position and orientation, then cancels.
						for(int j = i; j <= colBegin; j++)
							field[lineBegin][j] = 'O';
						return false;
					}
					field[lineBegin][i] = 'N';
				}

				return true;
				break;

			case 'D':	// Right
				lim = colBegin + size - 1;
				if(lim > 9)
					return false;

				for(int i = colBegin; i <= lim; i++){
					if(field[lineBegin][i] == 'N'){	// If the ship cannot be placed on this position and orientation, then cancels.
						for(int j = i; j >= colBegin; j--)
							field[lineBegin][j] = 'O';
						return false;
					}
					field[lineBegin][i] = 'N';
				} 
				return true;
				break;
		}
	}
	else
		return false;
}

// Set a player's ship
void setShips(char **field, char ship[15], int size, int player, int rep){
	int lineBegin, colBegin, lineEnd, colEnd;
	char c, or;
	bool validInput;

	for(int i = 0; i < rep; i++){
		validInput = false;
		while(!validInput){
			printf("Diga a primeira coordenada do %do. %s e sua orientação: ['C']: cima/ ['B']: baixo/ ['D']: direita/ ['E']: esquerda. Ele ocupa %d posicoes: ", i, ship, size);
			scanf("%d %c %c", &lineBegin, &c, &or);
			colBegin = getColumnIndex(c);
			//printf("colBegin = %d\n", colBegin);
			if(placeShip(field, lineBegin, colBegin, or, size)){		// Place and print the ship
				validInput = true;
				printField(field, player);
			}
			else
				printf("Coordenadas invalidas!\n");
		}
	}
}

// Set all the field's places as 'O'
void resetField(char **field){
	for(int i = 0; i < LENGTH; i++){
		for(int j = 0; j < LENGTH; j++)
			field[i][j] = 'O';
	}
}

int main(int argc, char *argv[]){
	int i, j;
	char sub[15] = "submarino";

	char **fieldOne = getField();
	char **fieldTwo = getField();

	resetField(fieldOne);
	resetField(fieldTwo);

	//bool gameOver = false;

	// Set submarines player one
	printField(fieldOne, 1);
	setShips(fieldOne, sub, 2, 1, 4);
	// Set submarines player two
	printField(fieldTwo, 1);
	setShips(fieldTwo, sub, 2, 2, 4);

	return 0;
}