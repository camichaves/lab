 #include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "headers.h"

/*struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};*/



//DEFINIR FUNCIONES DE HEADERS


int def_socket();
void def_puerto(int puerto, struct sockaddr_in procrem);



int main(int argc, char * const argv[]){





	int fd, leido, conectado;
	char buff[1000];
    char dir[256];
    char palabra[256];
    int puerto=-1;
    char ip[16];
    char c;
	struct sockaddr_in procrem={};
    fd=def_socket();
    if(fd<0){return fd;}
    //if(fd==-1){return -1;};
/*	fd = socket(AF_INET, SOCK_STREAM, 0);  // definicion socket
	if  (fd < 0 ){
		perror ("socket");
		return -1;
	}
*/

/* bucle para loopear por las opciones pasadas */
        while ((c=getopt(argc, argv, "d:w:i:p:"))>=0) {
                switch (c) {
                        case 'd':
                                strcpy(dir,optarg);
                                
                                
                                
                                break;

                        case 'w':
                                strcpy(palabra,optarg);

                                break;

                        case 'i':
                                strcpy(ip,optarg);

                                break;
                        
                        case 'p':
                                puerto=atoi(optarg);

                                break;
                        default:
                                fprintf(stderr, "opcion invalida: %c\n", c);
                                        return 1;
        }
                }


	procrem.sin_family = AF_INET; 
	procrem.sin_port = htons(puerto);
	

//def_puerto(puerto, procrem);
        struct hostent *hp = gethostbyname(ip);
	
	inet_pton(AF_INET,inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0])), &procrem.sin_addr);
	conectado = connect(fd,(struct sockaddr *)&procrem, sizeof procrem);  
	if  (conectado < 0 ){
                perror ("connect");
                return -1;
        }


    char cadena_protocolo[256];
strncpy(cadena_protocolo,dir,strlen(dir));
strcat(cadena_protocolo,"#");
strncat(cadena_protocolo,palabra,strlen(palabra));
strcat(cadena_protocolo,"#");



	write(fd, cadena_protocolo,strlen(cadena_protocolo));
	while ((leido = read(fd, buff , sizeof buff)) > 0){
		if  (leido < 0 ){
			perror ("read");
			return -1;
		}
	write(1,buff,leido);
	
	}

	return 0;
} 
