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



/*struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};*/


char archconfig[70]="tp5.conf";

void error404(int connfd) {
	write(connfd, "HTTP/1.0 404 Not Found\nContent-Length: 26\nContent-Type: text/html\n\n<h1>404 Not Found :'(</h1>\n\n", 101);
}

int obtener_puerto() {
	int confFileFd = open(archconfig, O_RDONLY);
	
	char buffer[1000];
	read(confFileFd, buffer, sizeof buffer);
    int x=0;
    char* p=strstr(buffer,"port=");
    p+=5;
    char por[6];
    int y =0;
	while(*p!='\n'){por[y]=*p; y++; p++;}
    por[y]='\0';
	return atoi(por);
}

void getdir (char* dir){

    int confFileFd = open(archconfig, O_RDONLY);
	char buffer[1000];
    //char dir[50];
	read(confFileFd, buffer, sizeof buffer);
    int x=0;
    while(buffer[x]!='='){ x++;}
    x++;int y=0;
    while(buffer[x]!='\n'){dir[y]=buffer[x]; y++; x++; }
    dir[y]='\0';
	return  0;

}

void servicio (int connfd){
    
    int leido;
    char dir[256];
    char buff[1000];
    leido = read(connfd,buff ,sizeof buff);
    char * puntero = strstr(buff,"GET ");
    puntero+=4;
    //dir[0]='.';  // Aca es . porque esta en la misma carpeta pero deberia ir la ruta que pide poner en el archivo de configuracion
    getdir(dir);    
    int c=strlen(dir);
    while(*puntero!=' '){dir[c]=*puntero; puntero++; c++;} // copia el nombre del archivo requerido

	write (1 ,buff ,leido);

    //int arch= open(dir, O_RDWR , 0666);  orginal
    int arch =open(dir,O_RDONLY);
    if(arch < 0) {
		error404(connfd);   //funcion que tira error por no encontrar archivo
		pthread_exit(NULL);
	}
    //if(arch<0){perror ("arch"); return 0;}  // original

    // TamanioArchivo
    struct stat estru;
    if((fstat(arch,&estru))<0){perror ("fstat"); return 0;};
    int tam=estru.st_size;
    // FinTamanioArchivo

    // CabeceraProtocolo
    char buff2[10000]="HTTP/1.1 200 OK\nContent-Length=";
    char taman[32];
    sprintf(taman, "%d", tam);   // Usa TamanioArchivo
    strcat(buff2,taman);
    strncat(buff2,"\nContent-type: ",15);
    int k=1; int g=0;
    char extension[6]; 
    while(dir[k]!='.'){k++;}
    k++;
    while(dir[k]!='\0'){extension[g]=dir[k]; k++; g++;}
    extension[g]='\0';

    if((strcmp(extension,"pdf"))==0){
        strcat(buff2,"application/pdf");
    }else if((strcmp(extension,"jpeg"))==0){
        strcat(buff2,"image/jpeg");}
        else if((strcmp(extension,"txt"))==0){
            strcat(buff2,"text/txt");}
            else if((strcmp(extension,"html"))==0){
                 strcat(buff2,"text/html");
            }
    
    strcat(buff2,"\n\n");
    //FinCabeceraProtocolo


    int largo=strlen(buff2);
    write(connfd,buff2,largo);
    char buff3[542750];
    while((leido=read(arch,buff3,tam))>0){
        write(connfd,buff3,leido);
    }
    write(connfd, "\n\n", 2);
    
    close(connfd);
    close(arch);
    pthread_exit(NULL);

    return 0;

}

int main(int argc, char * const argv[]){

	int fd,connfd,c;
    
    
	struct sockaddr_in procrem={};
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if  (fd < 0 ){
		perror ("socket");
		return -1;
	}


/* bucle para loopear por las opciones pasadas */
        while ((c=getopt(argc, argv, "f:"))>=0) {
                switch (c) {
                        
                        case 'f':
                                if((open(optarg,O_RDONLY))>=0){strcpy(archconfig,optarg);}

                                break;
                        
                      
                        default:
                                fprintf(stderr, "opcion invalida: %c\n", c);
                                        return 1;
        }
                }




	procrem.sin_family = AF_INET;
	procrem.sin_port = htons(obtener_puerto());  //poner funcion GetPuerto para obtener puerto del archivo de connfig
	procrem.sin_addr.s_addr = htonl(INADDR_ANY);
	int a;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {
		perror ("bind");
                return -1;
        }

	listen(fd,10);
    //NO IRIA AK
    
	while ((connfd = accept(fd,NULL, 0)) > 0 ){
		//crear un hilo
        pthread_t tid;
        pthread_create(&tid,NULL,servicio,(void *) connfd); //Definicion del hilo en internet https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
        
    }
    
    close(fd);
	return 0;
} 
