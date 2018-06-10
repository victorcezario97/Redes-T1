#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <server_client.h>

#define CONMSG1 "Conectado! Aguardando outro jogador...\n"
#define CONMSG2 "Conectado! Começando jogo...\n"
#define bool int
#define true 1
#define false 0
#define LENGTH 10
#define TOTAL 30

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
	return -1;
}

// Set all the field's places as 'O'
void resetField(char **field){
	for(int i = 0; i < LENGTH; i++){
		for(int j = 0; j < LENGTH; j++)
			field[i][j] = 'O';
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
void printOwnField(char **field, int player, int client){
	char string[270] = "Campo player ";
	sprintf(string +strlen(string), "%d", player);
	strcat(string, "\n  A B C D E F G H I J\n");

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
		}
		strcat(string, "\n");
	}
	strcat(string, "\n");

	int er = write((client), string, strlen(string));
	if(er < 0) printf("ERROR WRITE\n");
}

void printOpponentField(char **field, int player, int client){
	char string[270] = "Campo player ";

	//printf("Campo player %d\n", player);
	sprintf(string +strlen(string), "%d", player);
	strcat(string, "\n  A B C D E F G H I J\n");
	printf("   A B C D E F G H I J\n");
	for(int i = 0; i < LENGTH; i++){
		// printf("%d  ", i+1);
		int arg = i+1;
		sprintf(string + strlen(string), "%d", arg);
		strcat(string, "  ");
		for(int j = 0; j < LENGTH; j++){
			if(field[i][j] != 'N'){	// If it's not a ship.
				if(field[i][j] == 'Y')
					strcat(string, "Y ");
				else if(field[i][j] == 'X')
					strcat(string, "X ");
				else if(field[i][j] == 'O')
					strcat(string, "O ");
					//printf("%c ", field[i][j]);
			}
			else					// If it's a ship, prints as an empty place.
				strcat(string, "O ");
		}
		strcat(string, "\n");
	}
	strcat(string, "\n");

	int er = write((client), string, strlen(string));
	if(er < 0) printf("ERROR WRITE\n");
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
	return false;
}

// Set a player's ship
void setShips(char **field, char ship[20], int size, int player, int rep, int client){
	int lineBegin, colBegin;
	char c, or, input[10], linAux, colAux, orAux;
	char string[100];
	bool validInput;
	int er;

	for(int i = 0; i < rep; i++){
		validInput = false;
		strcpy(string, "Diga a primeira coordenada do ");
		while(!validInput){
			sprintf(string + strlen(string), "%d", i+1);
			strcat(string, "o. ");
			strcat(string, ship);
			strcat(string, " e sua orientação: ['C']: cima/ ['B']: baixo/ ['D']: direita/ ['E']: esquerda. Ele ocupa ");
			sprintf(string + strlen(string), "%d", size);
			strcat(string, " posicoes: ");

			// Writing at client
			er = write((client), string, strlen(string));
			if(er < 0) printf("ERROR WRITE\n");

			//Reading a message from the server
			er = read(client, &input, sizeof(input));
			if(er < 0){
				printf("ERROR READ\n");
				return;
			}

			linAux = input[0];
			colAux = input[2];
			orAux = input[4];

			lineBegin = linAux - '0';
			c = colAux;
			or = orAux;
			//scanf("%d %c %c", &lineBegin, &c, &or);
			lineBegin--;
			colBegin = getColumnIndex(c);
			//printf("colBegin = %d\n", colBegin);
			if(placeShip(field, lineBegin, colBegin, or, size)){		// Place and print the ship
				validInput = true;
				printOwnField(field, player, client);
			}
			else{
				// Writing at client
				strcpy(string, "Coordenadas invalidas!\n");
				er = write((client), string, strlen(string));
				if(er < 0) printf("ERROR WRITE\n");
			}
		}
	}
}

