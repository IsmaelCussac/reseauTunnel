#include "iftun.h"


#define MAX 1500
int main (int argc, char **argv){

	char *port = "1234";
	char *hote= "fc00:1234:1::26";
	char dev2[MAX];
	char buf[MAX];
	int src, dest;
	strcpy(dev2,"tun%d");
	src = tun_alloc(dev2);
	
	system("./configure-tun.sh");
	
	int in;
	
	in = ext_in(hote, port, src);
	
	return 0;
}
