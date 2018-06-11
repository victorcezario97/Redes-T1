#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <server_client.h>

#define bool int
#define true 1
#define false 0
#define LENGTH 10

char *convert(char **f){
	char *s = (char *)malloc(sizeof(char) * 100);

	
	for(int i=0; i< LENGTH; i++){
		for(int j=0; j<LENGTH; j++){
			s[(i*10)+j] = f[i][j];
		}
	}

	return s;
}

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

// Print a field
void printOwnField(char **field){
	char string[270] = "Campo player ";
	//sprintf(string +strlen(string), "%d", player);
	strcat(string, "\n  A B C D E F G H I J\n");

	for(int i = 0; i < LENGTH; i++){
		sprintf(string + strlen(string), "%d", i);
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
	//int er = write((client), string, strlen(string));
//printf("print own after write\n");

//	if(er < 0) printf("ERROR WRITE\n");
	printf("%s\n", string);
}

// Set a player's ship
void setShips(char **field, char ship[20], int size, int rep){
	int lineBegin, colBegin;
	char c, or, input[10], linAux, colAux, orAux;
	//char string[500];
	bool validInput;
	//int er;

	for(int i = 0; i < rep; i++){
		validInput = false;
		//bzero(string, sizeof(string));
		
		while(!validInput){
			printf("Diga a primeira coordenada do ");
			printf("%d", i+1);
			printf("o. %s", ship);
			printf(" e sua orientação: ['C']: cima/ ['B']: baixo/ ['D']: direita/ ['E']: esquerda. Ele ocupa %d posicoes: ", size);



			//string[strlen(string)] = '\0';
			// Writing at client...das instrucoes
			//printf("%s\n", string);
			//er = write((client), string, strlen(string));
			//if(er < 0) printf("ERROR WRITE\n");
			//Reading a message from the server
			/*er = read(client, &input, sizeof(input));
			if(er < 0){
				printf("ERROR READ\n");
				return;
			}*/
			scanf("%s", input);
			getchar();
			printf("INPUT: %s\n", input);
			linAux = input[0];
			colAux = input[1];
			orAux = input[2];

			lineBegin = linAux - '0';
			c = colAux;
			or = orAux;
			//scanf("%d %c %c", &lineBegin, &c, &or);
			colBegin = getColumnIndex(c);
			printf("CHEGOU AQUI\n");
			//printf("colBegin = %d\n", colBegin);
			if(placeShip(field, lineBegin, colBegin, or, size)){		// Place and print the ship
				validInput = true;
				printOwnField(field);
			}
			else{
				// Writing at client
				printf("Coordenadas invalidas!\n");
				//i--;
				//er = write((client), string, strlen(string));
				//if(er < 0) printf("ERROR WRITE\n");
			}
		}
	}
}

void printOpponentField(char **field, int player, int client){
	char string[270] = "Campo player ";

	//printf("Campo player %d\n", player);
	sprintf(string +strlen(string), "%d", player);
	strcat(string, "\n  A B C D E F G H I J\n");
	for(int i = 0; i < LENGTH; i++){
		// printf("%d  ", i+1);
		sprintf(string + strlen(string), "%d", i);
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
	string[strlen(string)] = '\0';

	int er = write((client), string, strlen(string));
	if(er < 0) printf("ERROR WRITE\n");
}

bool positionsLoop(char **field){
	char sub[20] = "submarino";//, cont[20] = "contratorpedeiro", tanq[20] = "navio-tanque", port[20] = "porta-aviao";
	//char string[40] = "Done\0";
	//char buffer[2];
	//int er;

	// Set submarines
	printOwnField(field);
	//printf("Printou o mapa\n");
	//er = read(client, &buffer, sizeof(buffer));
	//if(er < 0) printf("ERRO READ\n");
	//if(strncmp(buffer, "ok", 2) == 0)
	setShips(field, sub, 2, 2);

	/*
	// Set contratorpedeiros player one
	printOwnField(field, player, client);
	setShips(field, cont, 3, player, 3, client);

	// Set navios-tanque player one
	printOwnField(field, player, client);
	setShips(field, tanq, 4, player, 2, client);

	// Set porta-aviões player one
	printOwnField(field, player, client);
	setShips(field, port, 5, player, 1, client);
	*/

	//strcpy(string, "Por favor, digite uma posicao valida.\n");
	//er = write((client), string, strlen(string));
	//if(er < 0) printf("ERROR WRITE\n");

	return true;
}

int main(int argc, char *argv[])
{
	int sockfd, port, er, playerNumber;
	struct sockaddr_in serv_addr;
	struct hostent *server = NULL;
	char buffer[500], *hostname, input[10], msgCompare[40], msgCompare2[40], msgCompare3[40];
	bool gameOver = false;
	char **field, *s, ok[3];//, **fieldOpp;
	

	if(argc < 3){
		printf("usage %s: port hostname\n", argv[0]);
		return -1;
	}

	//Getting the arguments
	port = atoi(argv[1]);
	hostname = (char*)malloc(sizeof(argv[2])*sizeof(char));
	strcpy(hostname, argv[2]);

	//Starting the client
	startClient(&sockfd, server, &serv_addr, (unsigned short)port, hostname);

	//Reading connection confirmation message
	er = read(sockfd, ok, sizeof(buffer));
	if(strncmp(ok, "ok", 2) == 0) printf("Conectado!\n");

	printf("\nReading playerNumber\n");
	er = read(sockfd, buffer, 1);
	playerNumber = buffer[0] - '0';
	printf("Voce eh o jogador %d\n", playerNumber);


	field = getField();
	//fieldOpp = getField();
	resetField(field);

	positionsLoop(field);
	s = convert(field);

	er = write(sockfd, s, 100);
	if(er < 0){
			printf("ERROR READ\n");
			return -1;
	}

	printf("Esperando o outro jogador...\n");

	//Waiting for the server to confirm both fields are set
	bzero(ok, 3);
	er = read(sockfd, ok, 3);
	if(er < 0){
			printf("ERROR READ\n");
			return -1;
	}

	if(strncmp(ok, "ok", 2) == 0) printf("Game Starting...\n");

	while(gameOver == false){
		//Reading map or error instruction
		bzero(&buffer, sizeof(buffer));
		er = read(sockfd, &buffer, sizeof(buffer));
		if(er < 0){
			printf("ERROR READ\n");
			return -1;
		}
		//print do mapa
		printf("%s\n", buffer);
		printf("PRIMEIRO.\n");

		//
		if(buffer[0] == 'J' || buffer[0] == 'P'){
			scanf("%s", input);
			er = write((sockfd), input, strlen(input));
			if(er < 0) printf("ERROR WRITE\n");
		}
		else{
			strcpy(msgCompare, "Aguarde o adversario\n");
			// Player turn
			if(((strncmp(buffer, msgCompare, 10)) != 0) && ((strncmp(buffer, "Voce", 4)) != 0)){
				printf("Entrou no IF\n");
				//Reading instructions
				bzero(&buffer, sizeof(buffer));
				er = read(sockfd, &buffer, sizeof(buffer));
				if(er < 0){
					printf("ERROR READ\n");
					return -1;
				}
				printf("%s\n", buffer);
				printf("SEGUNDO.\n");

				strcpy(msgCompare, "Parabens, player 1!\n");
				strcpy(msgCompare2, "Parabens, player 2!\n");
				strcpy(msgCompare3, "Voce perdeu!\n");

				if(strcmp(buffer, msgCompare) == 0 || strcmp(buffer, msgCompare2) == 0 || 
					strcmp(buffer, msgCompare3) == 0)
					gameOver = true;
				else{
					scanf("%s", input);
					er = write((sockfd), input, strlen(input));
					if(er < 0) printf("ERROR WRITE\n");
				}
			}
		}
	}

	//printf("%s\n", buffer);

	return 0;
}