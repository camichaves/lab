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

int main(){
char* buff="Hola mi nombre es Camila \n Soy de San JUan \n Gane la hackathon de la NASA \n me voy de viaje a europa con mi novio \n vivo con una amiga en un depto en mendoza \n tengo un novio que amo \n me va bien en la facultad";
char buff2[256]="";
char* pal_reservada="novio";
char * puntero=strstr(buff,pal_reservada);
char* puntero_aux=puntero;
while(*puntero!='\n'){puntero--;}
puntero++;
while(*puntero_aux!='\n'){puntero_aux++;}

strncat(buff2,puntero,(puntero_aux-puntero));

printf("MI linea es: %s ", buff2);

return 0;
}
