#include "extremite.h"

/* taille maximale des lignes */
#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"


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

int ext_out (void){

	int s,n; /* descripteurs de socket */
	int len,on; /* utilitaires divers */
	struct addrinfo * resol; /* rÃ©solution */
	struct addrinfo indic = {AI_PASSIVE, PF_INET,SOCK_STREAM,0, 0,NULL,NULL,NULL};
	struct sockaddr_in client; /* adresse de socket du client */
	char * port; /* Port pour le service */
	int err; /* code d'erreur */

	port="123";
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
	if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
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

	while(1) {
		/* attendre et gÃ©rer indÃ©finiment les connexions entrantes */
		len=sizeof(struct sockaddr_in);
		if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
			perror("accept");
			exit(7);
		}
		/* Nom rÃ©seau du client */
		char hotec[NI_MAXHOST]; char portc[NI_MAXSERV];
		err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
		if (err < 0 ){
			fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
		}
		else{
			fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
		}
		/* traitement */
		echo(n,hotec,portc);
	}
}





int ext_in(int port, char* addrIp, int fd)
{
	char *buf = malloc(IFNAMSIZ*sizeof(char));
	struct sockaddr_in sin;
	int soc = socket(AF_INET, SOCK_STREAM, 0);
	if(soc == -1)
	{
		printf("error opening socket");
		return -1;
	}
	
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = 0;
	sin.sin_addr.s_addr = inet_addr(addrIp);
	sin.sin_family = AF_INET;
	
	if(bind(soc, (struct sockaddr *)&sin,sizeof(struct sockaddr_in) ) == -1)
	{
		printf("error binding socket");
		return -1;
	}
	
	read(fd, buf, IFNAMSIZ);
	
	int r = sendto (soc, buf, sizeof(buf), 0, (struct sockaddr *) sin.sin_addr.s_addr, sizeof(buf));
	if(r<0){
		perror("Envoie dans socket");
        exit(1);
	}
}
