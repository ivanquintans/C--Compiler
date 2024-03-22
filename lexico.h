#ifndef LEXICO_H
#define LEXICO_H

#include "definiciones.h"
typedef struct{
  int codigo;
  char *lexema;
} compLexico;

/**
 * Funcion encargada de proporcionar el componente léxico al sintactico
 */
compLexico sigCompLexico();

/**
 * Funcion auxiliar que se encarga de aceptar el lexema con el codigo especificado
 * @param compActual codigo con el que se aceptara el componente.
 */
void lexemaAceptadoConcodigo(int codigo_componente);

/**
 * Funcion encargada de finalizar el analizador lexico
 */
void finalizarAnalizadorLexico();

#endif