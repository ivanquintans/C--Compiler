#ifndef LEXICO_H
#define LEXICO_H

#include "definiciones.h"
typedef struct
{
  int codigo;
  char *lexema;
} compLexico;

// Función que inicia o analizador léxico
void iniciarAnalizadorLexico();

// Función que le o seguinte compoñente léxico do código fonte
char sigCompLexico();

#endif