#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "lexico.h"
#include "sistemaEntrada.h"
#include "TS.h"

char caracter = ' ';
compLexico comp = {0, NULL};

//Funciones para los autómatas que reconocen cada tipo

// Función auxiliar correspondente ao AF de cadeas alfanuméricas
char _alfanumerico() {
  
  caracter=sigCaracter();
  //printf(caracter);
    
}

//funcion que devuelve al sintactico el siguiente componente lexico 
char sigCompLexico(){
  return(_alfanumerico());
  
}