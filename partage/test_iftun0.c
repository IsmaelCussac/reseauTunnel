#include "iftun.h"

int main (void){

	char *dev2 = malloc(1024*sizeof(char));
		char *buf = malloc(IFNAMSIZ*sizeof(char));
	int src, dest;
	strcpy(dev2,"tun%d");
	src = tun_alloc(dev2);
	

	
	
	while(1){
		tun_copy(src, dest, buf);
	}
	return 0;
}
