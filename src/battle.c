/*
Os tipos de navios são: porta-aviões (cinco quadrados) (1), 
navios-tanque (quatro quadrados) (2), contratorpedeiros (três quadrados) (3) e submarinos (dois quadrados) (4)

Campo: O => campo livre		N => navios 	X => pedaço navio afundado	Y => mar atacado

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
#define TOTAL 30

/*
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
*/
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
void printOwnField(char **field, int player){
	int len;
	char string[270] = "Campo player ";
	char aux[1];
	sprintf(string +strlen(string), "%d", player);
	//sprintf(string + strlen(string), "%d", player);
	//strcat(string, aux);
	strcat(string, "\n  A B C D E F G H I J\n");
	printf("test 1: %s", string);
	for(int i = 0; i < LENGTH; i++){
		int arg = i+1;
		sprintf(string + strlen(string), "%d", arg);
		strcat(string, " ");
		for(int j = 0; j < LENGTH; j++){
			if(field[i][j] == 'N')
				strcat(string, "N ");
			else if(field[i][j] == 'Y')
				strcat(string, "Y ");
			else if(field[i][j] == 'X')
				strcat(string, "X ");
			else if(field[i][j] == 'O')
				strcat(string, "O ");
			/*aux[0] = field[i][j];
			len = strlen(string);
			string[len++] = field[i][j];
			string[len] = '\0';
			//sprintf(string, "%c", field[i][j]);*/
			//strcat(string, aux);
			/////////strcat(string, " ");
			//printf("%c ", field[i][j]);
		}
		strcat(string, "\n");
		//printf("\n");
	}
	strcat(string, "\n");
	printf("%s", string);
	/*
	sprintf(string + strlen(string), "\n  A B C D E F G H I J\n");
	for(int i = 0; i < LENGTH; i++){
		sprintf(string + strlen(string), )
		printf("%d ", i+1);
		for(int j = 0; j < LENGTH; j++)
			printf("%c ", field[i][j]);
		printf("\n");
	}*/
	
	//free(string);
	/*
	printf("Campo player %d\n", player);
	printf("  A B C D E F G H I J\n");
	for(int i = 0; i < LENGTH; i++){
		printf("%d ", i+1);
		for(int j = 0; j < LENGTH; j++)
			printf("%c ", field[i][j]);
		printf("\n");
	}
	printf("\n");
	*/
	
}

void printOpponentField(char **field, int player){
	printf("Campo player %d\n", player);
	printf("   A B C D E F G H I J\n");
	for(int i = 0; i < LENGTH; i++){
		printf("%d  ", i+1);
		for(int j = 0; j < LENGTH; j++){
			if(field[i][j] != 'N')	// If it's not a ship.
				printf("%c ", field[i][j]);
			else					// If it's a ship, prints as an empty place.
				printf("O ");
		}
		printf("\n");
	}
	printf("\n");
}

