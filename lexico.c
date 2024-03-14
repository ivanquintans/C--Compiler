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

    89789.
  */

  //en primer lugar vamos a separar los caracteres de incio 
  if (caracter == 0){
    estado_num = 1;
  }else if (caracter =='.'){
    estado_num =2;
  }else {
    estado_num =0;
  }

  do {
    caracter = sigCaracter();
    switch (estado_num){

      case 0: //sigue siendo un numero
        if(!isdigit(caracter)){ //redirecion de tipo

          switch (caracter){

            case '.':
              estado_num=2;
              break;
            


            default:


              break;
          }
        
      
        }

      
        break;
      
      case 1: //redirecion de tipo

      //TODO: TENER CUIDADO CON LO HEX
          if (caracter == 'b' || caracter == 'B' || caracter == 'X' || caracter == 'x' || caracter == 'o' || caracter == 'O'){
            estado_num = 2;
          }else if (caracter ==' '){

          }

      



      default:

        break;
    }

  }while (caracter != EOF && aceptado==0 );
  
}

//funcion que devuelve al sintactico el siguiente componente lexico 

char sigCompLexico(){
  //
  int estado =0;

  //do{
   
    caracter = sigCaracter();
        

    /*
    
    //hacemos un switch por el estado 
    switch (estado){

      case 0: //primer caracter leido redirigimos a automas principales
    

        if (isdigit(caracter)){
          _numerico();
        }else if (isalpha(caracter) || caracter == '_'){
          _alfanumerico();
        }else{
            switch (caracter)
          
        }
        
        break;
    
      default:

        break;
    }
    
  } while (caracter != EOF && aceptado==0);


  */
  
  
}