#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>





int conectarse(char* ip,struct sockaddr_in procrem, int fd){
int conectado;
struct hostent *hp = gethostbyname(ip);
inet_pton(AF_INET,inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0])), &procrem.sin_addr);
	conectado = connect(fd,(struct sockaddr *)&procrem, sizeof procrem);  
	if  (conectado < 0 ){
                perror ("connect");
                return -1;
        }
return 0;
}



int bind_socket(int a,struct sockaddr_in procrem, int fd){
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {  // avisa al sistema que se creo un socket
		perror ("bind");
                return -1;
        }
return 0;
}
