#include "iftun.h"


#define MAX 1500
int main (int argc, char **argv){

	char *port = "123";
	int out;
	char dev2[MAX];
	
	int src, dest;
	strcpy(dev2,"tun%d");
	src = tun_alloc(dev2);
	system("./configure-tun.sh");
	
	
	out = ext_out(port, src);

		
	
	return 0;
}
