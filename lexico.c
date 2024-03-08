#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "lexico.h"
#include "sistemaEntrada.h"
#include "TS.h"

char caracter = ' ';

//Funciones para los autómatas que reconocen cada tipo

// Función auxiliar correspondente ao AF de cadeas alfanuméricas
void _alfanumerico() {
  
  caracter=sigCaracter();
  printf(caracter);
    
}