#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

#define MAXLIGNE 1024

int tun_alloc(char *dev){
      
	struct ifreq ifr;
	
	int fd ;
	int err;
  
   if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
  		perror("création tun");
      exit(1);
   }
  
   memset(&ifr, 0, sizeof(ifr));
  
   /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
    *        IFF_TAP   - TAP device  
    *
    *        IFF_NO_PI - Do not provide packet information  
    */ 
    
    ifr.ifr_flags = IFF_TUN; 
    
    if( *dev )
    	strncpy(ifr.ifr_name, dev, IFNAMSIZ);
  
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        close(fd);
        return err;
     }
        
     strcpy(dev, ifr.ifr_name);
     return fd;
 } 
 

int alloc_sock_ipv6(int argc, char *argv[], int tun){

  char * hote; /* nom d'hôte du  serveur */   
  char * port; /* port TCP du serveur */   

  char ip[NI_MAXHOST]; /* adresse IPv4 en notation pointée */
  struct addrinfo *resol; /* struct pour la résolution de nom */
  int s; /* descripteur de socket */
  
 
  /* Traitement des arguments */
  if (argc!=3) {/* erreur de syntaxe */
    printf("Usage: %s hote port\n",argv[0]);
    exit(1);
  }
  hote=argv[1]; /* nom d'hôte du  serveur */   
  port=argv[2]; /* port TCP du serveur */   

  /* Résolution de l'hôte */
  if ( getaddrinfo(hote,port,NULL, &resol) < 0 ){
    perror("résolution adresse");
    exit(2);
  }

	
  
  /* On extrait l'addresse IP */
  
  
  inet_ntop(resol->ai_family,resol,ip, INET6_ADDRSTRLEN);
  

  /* Création de la socket, de type TCP / IP */
  /* On ne considère que la première adresse renvoyée par getaddrinfo */
  if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  

  /* Connexion */
  fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
	  hote,ip,port);
  if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("connexion");
    exit(4);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */

  /* Session */
  char tampon[MAXLIGNE + 3]; /* tampons pour les communications */
  ssize_t lu=0;
  int fini=0;
  
  while( 1 ) { 
    /* Jusqu'à fermeture de la socket (ou de stdin)     */
    /* recopier à l'écran ce qui est lu dans la socket  */
    /* recopier dans la socket ce qui est lu dans stdin */

    /* réception des données */
    lu = read(tun,tampon,MAXLIGNE); /* bloquant */
    if (lu == 0 ) {
      fprintf(stderr,"Connexion terminée par l'hôte distant\n");
      break; /* On sort de la boucle infinie */
    }
    tampon[lu] = '\0';
    write(1,tampon,lu);
    //printf("reçu: %s",tampon);
    if ( fini == 1 )
      break;  /* on sort de la boucle infinie*/
  	 send(s,tampon,lu,0);
  } 
  /* Destruction de la socket */
  close(s);

  fprintf(stderr,"Fin de la session.\n");
  return EXIT_SUCCESS;


}

int extremite_in(int argc, char *argv[], int fd){
 printf("demarrage extrimite out ....");
 	alloc_sock_ipv6(argc, argv,fd);
 	
 	return 0;
}


/* echo des messages reçus (le tout via le descripteur f) */
void echo(int f, char* hote, char* port, int fd){
  ssize_t lu; /* nb d'octets reçus */
  char msg[MAXLIGNE+1]; /* tampons pour les communications */ 
  char tampon[MAXLIGNE+1]; 
  int pid = getpid(); /* pid du processus */
  int compteur=0;
  
 
  
  /* message d'accueil */
  snprintf(msg,MAXLIGNE,"Bonjour %s! (vous utilisez le port %s)\n",hote,port);
  /* envoi du message d'accueil */
  send(f,msg,strlen(msg),0);
  
  do { /* Faire echo et logguer */
    lu = recv(f,tampon,MAXLIGNE,0);
    if (lu > 0 )
      {
        compteur++;
        tampon[lu] = '\0';
        /* log */
        write(1,tampon,lu);
        write(fd,tampon,lu);
        snprintf(msg,MAXLIGNE,"> %s",tampon);
      } else {
        break;
      }
  } while ( 1 );
       
  /* le correspondant a quitté */
  close(f);
  fprintf(stderr,"[%s:%s](%i): Terminé.\n",hote,port,pid);
}


int extremite_out(int argc, char *argv[], int fd){

printf("demarrage extrimite out ....");
int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
 
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in6 client; /* adresse de socket du client */
  char * port; /* Port pour le service */
  int err; /* code d'erreur */
  
  /* Traitement des arguments */
  if (argc!= 3) { /* erreur de syntaxe */
    printf("Usage: %s hote port\n",argv[0]);
    exit(1);
  }
  
  port= "1234"; 
  fprintf(stderr,"Ecoute sur le port - %s\n", port);
  err = getaddrinfo(NULL,port,&indic,&resol); 
  if (err<0){
    fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
    exit(2);
  }

  /* Création de la socket, de type TCP / IP */
  if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* On rend le port réutilisable rapidement /!\ */
  on = 1;
  if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
    perror("option socket");
    exit(4);
  }
  fprintf(stderr,"Option(s) OK!\n");

  /* Association de la socket s à l'adresse obtenue par résolution */
  if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("bind");
    exit(5);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  fprintf(stderr,"bind!\n");

  /* la socket est prête à recevoir */
  if (listen(s,SOMAXCONN)<0) {
    perror("listen");
    exit(6);
  }
  fprintf(stderr,"listen!\n");

  while(1) {
    /* attendre et gérer indéfiniment les connexions entrantes */
    len=sizeof(struct sockaddr_in6);
    if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
      perror("accept");
      exit(7);
    }
    /* Nom réseau du client */
    char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
    err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
    if (err < 0 ){
      fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
    }else{ 
      fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
    }
    /* traitement */
    
    echo(n,hotec,portc, fd);
  }
  return EXIT_SUCCESS;

}

int lire_fichier(){

 }

int main(int argc, char *argv[]){
   int fd ;
	char tun[1024];
	
	sprintf(tun,"tun0");
	fd= tun_alloc(tun);
	printf("tun0: %s", tun);


   pid_t pid = fork();
 
   if (pid == -1) {
      perror("fork failed");
      exit(EXIT_FAILURE);
   }
   else if (pid == 0) {
       extremite_out(argc, argv, fd);
     
   }
   else {
      extremite_in(argc, argv, fd);
   }


	
	
 
 	
	return 0;
}

