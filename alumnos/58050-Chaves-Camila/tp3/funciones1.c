#include <stdio.h>
#include <string.h>


const char* PalabrasReservadas[3];
int Contador[21];


void cerearcontador(){
    memset(Contador, 0, sizeof Contador);
}

void definir_pal_reservadas(){
	PalabrasReservadas[0]="THESE";
	PalabrasReservadas[1]="ADVENTURE";
	PalabrasReservadas[2]="WITHOUT";
	
}
