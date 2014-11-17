#include "iftun.h"

/* Auteurs
	Dalal ZOUAI
	Claire GERARD
	Sébastien GOUVERNEUR
	Ismaël CUSSAC
*/
/*
./tunnel46d 123 fc00:1234:2::36 172.16.1.1 1 | hexdump -C
./tunnel46d 123 fc00:1234:1::16 172.16.1.2 2 | hexdump -C
*/

int main (int argc, char **argv){

	if(argc != 5){
		printf("Usage: %s port ipServeur ipTun machine\n", argv[0]);
		exit(1);
	}

	char *port = argv[1];
	char *ipServ = argv[2];
	char *ipTun = argv[3];
	char *machine = argv[4];
	char command[100];
	char dev2[MAX];
	char buf[MAX];
	int fdTun;
	strcpy(dev2,"tun%d");
	fdTun = tun_alloc(dev2);
	
	if(strcmp(machine,"1")){
		strcpy(command, "./configure-tun.sh ");
		strcat(command, ipTun);
	}
	else{
		strcpy(command, "./configure-tun1.sh ");
		strcat(command, ipTun);
	}
	
	system(command);
		
	int in, out;
	
	int f = fork();
	if(f < 0){
		perror("Fork\n");
		exit(1);
	}
	else if(f == 0){
		sleep(5);
		in = ext_in(ipServ, port, fdTun);
		}
	else
		out = ext_out(port, fdTun);
	return 0;
}
