#ifndef __IFTUN__
#define __IFTUN__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <unistd.h>


int tun_alloc(char *dev);
int tun_copy (int src, int dest, char * buf);


#endif
