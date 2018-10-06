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

#include <sys/mman.h>
#include <semaphore.h>


#include "headers.h"


extern int Contador[21];
extern const char* PalabrasReservadas[3]; //cuantas??
char buff[1500];
char buffauxiliar[1500];



int buscar(char* palab);
void definir_pal_reservadas();
void cerearcontadOr();


//Funcion Imprimir Palabra Reservada
int Resto(char* palabra, int indpal, int tam2){


        int tam=strlen(palabra);
        int ban=0;

        int i=0,j=0, aux;

        while(ban==0 && i<tam2){
                if(buffauxiliar[i]==palabra[j]){
                        aux=i;
                        while(buffauxiliar[aux]==palabra[j] && j<tam){
                                j++;
                                aux++;
                        }
                        if(tam==j) ban=1; else{ i++; j=0; }
                }
                else i++;


 }
       for(int g=0; g<i; g++){

                printf("%c",buffauxiliar[g]);
        }

        char buffaux[1500];
        strncpy(buffaux,buffauxiliar,sizeof(buffauxiliar));

        memset(buffauxiliar,'\0',tam2);
        for(int g=aux;g<tam2;g++){
                buffauxiliar[g-aux]=buffaux[g];
        }
        int rta=(tam2-aux);

        printf("%s",PalabrasReservadas[indpal]);

        return rta;
}

//Funcion hijo 1 contador
int Hijo1(int *leido, char *ptr , sem_t *sem1, sem_t *sem1p){

        
  int tamanio;
        char * lineas[256];
        char d2[]="|123456789 - 0'¿°!«#$%&/()=—?¡[]_:;,.-{+}´ \n";

        int j;
        cerearcontador();        
        while(*leido>0){
                for(int i= 0; i<*leido;i++){
		        buff[i]=ptr[i];
	        }
                j=0;
                lineas[j]=strtok(buff,d2);
                while(lineas[j]!= NULL){
                        tamanio= strlen(lineas[j]);
                        Contador[tamanio-1]++;
                        j++;
                        lineas[j]=strtok(NULL,d2);

                }
                sem_post(sem1p);
                sem_wait(sem1);
        }
        // Contadores listos
        for(int g=0;g<21;g++){
        printf("\n Cantidad de palabras de %d caracteres:%d",g+1,Contador[g]);
            }
        printf("\n");

        sem_post(sem1p);
      

        return 0;
}



//Funcion Hijo 2 conversor a mayus
int Hijo2(int leido, char *buff){

        definir_pal_reservadas();

        char d2[]="|123456789 - 0'¿°!«#$%&/()=—?¡[]_:;,.-{+}´ \n\t";
        char * lineas[256];
        int j;
        int indpal;
        int h=0;
        int tambuff;
     
               
        tambuff=leido;
        strncpy(buffauxiliar,buff,leido);
        j=0;
        lineas[j]=strtok(buff,d2);
        while(lineas[j]!=NULL){
                indpal=buscar(lineas[j]);
                h++;
                if(indpal!=-1){
                        tambuff=Resto(lineas[j],indpal,tambuff);//imprime all hasta pal
                        j++;
                }else {
                        j++;
                }

                lineas[j]=strtok(NULL,d2);
        }

                printf("%s",buffauxiliar);

                memset(buffauxiliar,'\0',tambuff);
        

       
        return 0;

}





int main(int argc, char * const argv[]){

        int flagi=0;
        char buffer[2200];

        char* opcion_i = NULL;
        int c;
        char *ptr;
        ptr = mmap(NULL,(4 * sizeof(sem_t))+ sizeof (int),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1, 0);
        int *leido;
        //leido = mmap(NULL,10,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1, 0);
        sem_t *sem1, *sem1p, *sem2, *sem2p;
       
	sem1 = (sem_t *) ptr + sizeof sem1;
	sem1p = sem1 + sizeof sem1p;
	sem2= sem1p + sizeof sem2;
	sem2p= sem2 + sizeof sem2p;
        leido= (int *)sem2p+ sizeof leido;
	sem_init(sem1,1,0);
	sem_init(sem1p,1,0);
	sem_init(sem2,1,0);
	sem_init(sem2p,1,0);


        /* bucle para loopear por las opciones pasadas */
        while ((c=getopt(argc, argv, "i:"))>=0) {
                switch (c) {
                        case 'i':
                                opcion_i=optarg;
                                flagi++;
                                break;
                        default:
                                fprintf(stderr, "opcion invalida: %c\n", c);
                                        return 1;
        }
                }


    int procnum;
    int pid;

    for(procnum=1;procnum<3;procnum++){
        pid=fork();
        if(pid==0) break;
        else if(pid==-1) { perror("ERROR con fork"); break; }
    }

if(procnum==1){
        sem_wait(sem1); //sem rojo
        Hijo1(leido, ptr,sem1,sem1p);          
        return 0;
}

if(procnum==2){
        do{
        sem_wait(sem2);
         for(int i= 0; i<*leido;i++){
		buff[i]=ptr[i];
	        }
        Hijo2(*leido, buff);
        sem_post(sem2p);
        }while(*leido!=0); 
return 0;
}


        if(flagi!=0){
                int arch= open(opcion_i, O_RDWR , 0666);

               while((*leido=read(arch,ptr,120))>0){
                     
                        sem_post(sem2);
			            sem_post(sem1);
			            sem_wait(sem2p);
                        sem_wait(sem1p);
                        
                       
                }
                sem_post(sem1);
                close(arch);
        }else{
                while((*leido=read(STDIN_FILENO,buffer,sizeof buffer))>0){
                        sem_post(sem2);
			            sem_post(sem1);
			            sem_wait(sem2p);
                        sem_wait(sem1p);
                        
                }
                sem_post(sem1);

                }

               
                wait(NULL);
                return 0;
                           
}
