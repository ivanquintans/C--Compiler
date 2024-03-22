#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "lexico.h"
#include "sistemaEntrada.h"
#include "TS.h"
#include "errores.h"

char caracter = ' ';
int aceptado = 0; //valor por defecto que determina que no esta aceptada la cadena
int linea=1; //valor que nos permite lanzar errores de una manera exitosa
compLexico comp = {0, NULL};

/*Funcion encargada de aceptar el lexema actual*/
void lexemaAceptadoConcodigo(int codigo_componente){

    aceptarLexema(&comp,0);
    comp.codigo = codigo_componente;
    aceptado=1;

    /*Con el componente léxico que sea, llamamos a la funcion que acepta el lexema en el sistema de entrada*/
}

// Función auxiliar correspondente ao AF de cadeas alfanuméricas
void alfanumerico() {
  
  do{
    caracter = sigCaracter();
  }while (isalpha(caracter) || isdigit(caracter) || caracter == '_');//mientras sea alpha, numerico o un guion bajo es valido
    
  //como se procesa de manera correcta debemos de aceptar el lexema y retroceder
  retroceder();
  aceptarLexema(&comp,1);

  /*No podemos llamar a la funcion auxiliar ya que esta recibe el codigo y necesitamos buscar en la tabla de simbolos*/
  if (comp.codigo != ERROR){
    insertarTablaSimbolos(&comp);
  }
  aceptado = 1;

}

void numerico() {

    /*Los tipos de numeros que hay en wilcoxon.py son:
   
   - Enteros normales
   - Exponenciales
   - Floats
   - Hexadecimales  */

  int estado_num = 0;
  int error=0;

  //en primer lugar vamos a separar los caracteres de incio 
  if (caracter == '0'){
    estado_num = 0;
  }else if (caracter =='.'){
    estado_num = 1;
  }else {
    estado_num = 2;
  }

  do {
        
    switch (estado_num){
        case 0: /*Es un cero por lo que despues solo le puede venir una x o X (en el caso de wilcoxon) o un punto o un exponencial */

            caracter = sigCaracter();

            switch (caracter){

                case 'X':  /*En caso de que sea una x estamos ante un hexadecimal*/
                case 'x':
                    estado_num = 3;
                    break;

                case 'e': //si le entra una e estamos en un exponencial
                case 'E':
                    estado_num = 4;
                    break;

                case '.': //Automata que reconoce numeros hasta que llegue una e
                    estado_num = 5;
                    break;

                default: //si llega cualquier caracter que no es ese retrocedemos y aceptamos cadena
                    retroceder();
                    lexemaAceptadoConcodigo(NUMERO);
                    break;
            }

            break;
      
        case 1: /*El primer digito es un punto*/

        /*En caso de ser un punto lo unico que le puede serguir son digitos y guiones bajos*/
        caracter = sigCaracter();

        if (isdigit(caracter)){ // si lo que le sigue es un numero lo mandamos al automata de floats
            retroceder();    
            estado_num=5;
        }else{ //si no es un delimitador
            retroceder();
            lexemaAceptadoConcodigo('.');
        }
           
            break;

        case 2: //sigue siendo un numero

             do{
                caracter = sigCaracter();
                //comprobamos que si el caracter es un _ que no hayan dos seguidos
                if (caracter== '_'){
                    caracter = sigCaracter();
                    if (caracter == '_'){
                        error = 1;

                    }else{
                        //en caso de que no se encuentren dos giones seguido retrocedemos el puntero
                        retroceder();
                    }
                }

            }while (isdigit(caracter) || (caracter == '_'));

            /*Ahora debemos de comprobar que caracter fue el que nos saco ya que en funcion del tipo puede ser diferentes:*/

            if (caracter == '.'){// -Puede ser un . y ser un float ahora
                estado_num = 2; //despues del punto reconoce numeros enetros de todas formas
                

            } else if (caracter == 'e' || caracter == 'E'){ //-Puede ser una e y ser un exponent float
                /*Mandamos al automata que reconoce parte final de un exponencial*/
                estado_num = 4;
                
            }else { //-Puede ser un delimitador y hay que aceptar la cadena y retroceder
                retroceder();
                lexemaAceptadoConcodigo(NUMERO);
            }

            break;

        case 3: //inicio de hexadecimal

            do{
                caracter = sigCaracter();
                //comprobamos que si el caracter es un _ que no hayan dos seguidos
                if (caracter== '_'){
                    caracter = sigCaracter();
                    if (caracter == '_'){
                        error = 1;
                    }else{
                        //en caso de que no se encuentren dos giones seguido retrocedemos el puntero
                        retroceder();
                    }
                }

            }while (isxdigit(caracter) || (caracter == '_'));

            /*Debemos de retroceder para procesar el caracter*/
            retroceder();
            lexemaAceptadoConcodigo(NUMERO);

            break;

        case 4: /*Final de exponencial, depues puede ser varias cosas diferentes un menos o un mas, o numeros seguidos*/

             do{
                caracter = sigCaracter();
                
                
                /*Comprobación de dos mases seguidos*/
                if (caracter== '+'){
                    caracter = sigCaracter();
                    if (caracter == '+'){
                        error = 1;

                    }else{
                        //en caso de que no se encuentren dos giones seguido retrocedemos el puntero
                        retroceder();
                    }
                /*Comprobación de dos menos seguidos*/
                }else if (caracter== '-'){
                    caracter = sigCaracter();
                    if (caracter == '-'){
                        error = 1;

                    }else{
                        //en caso de que no se encuentren dos giones seguido retrocedemos el puntero
                        retroceder();
                    }
                /*Comprobación de dos __ seguidos*/
                }else if (caracter== '_'){
                    caracter = sigCaracter();
                    if (caracter == '_'){
                        error = 1;

                    }else{
                        //en caso de que no se encuentren dos giones seguido retrocedemos el puntero
                        retroceder();
                    }
                }

            }while (isdigit(caracter) || (caracter == '+') || (caracter == '-') || (caracter == '_'));

            /*Debemos de retroceder para procesar el caracter*/
            retroceder();
            lexemaAceptadoConcodigo(NUMERO);

            break;

        case 5: //reconocer segunda parte de un float

        do{
                caracter = sigCaracter();
               // printf("Caracter procesado %c\n",caracter);
                //comprobamos que si el caracter es un _ que no hayan dos seguidos
                if (caracter== '_'){
                    caracter = sigCaracter();
                    if (caracter == '_'){
                        error = 1;

                    }else{
                        //en caso de que no se encuentren dos giones seguido retrocedemos el puntero
                        retroceder();
                    }
                }

            }while (isdigit(caracter) || (caracter == '_'));

            /*Ahora debemos de comprobar que caracter fue el que nos saco ya que en funcion del tipo puede ser diferentes:*/

            if (caracter == 'e' || caracter == 'E'){ //-Puede ser una e y ser un exponent float
                /*Mandamos al automata que reconoce parte final de un exponencial*/
                estado_num = 4;
                
            }else { //-Puede ser un delimitador y hay que aceptar la cadena y retroceder
                retroceder();
                lexemaAceptadoConcodigo(NUMERO);
            }
            break;
    }

  }while (caracter != EOF && aceptado==0 && error==0 );
  
}

