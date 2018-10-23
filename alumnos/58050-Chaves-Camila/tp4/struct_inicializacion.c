#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>




int def_socket(){

    int fd;
	fd = socket(AF_INET, SOCK_STREAM, 0);  // definicion socket
	if  (fd < 0 ){
		perror ("socket");
		return -1;
	}else{ return fd;}
}

void def_puerto(int puerto, struct sockaddr_in procrem){
//procrem.sin_family = AF_INET; 
procrem.sin_port = htons(puerto);
}




void eliminar_hijos_zombies(){signal(SIGCHLD,SIG_IGN );} //para sacar procesos terminados de la PCB ...}
