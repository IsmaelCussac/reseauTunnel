#include "iftun.h"

/* Auteurs
	Dalal ZOUAI
	Claire GERARD
	Sébastien GOUVERNEUR
	Ismaël CUSSAC
*/

int main (int argc, char **argv){

	if(argc != 3){
		printf("Usage: %s port ipServeur\n",argv[0]);
		exit(1);
	}

	char *port = argv[1];
	char *ipServ = argv[2];
	char dev2[MAX];
	char buf[MAX];
	int fdTun;
	strcpy(dev2,"tun%d");
	fdTun = tun_alloc(dev2);
	
	system("./configure-tun.sh");
		
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
		out = out = ext_out(port, fdTun);
	return 0;
}
