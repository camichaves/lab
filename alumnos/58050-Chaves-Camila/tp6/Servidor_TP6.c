#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>

//   /user/nro


int tamano_ar(int fd){
	char buffer[50];
	int tamano = 0;
	int leido;
	while((leido = read(fd, buffer, 50)) > 0)
	{
		tamano = tamano + leido;
	}
	lseek(fd, 0, SEEK_SET);
	return tamano;
}

int main(){

	
  
	int fd, fdcliente, leido, conectado, connfd, pid;
	char buff[5000];
    //char buff2[2000];
    char c;
    int puerto=8000; //por defecto
    
    
    
    
	struct sockaddr_in procrem={};

fd=socket(AF_INET, SOCK_STREAM, 0);
if(fd<0){return fd;}

    
	procrem.sin_family = AF_INET;   //Familia TCP/IP
    procrem.sin_port = htons(puerto);  




	procrem.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"0.0.0.0", &procrem.sin_addr);
	int a=1;

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);

if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {
		perror ("bind");
                return -1;
        }

	listen(fd, 5);
	
    
	while ( (connfd = accept(fd,NULL, 0)) > 0 ){
		pid = fork();
		//hijo
		if ( pid == 0 ) {
		  
		  
int arch=open("/home/guille/Documentos/cami/plantillas/TP6/usuario.txt",O_RDONLY);
	
		  
 			
		  //desarmar el link que me pasa el cliente
		  
		  int leido;
		char dir[256];
		char buff[1000];
		char buff2[tamano_ar(arch)];
		char rta[1000]="";
		char rta2[1000]="";
		char prot[1000];
		char dd[10];
		leido = read(connfd,buff ,sizeof buff);
		 char * lin=strtok(buff,"/");
		 lin=strtok(NULL,"/");
		
        if(strcmp(lin,"user")==0){ //verificamos que el LINK este correcto
		
		 lin=strtok(NULL,"/");
		//printf("Esto es lin: %s",lin);
		char * campo=strtok(NULL,"/");
		//printf("Esto es campo: %s \n",campo);
		char * campo2=strtok(campo,"");
		printf("Esto es campo2 :%s \n",campo2);
		 char * linea=strtok(lin," ");
		 char palabra[50];
		char palaux[50]="|";
		strcat(palaux,linea);
		
		
		 strcpy(palabra,palaux);
		 strcat(palabra,"|");
		

//tengo que hacer un if de si sigue otro campo


		 //linea es el comando, lo que hay que buscar
		 //printf("%s",linea);
		 read(arch, buff2, sizeof buff2); //recorro arch
		 int n; int g=0;char * dato;
		 
		 //funcionaba
		 
		 char * puntero=strstr(buff2,palabra);
	    if(puntero!=NULL){
        
		
        char* puntero_aux=puntero;
        //while(*puntero!=':'){puntero++; }
       // puntero++;
        while(*puntero_aux!='\n'){puntero_aux++;}
        if((strstr(campo2,"HTTP")==NULL)){
	strncat(rta,puntero,(puntero_aux-puntero)); }else{
	strncat(rta2,puntero,(puntero_aux-puntero));
	//printf("%s",rta);
	

	char * columna=strtok(campo2," ");
	
	if(strcmp(columna,"nombre")==0){ n=1;}
	if(strcmp(columna,"mail")==0){ n=2;}
 	if(strcmp(columna,"tel")==0){ n=3;}
	if(strcmp(columna,"ciudad")==0){ n=4;}
	if(strcmp(columna,"cp")==0){ n=5;}
		
		dato =strtok(rta2,"|");
	while(g<n){ dato =strtok(NULL,"|"); g++;}
	
	strcpy(rta,dato);
	
	if(n==0){strcpy(rta,"");} //ninguna de las opc
					
	

	}//fin else

	
	strcpy(prot,"HTTP/1.1 200 OK\nContent-Length: ");
	int tam=strlen(rta);
	char tamm[50];
	sprintf(tamm,"%d",tam);
	strcat(prot,tamm);
	 strncat(prot,"\nContent-type: ",15);
	 strcat(prot,"text/html\n\n");
	 //printf("%s",prot);
	 strcpy(dd,"\n\n");
			  write(connfd,prot, strlen(prot));
			  write(connfd,rta,strlen(rta));
			  write(connfd,dd,strlen(dd) );
			  
			  
        
	    
      }else{
	strcpy(prot,"HTTP/1.0 404 Not Found\nContent-Length: 26\nContent-Type: text/html\n\n<h1>404 Not Found :'(</h1>\n\n");
	write(connfd,prot, strlen(prot));
	
      }
      
}else{ // si el link esta incorrecto:
strcpy(prot,"HTTP/1.0 404 Not Found\nContent-Length: 26\nContent-Type: text/html\n\n<h1>404 Not Found :'(</h1>\n\n");
	write(connfd,prot, strlen(prot));
}
		  close(connfd);
		  close(arch);
		  return 0;
		  
		  
		  }
		  }
		  
		  
    
    close(connfd);
   
	return 0;
} 