//automata que se encarga de reconocer un automata
void comentarios_strings(){

    int estado = 0;
    int saltado = 0;
    int error = 0;

    //si lo que se paso es una almohadilla
    if (caracter == '#'){
        estado = 0;

    }else if (caracter == '"'){
        //pedimos siguiente carater
        caracter = sigCaracter();

        if (caracter == '"'){ 
            caracter = sigCaracter(); //llevamos ""
            if (caracter == '"'){
                //vamos al automata que procesa caracteres hasta que encuentra tres comillas
                estado = 1; //llevamos """"
                
            }else{
                /*Dos comillas seguidas de un caracter diferente, string vacio*/
                estado = 4;
            }
            estado = 1; //compramos si viene otra comilla
        }else{ 
            /*Llevamos una comilla seguida de un caracter diferente (posible string)*/
            estado=3; //estado no aceptado
            //puede que sea un string
        }

    }else{ //son comillas simples

        //pedimos siguiente carater
        caracter = sigCaracter();

        if (caracter == '\''){ 
            caracter = sigCaracter(); //llevamos ""
            if (caracter == '\''){
                //vamos al automata que procesa caracteres hasta que encuentra tres comillas simples
                estado = 5; //llevamos '''
                
            }else{
                /*Dos comillas seguidas de un caracter diferente, string vacio*/
                estado = 8;
            }
            estado = 5; //compramos si viene otra comilla
        }else{ 
            /*Llevamos una comilla seguida de un caracter diferente (posible string)*/
            estado=7; //estado no aceptado
            //puede que sea un string
        }

    }

    while (saltado==0 && error == 0 && aceptado == 0){

        switch (estado){

            case 0:
                //tenemos que hacer un blucle para procesar toda la cadena hasta encontrar el "\n
                do{
                     caracter = sigCaracter();
                    

                }while(caracter!='\n');

                //retrocedemos para leer el '\n'
                retroceder();
                saltado=1;
                saltarLexema();
                break;
            
            case 1:
                //comentario multilinea

                do {
                    caracter = sigCaracter();
                   

                //encontro la primera comilla aun le faltan dos comillas mas para aceptarlo    
                }while (caracter != '"' && caracter != EOF);
                
                //comprobamos que se haya cerrado correctamente lo que queremos

                if (caracter == EOF) {
                    printf("Error no se ha cerrado el comentario multilinea");
                    error=1;
                }else{
                    //falta por procesar todavia dos comillas 
                    estado = 2;
                }
                break;

            case 2: //string literal largo multilinea, nos faltan dos comillas por procesar

                caracter = sigCaracter(); 
            
                if (caracter =='"'){//Llevamos ""
                    caracter = sigCaracter();
                    if (caracter == '"'){ //encontrado el string literal largo
                        lexemaAceptadoConcodigo(STRING);
                    }else{
                        estado = 1;
                    }
                }else{
                    //no se cerro de manera correcta asi que tenemos que esperar de nuevo tres """"
                    estado = 1;
                }
                break;

            case 3: //una comilla seguida de un caracter diferente a una comilla

            /*Debemos de leer caracteres hasta que leeamos una comilla 
            que significa que es un string. Tener cuenta el caso aislado de escapado de comillas con una barra */

                /*Caracter procesado */
                 do {
                    caracter = sigCaracter();

                //encontro la primera comilla aun le faltan dos comillas mas para aceptarlo    
                }while (caracter != '"' && caracter != EOF && caracter != '\\');

                if (caracter == '"'){
                    //si se leyo una comilla ya se cierra el string  y lo aceptamos
                    lexemaAceptadoConcodigo(STRING);
                }else if (caracter == '\\'){
                    /*Tenemos que procesar el siguiente caracter para evitar los escapados
                    Como por ejemplo:
                    \""*/
                    caracter = sigCaracter();
                    /*Una vez pillado el siguiente caracter seguimos en el mismo automata esperando la comilla*/
    
                }else{ //es un eof
                    imprimirError(COMENTARIO_MULTILINEA_NO_CERRADO);
                    error = 1;
                }

                break;

            case 4: //doble comilla es un string vacio

            //Al encontrar la doble comilla ya aceptamos la cadena
                aceptado = 1;
                break;

            case 5:
                //comentario multilinea
                do {
                    caracter = sigCaracter();
                //encontro la primera comilla aun le faltan dos comillas mas para aceptarlo    
                }while (caracter != '\'' && caracter != EOF);
                
                //comprobamos que se haya cerrado correctamente lo que queremos

                if (caracter == EOF) {
                    printf("Error no se ha cerrado el comentario multilinea");
                    error=1;
                }else{
                    //falta por procesar todavia dos comillas 
                    estado = 2;
                }
                break;

            case 6: //string literal largo multilinea, nos faltan dos comillas por procesar
                caracter = sigCaracter(); 
                if (caracter =='\''){//Llevamos ""
                    caracter = sigCaracter();
                    if (caracter == '\''){ //encontrado el string literal largo
                        lexemaAceptadoConcodigo(STRING);
                    }else{
                        estado = 1;
                    }
                }else{
                    //no se cerro de manera correcta asi que tenemos que esperar de nuevo tres """"
                    estado = 1;
                }
                break;

            case 7: //una comilla seguida de un caracter diferente a una comilla

            /*Debemos de leer caracteres hasta que leeamos una comilla 
            que significa que es un string. Tener cuenta el caso aislado de escapado de comillas con una barra */
                 do {
                    caracter = sigCaracter();
                //encontro la primera comilla aun le faltan dos comillas mas para aceptarlo    
                }while (caracter != '\'' && caracter != EOF && caracter != '\\');

                if (caracter == '\''){
                    //si se leyo una comilla ya se cierra el string  y lo aceptamos
                    lexemaAceptadoConcodigo(STRING);
                }else if (caracter == '\\'){
                    /*Tenemos que procesar el siguiente caracter para evitar los escapados
                    Como por ejemplo:
                    \""*/
                    caracter = sigCaracter();
                    /*Una vez pillado el siguiente caracter seguimos en el mismo automata esperando la comilla*/
    
                }else{ //es un eof
                    printf("Error no se ha cerrado el comentario multilinea");
                    error = 1;
                }

                break;

            case 8: //doble comilla es un string vacio

            //Al encontrar la doble comilla ya aceptamos la cadena
                aceptado = 1;
                break;

        }
    }

}

