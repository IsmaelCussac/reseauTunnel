#Projet Reseaux

CC = gcc
CFLAGS = -Wall -W -std=c99 -pedantic
OBJ = iftun.o extremite.o


all: tunnel46d

extremite.o : extremite.h extremite.c 
	$(CC) $(CCFLAGS) -c extremite.c 

iftun.o : iftun.h iftun.c 
	$(CC) $(CCFLAGS) -c iftun.c 

tunnel46d :  tunnel46d.c $(OBJ)
	$(CC) $(CCFLAGS) -o tunnel46d tunnel46d.c $(OBJ)


clean:
	rm -f *.o core

