#ifndef __EXTREMITE__
#define __EXTREMITE__


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>

int ext_out(void);
int ext_in(int port, char* addrIp, int fd);
void echo(int f, char* hote, char* port);


#endif
