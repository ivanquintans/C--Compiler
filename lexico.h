#ifndef LEXICO_H
#define LEXICO_H

#include "definiciones.h"
typedef struct{
  int codigo;
  char *lexema;
} compLexico;


// Función que le o seguinte compoñente léxico do código fonte
compLexico sigCompLexico();

//funcion auxiliar que se encarga de aceptar uma cadena
void lexemaAceptadoConcodigo(int codigo_componente);


void finalizarAnalizadorLexico();




#endif