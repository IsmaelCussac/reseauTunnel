#include "iftun.h"


#define MAX 1500
int main (int argc, char **argv){

	char *port = "123";
	char *hote = "fc00:1234:1::16";
	char dev2[MAX];
	char buf[MAX];
	int src, dest;
	strcpy(dev2,"tun%d");
	src = tun_alloc(dev2);
	
		system("./configure-tun2.sh");
		
	int in, out;
	
	int f = fork();
	if(f < 0){
		perror("Fork\n");
		exit(1);
	}
	else if(f == 0){
		sleep(5);
		in = ext_in(hote, port, src);
		}
	else
		out = out = ext_out(port, src);
	return 0;
}
