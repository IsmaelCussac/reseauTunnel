#include "iftun.h"

#define PORT 123

int main (int argc, char **argv){


	char *dev2 = malloc(1024*sizeof(char));
	int src, dest;
	strcpy(dev2,"tun%d");
	src = tun_alloc(dev2);
	
	system("./configure-tun.sh");
	
	//int out = ext_out();
	//int in = ext_in(PORT, "172.16.1.10", src);
	
	
	while(1){
		tun_copy(src, 1);
	}
	
	return 0;
}
