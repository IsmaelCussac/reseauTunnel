#include "extremite.h"
#include "iftun.h"

/* taille maximale des lignes */
#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"
#define MAX 1500

	/* echo des messages reÃ§us (le tout via le descripteur f) */

void echo(int f, char* hote, char* port)
{
	ssize_t lu; /* nb d'octets reÃ§us */
	char msg[MAXLIGNE+1]; /* tampons pour les communications */
	char tampon[MAXLIGNE+1];
	int pid = getpid(); /* pid du processus */
	int compteur=0;


	do { /* Faire echo et logguer */
		lu = recv(f,tampon,MAXLIGNE,0);
		if (lu > 0 )
		{
			compteur++;
			tampon[lu] = '\0';
			/* log */
			fprintf(stderr,"[%s:%s](%i): %3i :%s",hote,port,pid,compteur,tampon);
			snprintf(msg,MAXLIGNE,"> %s",tampon);
			/* echo vers la sortie standard */
			fprintf(stdout,"%s", msg);
		} 
		else {
			break;
		}
	} 
	while ( 1 );

	/* le correspondant a quittÃ© */
	send(f,CIAO,strlen(CIAO),0);
	close(f);
	fprintf(stderr,"[%s:%s](%i): Terminé.\n",hote,port,pid);
}

int ext_out (char* port){
	char msg[MAXLIGNE+1]; /* tampons pour les communications */
	int s,n; /* descripteurs de socket */
	int len,on; /* utilitaires divers */
	struct addrinfo * resol; /* rÃ©solution */
	struct addrinfo indic = {AI_PASSIVE, PF_INET6,SOCK_STREAM,0, 0,NULL,NULL,NULL};
	struct sockaddr_in6 client; /* adresse de socket du client */
	int err; /* code d'erreur */

	fprintf(stderr,"Ecoute sur le port %s\n",port);
	err = getaddrinfo(NULL,port,&indic,&resol);
	if (err<0){
		fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
		exit(2);
	}

	/* CrÃ©ation de la socket, de type TCP / IP */
	if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
		perror("allocation de socket");
		exit(3);
	}
	fprintf(stderr,"le nÂ° de la socket est : %i\n",s);

	/* On rend le port rÃ©utilisable rapidement /!\ */
	on = 1;
	if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
		perror("option socket");
		exit(4);
	}
	fprintf(stderr,"Option(s) OK!\n");

	/* Association de la socket s Ã l'adresse obtenue par rÃ©solution */
	if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
		perror("bind");
		exit(5);
	}
	freeaddrinfo(resol); /* /!\ LibÃ©ration mÃ©moire */
	fprintf(stderr,"bind!\n");

	/* la socket est prÃªte Ã recevoir */
	if (listen(s,SOMAXCONN)<0) {
		perror("listen");
		exit(6);
	}
	fprintf(stderr,"listen!\n");
	printf("sortie listen\n");
	while(1) {
		
		/* attendre et gÃ©rer indÃ©finiment les connexions entrantes */
		len=sizeof(struct sockaddr_in6);
		n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len);
		if( n< 0 ) {
			perror("accept");
			exit(7);
		}
		printf("ici\n");
		/* Nom rÃ©seau du client */
		char hotec[NI_MAXHOST]; 
		char portc[NI_MAXSERV];
		err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
		if (err < 0 ){
			fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
		}
		else{
			fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
		}
		/* traitement */
		tun_copy(n, 1, msg);
	//	echo(n,hotec,portc);
	
		
	}
	return EXIT_SUCCESS;
}





int ext_in(char* hote, char* port, int fdtun)
{
	
	char buf[MAX];
	char ip[INET6_ADDRSTRLEN]; /* adresse IPv6 en notation pointée */
	struct addrinfo *resol; /* struct pour la résolution de nom */
	int s, k; /* descripteur de socket */

	/* Résolution de l'hôte */
	if ( getaddrinfo(hote,port,NULL, &resol) < 0 ){
	perror("résolution adresse");
	exit(2);
	}


	/* On extrait l'addresse IP */
	inet_ntop(resol->ai_family, resol, ip, INET6_ADDRSTRLEN);
	
	/* Création de la socket, de type TCP / IP */
	/* On ne considère que la première adresse renvoyée par getaddrinfo */
	s = socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol);
	if (s < 0) {
	perror("allocation de socket");
	exit(3);
	}
	fprintf(stderr,"le n° de la socket est : %i\n",s);
	
	/* Connexion */
	fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s \n\n", hote,ip,port);
	
	int c = connect(s,resol->ai_addr,sizeof(struct sockaddr_in6));
	printf("%d\n",c);
	if (c < 0) {
	perror("connection");
	exit(4);
	}
	
	printf("etre sur\n");
	freeaddrinfo(resol); /* /!\ Libération mémoire */


	while (1) {
		k = tun_copy(fdtun, s, buf);
	}

	close(s);

	return 0;

}