/*Funcion que redirreciona el automata a cualquiera de los otros casos que no sea alfanumerico o numérico*/
void otroTipo(){

    int estado = 0;
    int caracteresIgnorar = 0;


    switch (caracter){

        //si es una barra o una almohadilla lo mandamos al automata de comentarios
        case '"':
        case '\'':
        case '#':
        
            comentarios_strings();
            caracteresIgnorar=1;

            break;
        
        /*Casos de aceptacion directa de delimitadores y operadores*/
        
        case '(':
            lexemaAceptadoConcodigo('(');  
            break;

        case ')':
            lexemaAceptadoConcodigo(')');
            break;

        case '[':
            lexemaAceptadoConcodigo('[');
            break;
        
        case ']':
            lexemaAceptadoConcodigo(']');
            break;
        
        case '{':
            lexemaAceptadoConcodigo('{');
            break;

        case '}':
            lexemaAceptadoConcodigo('}');
            break;
        
        case '~':
            lexemaAceptadoConcodigo('~');
            
            break;

        case ',':
            lexemaAceptadoConcodigo(',');
            break;

        case '.':
            /*El punto lo gestionamos en el automata numérico*/
            lexemaAceptadoConcodigo('.');
            break;

        case ';':
            lexemaAceptadoConcodigo(';');
            break;

        case '!':
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(DISTINTO);
            }          
            break;

        /*Casos de aceptacion no directa de delimitadores y operadores*/

        case '=': 
            /*Puede ser o igual solo o igual igual*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(IGUALIGUAL);
            }else{ //no es un igual, retrocedemos y devolvemos solo el igual
                retroceder();
                lexemaAceptadoConcodigo('=');
            }
            break;

        case ':': 
            /*Puede ser o : solo o :=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(DOUSPUNTOSIGUAL);
            }else{ //no es un igual, retrocedemos y devolvemos solo el :
                retroceder();
                lexemaAceptadoConcodigo(':');

            }
            break;

        case '^': 
            /*Puede ser o ^ solo o ^=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(EXPONENCIALIGUAL);

            }else{ //no es un igual, retrocedemos y devolvemos solo el ^
                retroceder();
                lexemaAceptadoConcodigo('^');

            }
            break;

        case '|': 
            /*Puede ser o | solo o |=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(BARRAVERTICALIGUAL);

            }else{ //no es un igual, retrocedemos y devolvemos solo el |
                retroceder();
                lexemaAceptadoConcodigo('|');
            }
            break;

        case '&': 
            /*Puede ser o & solo o &=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(UNPERSANTIGUAL);
            }else{ //no es un igual, retrocedemos y devolvemos solo el &
                retroceder();
                lexemaAceptadoConcodigo('&');
            }
            break;

        case '@': 
            /*Puede ser o @ solo o @=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(ARROBAIGUAL);
            }else{ //no es un igual, retrocedemos y devolvemos solo el @
                retroceder();
                lexemaAceptadoConcodigo('@');

            }
            break;

        case '%': 
            /*Puede ser o % solo o %=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(PORCIENTOIGUAL);
            }else{ //no es un igual, retrocedemos y devolvemos solo el %
                retroceder();
                lexemaAceptadoConcodigo('%');
            }
            break;

        case '+': 
            /*Puede ser o + solo o +=*/
            caracter = sigCaracter();
            if (caracter == '='){
                lexemaAceptadoConcodigo(MASIGUAL);
            }else{ //no es un igual, retrocedemos y devolvemos solo el +
                retroceder();
                lexemaAceptadoConcodigo('+');
            }
            break;


        case '>':
            estado = 1;
            break;

        case '<':
            estado = 3;
            break;

        case '/':
            estado = 7;
            break;

        case '*':
            estado = 5;
            break;

        case '-':
            estado = 9;
            break;
        

        default:
                saltarCaracter();
                //Está pensado para los \t, \n e espacios do código
                if (caracter == '\n') {
                    linea++;
                }
                caracteresIgnorar=1;
            break;
    }

    do{

        switch (estado){

            case 0: //se aceptan directamente
                continue;
                break;

            case 1: // se leyó >

                caracter = sigCaracter();
                switch(caracter){

                    case '>': //>>
                        estado=2;
                        break;
                    
                    case '=': //>=
                        lexemaAceptadoConcodigo(MAYORIGUAL);
                        break;

                    default: //solo es >
                        retroceder();
                        lexemaAceptadoConcodigo('>');
                        break;
                }

                break;

            case 2: // se leyó >>

                caracter = sigCaracter();

                if (caracter == '='){ //se leyo >>=
                    lexemaAceptadoConcodigo(MAYORMAYORIGUAL);
                }else{ //se leyo solo >>
                    retroceder();
                    lexemaAceptadoConcodigo(MAYORMAYOR);
                }
                break;


            case 3: // se leyó <

                caracter = sigCaracter();

                switch(caracter){

                    case '<': //<<
                        estado=4;
                        break;
                    
                    case '=': //<=
                        lexemaAceptadoConcodigo(MENORIGUAL);
                        break;

                    default: //solo es <
                        retroceder();
                        lexemaAceptadoConcodigo('<');
                        break;
                }
                
                break;

            case 4: // se leyó <<

                caracter = sigCaracter();

                if (caracter == '='){ //se leyo <<=
                    lexemaAceptadoConcodigo(MENORMENORIGUAL);
                }else{ //se leyo solo <<
                    retroceder();
                    lexemaAceptadoConcodigo(MENORMENOR);
                }

                break;

            case 5: // se leyó *

                caracter = sigCaracter();

                switch(caracter){

                    case '*': //**
                        estado=4;
                        break;
                    
                    case '=': //*=
                        lexemaAceptadoConcodigo(ASTERISCOIGUAL);
                        break;

                    default: //solo es *
                        retroceder();
                        lexemaAceptadoConcodigo('*');

                        break;
                }
                
                break;

            case 6: // se leyó **

                caracter = sigCaracter();

                if (caracter == '='){ //se leyo **=
                    lexemaAceptadoConcodigo(ASTERISCOASTERISCOIGUAL);
                }else{ //se leyo solo **
                    retroceder();
                    lexemaAceptadoConcodigo(ASTERICOASTERISCO);
                }

                break;

            case 7: // se leyó /

                caracter = sigCaracter();
                switch(caracter){

                    case '/': // //
                        estado=4;
                        break;
                    
                    case '=': // /=
                        lexemaAceptadoConcodigo(BARRAIGUAL);
                        break;

                    default: //solo es /
                        retroceder();
                        lexemaAceptadoConcodigo('/');
                        break;
                }
                
                break;

            case 8: // se leyó //

                caracter = sigCaracter();

                if (caracter == '='){ //se leyo //=
                    lexemaAceptadoConcodigo(BARRABARRAIGUAL);
                }else{ //se leyo solo //
                    retroceder();
                    lexemaAceptadoConcodigo(BARRABARRA);
                }

                break;

            case 9:  // -

            caracter = sigCaracter();

            switch (caracter){

                case '>': // ->
                    lexemaAceptadoConcodigo(GUIONFLECHA);
                    break;

                case '=': // -=
                    lexemaAceptadoConcodigo(MENOSIGUAL);
                    break;

                default: //solo es un -

                    retroceder();
                    lexemaAceptadoConcodigo('-');
                    break;
            }
        }
        
    }while (aceptado == 0 && caracteresIgnorar == 0);

    caracteresIgnorar=0;

}

