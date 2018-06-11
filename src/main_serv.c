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
#define TOTAL 30

/*
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
		strcpy(string, "Digite as coordenadas da posicao a ser atacada: \0");
		er = write((client), string, strlen(string));
		if(er < 0) printf("ERROR WRITE\n");
		//printf("Digite as coordenadas da posicao a ser atacada: ");

		//Reading ATTACK
		er = read(client, &input, sizeof(input));
		if(er < 0){
			printf("ERROR READ\n");
			return -1;
		}

		line = input[0] - '0';
		c = input[1];
		//scanf("%d %c", &line, &c);
		col = getColumnIndex(c);

		// Campo: O => campo livre		N => navios 	X => pedaço navio afundado	Y => mar atacado
		if(line >= 0 && line <= 9 && col >= 0 && col <= 9){		// If it's a valid position
			if(opponentField[line][col] != 'X' && opponentField[line][col] != 'Y'){	// If it wasn't already attacked
				if(opponentField[line][col] == 'N') {
					opponentField[line][col] = 'X';
					strcpy(string, "Voce acertou um navio caralho!!!!\n");
					er = write((client), string, strlen(string));
				}
				else {
					opponentField[line][col] = 'Y';
					strcpy(string, "Voce errou otario!!!\n");
					er = write((client), string, strlen(string));
				}
				//printOpponentField(opponentField, opponentPlayer, client);
				return true;
			}
			else{
				strcpy(string, "Ja foi atacada esta posicao. Tente novamente.\n");
				er = write((client), string, strlen(string));
				if(er < 0) printf("ERROR WRITE\n");
			}
		} else {
			strcpy(string, "Por favor, digite uma posicao valida.\n");
			er = write((client), string, strlen(string));
		}
		if(er < 0) printf("ERROR WRITE\n");
	}
	return false;
}
*/
//setShips(char **field, char ship[20], int size, int player, int rep)


// Get an empty field
char **getField(){
	int i, j;

	char **field = (char**)malloc(sizeof(char*) * 10);

	for(i = 0; i < 10; i++)
		field[i] = (char*)malloc(sizeof(char) * 10);

	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++)
			field[i][j] = 'O';
	}

	return field;
}

void convertBack(char *s, char **f){
	for(int i=0;i<10; i++){
		for(int j=0; j<10; j++){
			f[i][j] = s[(10*i) + j];			
		}
	}
}

void printF(char **f){
	printf("  A B C D E F G H I J\n");
	for(int i=0; i<10; i++){
		for (int j=0; j<10; j++){
			if(j==0) printf("%d ", i);
			printf("%c ", f[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[]){

	int sockfd, cli1_sockfd, cli2_sockfd;
	struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
	socklen_t clilen1, clilen2;
	int port;
	char **field1, **field2, *s = (char*)malloc(100*sizeof(char));

	int er;

	//int pointsOne = 0, pointsTwo = 0, turn = 1;
	//bool playerOneReady = false, playerTwoReady = false;
	//char string[30];

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
	acceptClient(&cli_addr1, &clilen1, sockfd, &cli1_sockfd, "ok");
	//Telling the first client he is player 1

	acceptClient(&cli_addr2, &clilen2, sockfd, &cli2_sockfd, "ok");
	//Telling the second client he is player 2
	

	er = write(cli1_sockfd, "1", 1);
	if(er<0) printf("ERRO WRITE\n");
	er = write(cli2_sockfd, "2", 1);
	if(er<0) printf("ERRO WRITE\n");

	field1 = getField();
	field2 = getField();

	er = read(cli1_sockfd, s, 100);
	if(er < 0) printf("ERROR READFIELD\n");
	convertBack(s, field1);

	er = read(cli2_sockfd, s, 100);
	if(er < 0) printf("ERROR READFIELD\n");
	convertBack(s, field2);
	//printf("%c\n", field1[0][0]);

	er = write(cli1_sockfd, "ok\0", 3);
	er = write(cli2_sockfd, "ok\0", 3);



/*
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
	while(pointsOne < 2 && pointsTwo < 2){
		strcpy(string, "Aguarde o adversario\n");
		if(turn == 1){
			// Prints wait
			er = write((cli2_sockfd), string, strlen(string));
			if(er < 0) printf("ERROR WRITE\n");

			if(mainLoop(fieldTwo, 1, cli1_sockfd) == true) {
				pointsOne++;
			}
			turn = 2;
		}
		else{
			// Prints wait
			er = write((cli1_sockfd), string, strlen(string));
			if(er < 0) printf("ERROR WRITE\n");

			//printOwnField(fieldTwo, 2, cli2_sockfd);

			if(mainLoop(fieldOne, 2, cli2_sockfd) == true)
				pointsTwo++;
			turn = 1;
		}
	}

	// End game
	if(pointsOne == 2){
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
*/
	return 0;
}
