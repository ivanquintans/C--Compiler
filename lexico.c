#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "lexico.h"
#include "sistemaEntrada.h"
#include "TS.h"

char caracter = ' ';
int aceptado = 0; //valor por defecto que determina que no esta aceptada la cadena
compLexico comp = {0, NULL};

//Funciones para los autómatas que reconocen cada tipo

// Función auxiliar correspondente ao AF de cadeas alfanuméricas
char _alfanumerico() {
  //el primer carcter es alpha numerico asi que estamos en este automata
  caracter = sigCaracter();

  while (isalpha(caracter) || isdigit(caracter) || caracter == '_'){ //mientras sea alpha, numerico o un guion bajo es valido
    caracter = sigCaracter();
  }
  
  //TODO: ver despues como gestionamos la devolucion del lexema

}

char _numerico() {

  int estado_num = 0;

  /*el numero puede ser de diversos tipos:
    -integer literal
    -floating point literals 
    -imaginary
  */

  if (isdigit(caracter)){
    estado_num = 0; //mientras sea carcacter no hay problema
  } else estado_num = 1;

  switch (estado_num){

    case 0: //sigue siendo un numero

      caracter = sigCaracter();

      break;
    
    case 1:


    default:

      break;

  }

  


  
    
}

char _otroTipo(char caracter){
  return caracter;
}

//funcion que devuelve al sintactico el siguiente componente lexico 
//TODO: TOCA PONERSE CON LA CARNITA ASADA
char sigCompLexico(){
  //return(_alfanumerico());
  int estado =0;

  do{
    caracter = sigCaracter();
    
    //hacemos un switch por el estado 
    switch (estado){

      case 0: //primer caracter leido redirigimos a automas principales
    

        if (isdigit(caracter)){
          _numerico();
        }else if (isalpha(caracter) || caracter == '_'){
          _alfanumerico();
        }else{
          caracter = _otroTipo(caracter);
          return caracter;
        }
        
        
      
        break;
    
      default:
        break;
    }
    
  } while (caracter != EOF && aceptado==0);
  
  
}