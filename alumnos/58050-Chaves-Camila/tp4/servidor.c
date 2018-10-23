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
#include "headers.h"
#include <stdlib.h>


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


void eliminar_hijos_zombies();
int conectarse(char* ip,struct sockaddr_in procrem, int fd);
int bind_socket(int a,struct sockaddr_in procrem, int fd);
int def_socket();
void def_puerto(int puerto, struct sockaddr_in procrem);

int main(int argc, char * const argv[]){



	int fd, fdcliente, leido, conectado, connfd, pid;
	char buff[5000];
    //char buff2[2000];
    char c;
    int puerto=5000; //por defecto
	struct sockaddr_in procrem={};

fd=def_socket();
if(fd<0){return fd;}

/* bucle para loopear por las opciones pasadas */
        while ((c=getopt(argc, argv, "p:"))>=0) {
                switch (c) {
                        
                        case 'p':
                                puerto=atoi(optarg);
                                

                                break;
                        default:
                                fprintf(stderr, "opcion invalida: %c\n", c);
                                        return 1;
        }
                }





	procrem.sin_family = AF_INET;   //Familia TCP/IP
    procrem.sin_port = htons(puerto);  




	procrem.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"0.0.0.0", &procrem.sin_addr);
	int a;

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);

if((bind_socket(a,procrem,fd))<0){return -1;}

	listen(fd, 5);
	
    eliminar_hijos_zombies();
	while ( (connfd = accept(fd,NULL, 0)) > 0 ){
		pid = fork();
		//hijo
		if ( pid == 0 ) {
			leido = read (connfd,buff ,sizeof buff);
			write (1 ,buff ,leido); 
			//write (connfd ,buff ,leido);
			// se conecta como cliente a un servidor http

	struct sockaddr_in serverhttp={};
	fdcliente = socket(AF_INET, SOCK_STREAM, 0);
	if  (fdcliente < 0 ){
		perror ("socketcliente");
		return -1;
	}
	serverhttp.sin_family = AF_INET;
	serverhttp.sin_port = htons(80); //puerto default servidor http



        char url[2][256];
        char pal_reservada[256]="";
        int x=0;
        while(buff[x]!='/'){ url[0][x]=buff[x];  x++;}
        url[0][x]='\0';
        printf("este es url[0]= %s \n",url[0]);
        int z=0;
        while(buff[x]!='#'){ url[1][z]=buff[x]; z++; x++;}
        url[1][z]='\0';
        printf("este es url[1]= %s \n",url[1]);
        int c=0; x++;
        while(buff[x]!='#'){pal_reservada[c]=buff[x]; c++; x++;}
        printf("este es palabra=-%s-\n",pal_reservada);



	
        struct hostent *hp = gethostbyname(url[0]);
	if(hp==NULL){ perror ("hethostbyname"); return -1;}
	
	inet_pton(AF_INET,inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0])), &serverhttp.sin_addr);
	conectado = connect(fdcliente,(struct sockaddr *)&serverhttp, sizeof serverhttp);
	if  (conectado < 0 ){
                perror ("connect");
                return -1;
        }



char cadena_protocolo[256];
    
    strcpy(cadena_protocolo,"GET ");
    strcat(cadena_protocolo,url[1]);
    strcat(cadena_protocolo," HTTP/1.1\nHost: ");
    strcat(cadena_protocolo,url[0]);
    strcat(cadena_protocolo,"\nConnection: close\n\n");
    //write (1 ,cadena_protocolo,69);

	char buff2[5000]="";
	write(fdcliente, cadena_protocolo, sizeof cadena_protocolo);
	while ((leido = read(fdcliente, buff , sizeof buff)) > 0){
		if  (leido < 0 ){
			perror ("read");
			return -1;
		}
    //buscar pal reservada;
   
    //write(1,buff,leido);
    char * puntero=strstr(buff,pal_reservada);
    if(puntero!=NULL){
        
        char* puntero_aux=puntero;
        while(*puntero!='\n'){puntero--; }
        puntero++;
        while(*puntero_aux!='\n'){puntero_aux++;}
        strncat(buff2,puntero,(puntero_aux-puntero));
        
        break;
    }
	
	
	}
    write(connfd,buff2,56);

		return 0;
		}
		close(connfd);
	}
	return 0;
} 