// Función auxiliar que libera a memoria asociada a un lexema lido anteriormente e restablece os valores inicias
void destruirComp() {
    if (comp.lexema != NULL) {
        free(comp.lexema);
        comp.codigo = 0;
        comp.lexema = NULL;
    }
}

//funcion que devuelve al sintactico el siguiente componente lexico 
compLexico sigCompLexico(){

    //vaciamos el anterior por si acaso
    destruirComp();

  do{

    caracter = sigCaracter();
    

    //si leo el /0 o el EOF paso al siguiente caracter 

    if (caracter != '\0' && caracter != EOF) {

        if (isdigit(caracter) || caracter == '.'){
            numerico();
            //en caso de ser identificador
        }else if (isalpha(caracter) || caracter == '_'){
            alfanumerico();
        }else{
                //en caso de que sea cualquier otra cosa
            otroTipo();
        }
    }
 //primer caracter leido redirigimos a automas principales

    
    
  } while (caracter != EOF && aceptado==0);


  if (caracter == EOF && aceptado == 0){
        /*Si es el EOF lo indicamos y finalizamos el analisis*/ 
        comp.lexema = NULL;
        comp.codigo = EOF;
                           
  }
  aceptado = 0;

  return comp;
}
// Función que finaliza el analizador léxico
void finalizarAnalizadorLexico() {
    destruirComp();
}