// Returns true if a piece of a ship was sinked. Otherwise, returns false.
bool mainLoop(char **opponentField, int player, int client){
	int opponentPlayer, line, col, er;
	char c, input[10], string[100];
	bool validInput = false;

	if(player == 1)
		opponentPlayer = 2;
	else
		opponentPlayer = 1;

	printOpponentField(opponentField, opponentPlayer, client);
	while(validInput == false){
		strcpy(string, "Digite as coordenadas da posicao a ser atacada: ");
		er = write((client), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");
		//printf("Digite as coordenadas da posicao a ser atacada: ");

		//Reading a message from the server
		er = read(client, &input, sizeof(input));
		if(er < 0){
			printf("ERROR READ\n");
			return -1;
		}

		line = input[0] - '0';
		c = input[2];
		//scanf("%d %c", &line, &c);
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
			else{
				strcpy(string, "Voce ja atacou esta posicao. Tente novamente.\n");
				er = write((client), string, strlen(string));
				if(er < 0) printf("ERROR WRITE\n");
				//printf("Voce ja atacou esta posicao. Tente novamente.\n");
			}
		}
		strcpy(string, "Por favor, digite uma posicao valida.\n");
		er = write((client), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");
	}
	return false;
}

//setShips(char **field, char ship[20], int size, int player, int rep)
bool positionsLoop(char **field, int player, int client){
	char sub[20] = "submarino", cont[20] = "contratorpedeiro", tanq[20] = "navio-tanque", port[20] = "porta-aviao";
	char string[7] = "Done";

	// Set submarines
	printOwnField(field, player, client);
	setShips(field, sub, 2, player, 4, client);

	// Set contratorpedeiros player one
	printOwnField(field, player, client);
	setShips(field, cont, 3, player, 3, client);

	// Set navios-tanque player one
	printOwnField(field, player, client);
	setShips(field, tanq, 4, player, 2, client);

	// Set porta-aviões player one
	printOwnField(field, player, client);
	setShips(field, port, 5, player, 1, client);

	strcpy(string, "Por favor, digite uma posicao valida.\n");
	int er = write((client), string, strlen(string));
	if(er < 0) printf("ERROR WRITE\n");

	return true;
}

int main(int argc, char *argv[]){

	int sockfd, cli1_sockfd, cli2_sockfd;
	struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
	socklen_t clilen1, clilen2;
	int port;

	int er;

	int pointsOne = 0, pointsTwo = 0, turn = 1;
	bool playerOneReady = false, playerTwoReady = false;
	char string[30];

	if(argc < 2){
		printf("usage %s: port\n", argv[0]);
		return -1;
	}

	port = atoi(argv[1]);

	//Starting the server
	startServer(&sockfd, &serv_addr, (unsigned short)port);

	//Listening on the created socket
	listen(sockfd, 5);

	//Accepting the 2 clients
	acceptClient(&cli_addr1, &clilen1, sockfd, &cli1_sockfd, CONMSG1);
	acceptClient(&cli_addr2, &clilen2, sockfd, &cli2_sockfd, CONMSG2);


	// Starting game
	char **fieldOne = getField();
	char **fieldTwo = getField();

	resetField(fieldOne);
	resetField(fieldTwo);

	// char **field, int player, int client
	playerOneReady = positionsLoop(fieldOne, 1, cli1_sockfd);
	playerTwoReady = positionsLoop(fieldTwo, 2, cli2_sockfd);

	while(playerOneReady == false || playerTwoReady == false);

	// Main loop
	while(pointsOne < 30 && pointsTwo < 30){
		strcpy(string, "Aguarde o adversario\n");
		if(turn == 1){
			// Prints wait
			er = write((cli2_sockfd), string, strlen(string));
			if(er < 0) printf("ERROR WRITE\n");

			if(mainLoop(fieldTwo, 1, cli1_sockfd) == true)
				pointsOne++;
			turn = 2;
		}
		else{
			// Prints wait
			er = write((cli1_sockfd), string, strlen(string));
			if(er < 0) printf("ERROR WRITE\n");

			if(mainLoop(fieldOne, 2, cli2_sockfd) == true)
				pointsTwo++;
			turn = 1;
		}
	}

	// End game
	if(pointsOne == 30){
		strcpy(string, "Parabens, player 1!\n");
		// Prints wait
		er = write((cli1_sockfd), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");

		strcpy(string, "Voce perdeu!\n");
		// Prints wait
		er = write((cli2_sockfd), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");
	}
	else{
		strcpy(string, "Parabens, player 2!\n");
		// Prints wait
		er = write((cli2_sockfd), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");

		strcpy(string, "Voce perdeu!\n");
		// Prints wait
		er = write((cli1_sockfd), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");
	}

	return 0;
}
