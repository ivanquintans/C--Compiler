#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "lexico.h"
#include "sistemaEntrada.h"
#include "TS.h"

char caracter = ' ';
int aceptado = 0; //valor por defecto que determina que no esta aceptada la cadena
int linea=1; //valor que nos permite lanzar errores de una manera exitosa
compLexico comp = {0, NULL};

//Funciones para los autómatas que reconocen cada tipo

// Función auxiliar correspondente ao AF de cadeas alfanuméricas
void alfanumerico() {
  //el primer caracter es alpha numerico asi que estamos en este automata
  caracter = sigCaracter();
  printf("Caracter Procesado %c\n",caracter);

  while (isalpha(caracter) || isdigit(caracter) || caracter == '_'){ //mientras sea alpha, numerico o un guion bajo es valido
    caracter = sigCaracter();
    printf("Caracter Procesado %c\n",caracter);
  }

  //como se procesa de manera correcta debemos de aceptar el lexema y de retroceder
  printf("Identificador Aceptado \n");
  retroceder();
  
  

}

void numerico() {

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

    

      



      default:

        break;
    }

  }while (caracter != EOF && aceptado==0 );
  
}

//automata que se encarga de reconocer un automata
void comentarios(){

    int estado = 0;
    int saltado = 0;
    int error = 0;

    //si lo que se paso es una almohadilla
    if (caracter == '#'){
        estado = 0;

    }else{
        //pedimos siguiente carater
        caracter = sigCaracter();

        if (caracter == '"'){
            caracter = sigCaracter();
            if (caracter == '"'){
                //vamos al automata que procesa caracteres hasta que encuentra tres comillas
                estado = 1;
                
            }else{
                estado = 3;
            }
            estado = 1; //compramos si viene otra comilla
        }else{
            estado=3; //estado no aceptado
            //puede que sea un string
        }
    }

    while (saltado==0 && error ==0){

        switch (estado){

            case 0:

                //tenemos que hacer un blucle para procesar toda la cadena hasta encontrar el "\n"

                do{
                     caracter = sigCaracter();
                     printf("Caracter procesado %c\n",caracter);

                }while(caracter!='\n');

                saltado=1;


                break;
            
            case 1:

                //comentario multilinea

                do {
                    caracter = sigCaracter();
                    printf("Caracter procesado %c\n",caracter);

                //encontro la primera comilla aun le faltan dos comillas mas para aceptarlo    
                }while (caracter != '"' && caracter != EOF);
                
                //comprobamos que se haya cerrado correctamente lo que queremos

                if (caracter == EOF) {
                    printf("Error no se ha cerrado el comentario multilinea");
                }else{
                    //falta por procesar todavia dos comillas 
                    estado = 2;
                }


                break;

            case 2: //comentario multilinea, nos faltan dos comillas por procesar

                caracter = sigCaracter(); 

                if (caracter =='"'){//Llevamos ""
                    caracter = sigCaracter();
                    if (caracter == '"'){
                        saltado = 1;
                        printf("Saltado\n");
                    }else{
                        estado = 1;
                    }
                }else{
                    //no se cerro de manera correcta asi que tenemos que esperar de nuevo tres """"
                    estado = 1;
                }

                break;

            case 3:




                break;
            
            default:


                break;


        }
    }

}





/*Funcion que redirreciona el automata a cualquiera de los otros casos que no sea alfanumerico o numérico*/
void otroTipo(){

    switch (caracter){

        //si es una barra o una almohadilla lo mandamos al automata de comentarios
        case '"':
        case '#':
        
            comentarios();

            break;
        
        case 0:
            
            break;
        
   
        
        
        default:

            break;
    }

}

//funcion que devuelve al sintactico el siguiente componente lexico 

char sigCompLexico(){

 
  //
  int estado =0;

  do{
   
    caracter = sigCaracter();
    printf("Caracter leido %c\n",caracter);
    //return caracter;
    
    
    //hacemos un switch por el estado 
    switch (estado){

      case 0: //primer caracter leido redirigimos a automas principales
    
        //en caso de ser un numero
        if (isdigit(caracter)){
          numerico();
        //en caso de ser identificador
        }else if (isalpha(caracter) || caracter == '_'){
          alfanumerico();
        }else{
            //en caso de que sea cualquier otra cosa
            otroTipo();
        }
        
        break;
    
      default:

        break;
    }
    
  } while (caracter != EOF && aceptado==0);



  
  
}