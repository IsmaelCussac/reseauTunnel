#Projet Reseaux

CC = gcc
CFLAGS = -Wall -W -std=c99 -pedantic
OBJ = iftun.o extremite.o


all: test_iftun1 test_iftun0 test_iftun_serveur

extremite.o : extremite.h extremite.c 
	$(CC) $(CCFLAGS) -c extremite.c 

iftun.o : iftun.h iftun.c 
	$(CC) $(CCFLAGS) -c iftun.c 
	
test_iftun0 :  test_iftun0.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_iftun0 test_iftun0.c $(OBJ)

test_iftun1 :  test_iftun1.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_iftun1 test_iftun1.c $(OBJ)
	
test_iftun_serveur :  test_iftun_serveur.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_iftun_serveur test_iftun_serveur.c $(OBJ)


clean:
	rm -f *.o core

