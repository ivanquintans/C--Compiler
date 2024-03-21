#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H

#include "definiciones.h"
#include "lexico.h"

// funcion que inicializa la tabla de simbolos
void inicializarTablaSimbolos();

//funcion que imprime la tabla de simbolos
void imprimirTablaSimbolos();

// funcion que comprueba que esta en la tabla de simbolos y si no esta lo inserta
void insertarTablaSimbolos(compLexico *compLex);

// funciona que libera la memoria de manera adeduaca al finalizar la tabla de simbolos
void finalizarTablaSimbolos();


#endif