#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>


/*struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};*/

int leido,connfd;
char buff[1000];

void *servicio (){

    leido = read (connfd,buff ,sizeof buff);
    char * puntero = strstr(buff,"GET ");
    char dir[256];
    char buff2[1000]="HTTP/1.1 200 Ok\nContent-Length=";
    puntero+=4;
    int c=0;
    while(*puntero!=' '){dir[c]=*puntero; puntero++; c++;}

	write (1 ,buff ,leido);

    int arch= open(dir, O_RDWR , 0666);
    write(1,dir,strlen(dir));
    struct stat estru;
    if((fstat(arch,&estru))<0){perror ("fstat"); return 0;};
    int tam=estru.st_size;



char taman[256];
sprintf(taman, "%d", tam);
strcat(buff2,taman);

strncat(buff2,"\nContent-type=",14);

char extension[256]="text/html"; //arreglar
strcat(buff2,extension);
strcat(buff2,"\n\n");
strcat(buff2,dir);
strcat(buff2,"\n");  
write (1,buff2 ,strlen(buff2)); 
write (connfd ,buff2 ,strlen(buff2));
close(connfd);
    return 0;
//fstat para devolver el size st_size
}

int main(){
	int fd;
	
	struct sockaddr_in procrem={};
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if  (fd < 0 ){
		perror ("socket");
		return -1;
	}

	procrem.sin_family = AF_INET;
	procrem.sin_port = htons(80);
	procrem.sin_addr.s_addr = htonl(INADDR_ANY);
	int a;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(fd,(struct sockaddr *)&procrem, sizeof procrem) < 0 ) {
		perror ("bind");
                return -1;
        }

	listen(fd, 5);
    //NO IRIA AK
                pthread_t tid1,tid2;
	while ( (connfd = accept(fd,NULL, 0)) > 0 ){
		//crear un hilo
		
        if(pthread_create(&tid1,NULL,servicio,NULL)==0){ printf("hilo creado \n");}
        //servicio();
	
	}
		close(connfd);
	
	return 0;
} 
