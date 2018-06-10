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
void printColumnIndex(int j){
	switch(j){
		case 0:
			printf("A ");
			break;
		case 1:
			printf("B ");
			break;
		case 2:
			printf("C ");
			break;
		case 3:
			printf("D ");
			break;
		case 4:
			printf("E ");
			break;
		case 5:
			printf("F ");
			break;
		case 6:
			printf("G ");
			break;
		case 7:
			printf("H ");
			break;
		case 8:
			printf("I ");
			break;
		case 9:
			printf("J ");
			break;
	}
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

// Check if a ship can be placed in a given position. If yes, place it and return true. Else, return false.
bool placeShip(int lineBegin, int colBegin, char direction, int size, char **field){
	if(lineBegin >= 0 && lineBegin <= 9 && colBegin >= 0 && colBegin <= 9){
		switch(direction){
			case 'C':	// Up
				int lim = lineBegin - size;
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
				int lim = lineBegin + size;
				if(lim > 9)
					return false;

				for(int i = lineBegin; i <= lim; i++){	
					if(field[i][colBegin] == 'N'){	// If the ship cannot be placed on this position and orientation.
						for(int j = i; j >= lineBegin; j--)
							field[j][colBegin] = 'O';
						return false;
					}
				}

				return true;
				break;
			case 'E':	// Left
				int lim = colBegin - size;
				if(lim < 0)
					return false;

				for(int i = colBegin; i >= lim; i--){
					if(field[lineBegin][i] == 'N')
						return false;
				}

				return true;
				break;
			case 'D':	// Right
				int lim = colBegin + size;
				if(lim > 9)
					return false;

				for(int i = colBegin; i <= lim; i++){
					if(field[lineBegin][i] == 'N')
						return false;
				} 
				return true;
				break;
		}
	}
	else
		return false;
}

// Set a player's ship
char ** setShips(char **field, char[15] ship, int size){
	int lineBegin, colBegin, lineEnd, colEnd;
	char c;
	bool validInput = false;

	for(int i = 0; i < 4; i++){
		while(!validInput){
			printf("Diga a primeira coordenada do %do. %s e sua orientação: ['C']: cima/ 
				['B']: baixo/ ['D']: direita/ ['E']: esquerda. Ele ocupa %d posicoes: ", i, ship, size);
			scanf("%d %c", lineBegin, c);
			colBegin = c - '0';

			if(checkShipPosition(lineBegin, colBegin, c, size, field)){		// Print the ship
				validInput = true;
				if(c == 'C'){
					int lim = lineBegin - size;
					for(int x = lineBegin; x >= lim; x--)
						field[x][colBegin] = 'N';
				}
				else if(c == 'B'){
					int lim = lineBegin + size;
					for(int x = lineBegin; x <= lim; x++)
						field[x][colBegin] = 'N';
				}
				else if(c == 'E'){
					int lim = colBegin - size;
					for(int x = colBegin; x >= lim; x--)
						field[lineBegin][x] = 'N';
				}
				else if(c == 'D'){
					int lim = colBegin + size;
					for(int x = colBegin; x<= size; x++)
						field[lineBegin][x] = 'N';
				}
			}
		}
	}
}

int main(int argc, char *argv[]){
	int i, j;
	char **fieldOne = getField();
	char **fieldTwo = getField();

	bool gameOver = false;

	for(i = 0; i < 1; i++){

	}

	while(!gameOver){

	}
}