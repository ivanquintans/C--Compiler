#include <stdio.h>

#include "sintactico.h"
#include  "lexico.h"


// funcion que inicializa el analizador sintactico
void inicializarSintactico() {
    compLexico comp;
    char caracter =' ';

    do{
        caracter = sigCompLexico();
        //printf("Caracter leido: %c\n", caracter);
        
    }while (caracter != EOF);
    

  
    /*while ((comp = segCompLexico()).codigo != EOF) {
        if (comp.lexema != NULL) { 
            printf("< %d, %s >\n", comp.codigo, comp.lexema);
        }
    }
    */
    

    //finalizarAnalizadorLexico();
}