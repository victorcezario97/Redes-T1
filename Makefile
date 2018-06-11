all:
	gcc -o main_serv src/main_serv.c src/server_client.c -I./include -Wall
	gcc -o main_cli src/main_client.c src/server_client.c -I./include -Wall
serv:
	./main_serv 51717
cli:
	./main_cli 51717 `hostname`
clean:
	rm -rf main_cli main_serv
serv2:
	./main_serv 8080
cli2:
	./main_cli 8080 `hostname`