// Check if a ship can be placed in a given position. If yes, place it and return true. Otherwise, returns false.
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
						for(int j = i + 1; j <= lineBegin; j++)	
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
						for(int j = i - 1; j >= lineBegin; j--)
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
						for(int j = i + 1; j <= colBegin; j++)
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
						for(int j = i - 1; j >= colBegin; j--)
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
void setShips(char **field, char ship[20], int size, int player, int rep){
	int lineBegin, colBegin, lineEnd, colEnd;
	char c, or;
	char string[100];
	bool validInput;

	for(int i = 0; i < rep; i++){
		strcpy(string, "Diga a primeira coordenada do ");
		validInput = false;
		while(!validInput){
			sprintf(string + strlen(string), "%d", i+1);
			strcat(string, "o. ");
			strcat(string, ship);
			strcat(string, " e sua orientação: ['C']: cima/ ['B']: baixo/ ['D']: direita/ ['E']: esquerda. Ele ocupa ");
			sprintf(string + strlen(string), "%d", size);
			strcat(string, " posicoes: ");
			//printf("Diga a primeira coordenada do %do. %s e sua orientação: ['C']: cima/ ['B']: baixo/ ['D']: direita/ ['E']: esquerda. Ele ocupa %d posicoes: ", i+1, ship, size);
			printf("%s", string);
			scanf("%d %c %c", &lineBegin, &c, &or);
			lineBegin--;
			colBegin = getColumnIndex(c);
			//printf("colBegin = %d\n", colBegin);
			if(placeShip(field, lineBegin, colBegin, or, size)){		// Place and print the ship
				validInput = true;
				printOwnField(field, player);
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

// Returns true if a piece of a ship was sinked. Otherwise, returns false.
bool mainLoop(char **opponentField, int player){
	int opponentPlayer, line, col;
	char c;
	bool validInput = false;

	if(player == 1)
		opponentPlayer = 2;
	else
		opponentPlayer = 1;

	printOpponentField(opponentField, opponentPlayer);
	while(validInput == false){
		printf("Digite as coordenadas da posicao a ser atacada: ");
		scanf("%d %c", &line, &c);
		line--;
		col = getColumnIndex(c);

		// Campo: O => campo livre		N => navios 	X => pedaço navio afundado	Y => mar atacado
		if(line >= 0 && line <= 9 && col >= 0 && col <= 9){		// If it's a valid position
			if(opponentField[line][col] != 'X' && opponentField[line][col] != 'Y'){	// If it wasn't already attacked
				if(opponentField[line][col] == 'N')
					opponentField[line][col] = 'X';
				opponentField[line][col] = 'Y';
				return true;
			}
			else
				printf("Voce ja atacou esta posicao. Tente novamente.\n");
		}
		printf("Por favor, digite uma posicao valida.\n");
	}
	return false;
}

int main(int argc, char *argv[]){
	int i, j, pointsOne = 0, pointsTwo = 0, turn = 1;
	char sub[20] = "submarino", cont[20] = "contratorpedeiro", tanq[20] = "navio-tanque", port[20] = "porta-aviao";

	char **fieldOne = getField();
	char **fieldTwo = getField();

	resetField(fieldOne);
	resetField(fieldTwo);

	// void setShips(char **field, char ship[20], int size, int player, int rep)

	// Set submarines player one
	printOwnField(fieldOne, 1);
	setShips(fieldOne, sub, 2, 1, 4);
	// Set submarines player two
	printOwnField(fieldTwo, 2);
	setShips(fieldTwo, sub, 2, 2, 4);

	// Set contratorpedeiros player one
	printOwnField(fieldOne, 1);
	setShips(fieldOne, cont, 3, 1, 3);
	// Set contratorpedeiros player two
	printOwnField(fieldTwo, 2);
	setShips(fieldTwo, cont, 3, 2, 3);

	//navios-tanque (quatro quadrados) (2)
	// Set navios-tanque player one
	printOwnField(fieldOne, 1);
	setShips(fieldOne, tanq, 4, 1, 2);
	// Set navios-tanque player two
	printOwnField(fieldTwo, 2);
	setShips(fieldTwo, tanq, 4, 2, 2);

	//porta-aviões (cinco quadrados) (1)
	// Set porta-aviões player one
	printOwnField(fieldOne, 1);
	setShips(fieldOne, port, 5, 1, 1);
	// Set porta-aviões player two
	printOwnField(fieldTwo, 2);
	setShips(fieldTwo, port, 5, 2, 1);

	// Main loop
	while(pointsOne < 30 && pointsTwo < 30){
		if(turn == 1){
			if(mainLoop(fieldTwo, 1) == true)
				pointsOne++;
			turn = 2;
		}
		else{
			if(mainLoop(fieldOne, 2) == true)
				pointsTwo++;
			turn = 1;
		}
	}

	if(pointsOne == 30)
		printf("Parabens, player 1!\n");
	else
		printf("Parabens, player 2!\n");

	return 0;
}