#include <stdio.h>

#include "sintactico.h"
#include  "lexico.h"


// funcion que inicializa el analizador sintactico
void inicializarSintactico() {
    compLexico comp;
    do{
        comp = sigCompLexico();
        if (comp.lexema != NULL) { 
            printf("< %d, %s >\n", comp.codigo, comp.lexema);
        }
    }while (comp.codigo !=EOF);

    printf("Saliendo del programa\n");

}