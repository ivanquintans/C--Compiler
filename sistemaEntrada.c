#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistemaEntrada.h"
#include "lexico.h"
#include "definiciones.h"
#include "errores.h"
#define TAM 16
FILE *archivo;
char caracter_actual;

typedef struct {
    char A [TAM];
    char B [TAM];
    int inicio;
    int delantero;
    int current; //Si 0 A sino B
    int no_cargar; // se encarga de no cargar el  buffer cuando se haya retrocedido justo al anterior
    int tamLexemaActual; //se aumenta cada vez que se actualiza el valor de delantero
}Buffer;

Buffer miBuffer;

//Inicializamos el buffer principal
void incializarBuffer(){
    miBuffer.inicio=0;
    miBuffer.delantero=-1;
    miBuffer.current=0; //A
    miBuffer.no_cargar=0;
    miBuffer.tamLexemaActual=0;
}

//cargamos el primero de los bloques
void cargarBloque(){
   

    int leidos; //variable que controla los bytes leidos por fread
    
    if (miBuffer.current == 0){ //A
         
        //comprobamos para saber si estamos en el eof si el numero de bytes recibidos es menor al tamaño
        if ((leidos = fread(miBuffer.A,sizeof(char),TAM-1,archivo))< TAM-1){
            /*Si se cumple la condición añadimos el EOF en el buffer*/
            miBuffer.A[leidos] = EOF;
        }
        //ahora añadimos el EOF
        miBuffer.A[TAM -1] = '\0';
    
    }else{ //B

        //comprobamos para saber si estamos en el eof si el numero de bytes recibidos es menor al tamaño
        if ((leidos = fread(miBuffer.B,sizeof(char),TAM-1,archivo)) < TAM-1){
            /*Si se cumple la condición añadimos el EOF en el buffer*/
            miBuffer.B[leidos] = EOF;
        }
        miBuffer.B[TAM -1] = '\0';
  }
  
}

//funcion para alternar el bloque en caso de querer cambiarlo
void alternarBloque(){

    if (miBuffer.current==0){ //A
        miBuffer.current=1;
    }else{ //B
        miBuffer.current=0;
        miBuffer.delantero=0; //Devolvemos el puntero delantero al inicio del primer buffer
    } 
}


// funcion que inicializa el sistema de entrada

void iniciarSistemaEntrada(char *fichero){
    archivo = fopen(fichero, "r");

    // comprobamos que este se abre de forma correcta
    if (archivo == NULL){
        imprimirError(ERROR_ABRIR_ARCHIVO);
        exit(EXIT_FAILURE);
    }
    //dejamos el fichero abierto

    //llamamos a inicilizar el buffer
    incializarBuffer();
    cargarBloque();

}

// funcion que se encarga de cerrar el archivo
void finalizarSistemaEntrada(){
    fclose(archivo);
}

// funcion que se encarga de enviar un caracter al analizadorlexico
char sigCaracter(){

    if (miBuffer.current==0){//A

        /*Como movemos el puntero y despues leemos, nuestro puntero empieza en la poscicion menos 1*/

        miBuffer.delantero++;
        miBuffer.tamLexemaActual++; //en caso de no ser la posición cero, muevo el puntero y luego consumo
        caracter_actual = miBuffer.A[miBuffer.delantero];
        
        /*Para comprobar si se llego al final del archivo vamos a comprobar 
        que el tamaño de bytes recibidos en el fread sea menor que los bytes recibidos */

        if (caracter_actual!= EOF){ //Si no se llego al fin de fichero
            if (caracter_actual!= '\0'){ 
                return caracter_actual;
            }else{ //en caso de ser el EOF del buffer
                alternarBloque();
                if(miBuffer.no_cargar == 0){ /*En el caso de que no vengamos de retroceder en el buffer*/
                    cargarBloque();
                }
                /*Devolvemos el valor para que se cargue*/
                miBuffer.no_cargar=0;
                /*Reajustamos el valor a devolver*/
                miBuffer.delantero++;
                caracter_actual = miBuffer.B[miBuffer.delantero - TAM];
                return caracter_actual;
            }

        }else{
            return EOF; //devuelvo el EOF del archivo
        }

    }else{ //B

        miBuffer.delantero++;
        miBuffer.tamLexemaActual++;
        caracter_actual = miBuffer.B[miBuffer.delantero - TAM]; //sirve para modular el segundo buffer

        /*Para comprobar si se llego al final del archivo vamos a comprobar 
        que el tamaño de bytes recibidos en el fread sea menor que los bytes recibidos */

        if (caracter_actual!= EOF){ //Si no se llego al fin de fichero
            if (caracter_actual!= '\0'){ 
                return caracter_actual;
            }else{ //en caso de ser el EOF del buffer
                alternarBloque();
                /*En el caso de que vengamos de retroceder en el buffer*/
                if(!miBuffer.no_cargar){
                    cargarBloque();
                }
                /*Devolvemos el valor para que se cargue*/
                miBuffer.no_cargar=0;
                //devuelvo ya el caracter siguiente
                caracter_actual = miBuffer.A[miBuffer.delantero];
                return caracter_actual;
            }
        }else{
            //final del archivo de verdad
            return EOF; //devuelvo el EOF del archivo
        }
    }
}

void retroceder(){

    /*En el caso de retroceder debemos de tener en cuenta de que si retrocedemos
    un bloque despues no debemos de volver a cargar este
    
    Para eso empleamos el atributo no cargar que se encarga de controlar esta situación*/

    /*Si estoy en el bloque A*/

    if (miBuffer.current == 0){
        if (miBuffer.delantero == 0){
            alternarBloque();
            miBuffer.delantero = 2* TAM -2; //ultima posición del buffer b antes del \0
            /*Valor que se encarga de que no sobrescriba el buffer*/
            miBuffer.no_cargar=1;
        }else{
          miBuffer.delantero--;
        }
    }else{ //En caso del bloque B

        if (miBuffer.delantero == TAM){
            miBuffer.current = 0;
            miBuffer.delantero -= 2; //ultima posición del buffer a antes del \0
             /*Valor que se encarga de que no sobrescriba el buffer*/
            miBuffer.no_cargar=1;
        }else{
            miBuffer.delantero--;
        }
    }

     /*Si saltamos un lexema reseteamos el valor de lexema*/
    miBuffer.tamLexemaActual--;

    //siempre que retrocedemos debemos de cambiar el caracter actual
}

void saltarLexema() {

    /*Funcion que se encarga de actualizar el valor del inicio*/

    if (miBuffer.delantero == TAM -2){ //si el valor es la penultima posición del buffer a
        miBuffer.inicio = miBuffer.delantero + 2;
    }else if (miBuffer.delantero == (2 * TAM)-2){ // si el valor es la penultima del buffer b
        miBuffer.inicio = 0;
    } else {
        miBuffer.inicio = miBuffer.delantero + 1;
    }

    /*Si saltamos un lexema reseteamos el valor de lexema*/
    miBuffer.tamLexemaActual=0;

}


void saltarCaracter(){

    if (miBuffer.current == 0) {  // Si estamos en el buffer a

        if (miBuffer.delantero == (TAM -2)){ //si estamos en la penultima posicion del buffer a saltamos dos posiciones y cambiamos el bloque activo
            /*Adelantamos tanto el inicio como el buffer delantero*/
            miBuffer.delantero++;
            miBuffer.inicio+=2;
            /*Cambiamos el bloque activo y cargamos*/
            alternarBloque();
            cargarBloque();
        }else{
            miBuffer.inicio++;
        }

    } else { // Si estamos en el buffer b

        if (miBuffer.delantero == (2 * TAM)-2){

            /*Alternamos el bloque pero no dejamos el puntero en la posición 0 ya que no estamos devlviendo el caracter
            Por lo que no llamas a la funcion alternar bloque*/

            miBuffer.current=0;
            miBuffer.delantero=-1;
            cargarBloque();
            miBuffer.inicio=0;

        }else{
            miBuffer.inicio++;
        }
    }
     /*Si saltamos un lexema reseteamos el valor de lexema*/
    miBuffer.tamLexemaActual=0;
   
}

void aceptarLexema(compLexico *compActual, int identifcador){

    int tamLexema=0;

    if (miBuffer.tamLexemaActual <= (TAM -1)){ 


        //En caso de que el inicio y el fin esten en a o en b
        if( (miBuffer.delantero < TAM -1 && miBuffer.inicio < TAM -1) || (miBuffer.delantero > TAM -1 && miBuffer.inicio > TAM -1)){
            
            tamLexema = (miBuffer.delantero - miBuffer.inicio) +1;
            compActual->lexema = malloc (tamLexema +1); //reservamos memoria para el lexema

            //hacemos la copia en el lexema del buffer que queremos
            //mas la posición inicio para asi empezar desde donde queremos

            if (miBuffer.current == 0){
                strncpy(compActual->lexema, miBuffer.A + miBuffer.inicio, tamLexema); //ponemos miBuffer.A + miBuffer.inicio para evitar warnings (aritmetica de punteros)
                compActual->lexema[tamLexema] = '\0'; //evitar errores
            }else {
                strncpy(compActual->lexema, miBuffer.B + miBuffer.inicio - TAM, tamLexema);
                compActual->lexema[tamLexema] = '\0'; //evitar errores
            }

        //Inicio en A y delantero en B
        }else if(miBuffer.inicio < TAM -1 && miBuffer.delantero > TAM -1){

            //copiamos la parte del Bloque A y luego le concatenemos la parte del bloque B
            int tamPrimeraparte = ((TAM - 2) - miBuffer.inicio) +1;
            int tamSegundaparte = (miBuffer.delantero - TAM) +1;

            //el tamaño del lexema es la suma de los dos tamaños
            tamLexema = tamPrimeraparte + tamSegundaparte;
            compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'

            //copiamos la parte del bloque A
            strncpy(compActual->lexema,miBuffer.A + miBuffer.inicio, tamPrimeraparte);
            //copiamos la parte del bloque B
            strncpy(compActual->lexema + tamPrimeraparte,miBuffer.B, tamSegundaparte); //le restamos al delantero la parte del buffer A
            //se mete el '\0'
            compActual->lexema[tamLexema] = '\0';

        //Inicio en B y delantero en A
        }else if(miBuffer.inicio > TAM -1 && miBuffer.delantero < TAM -1){

            int tamPrimeraparte = ((2 * TAM -2)- miBuffer.inicio)+1;
            int tamSegundaparte = (miBuffer.delantero)+1;

            //el tamaño del lexema es la suma de los dos tamaños
            tamLexema = tamPrimeraparte + tamSegundaparte;
            compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'

            //copiamos la parte del lexema del bloque B
            strncpy(compActual->lexema,miBuffer.B + (miBuffer.inicio - TAM),tamPrimeraparte); //el numero a copiar es el tamaño de lexema -1 menos la posicón del buffer B
            //copiamos la parte del bloque A
            strncpy(compActual->lexema + tamPrimeraparte, miBuffer.A, tamSegundaparte );
            //se mete el '\0'
            compActual->lexema[tamLexema] = '\0';

        }

    }else{
        
        //imprimos el error en caso de que el tamano sea mayor
        imprimirError(TAM_LEXEMA_EXCEDIDO);

        if (identifcador == 1){ //en caso de que sea un identificador
            //ponemos el codigo a null ya que esto permite no insertar el elemento en la tabla de simbolos
            compActual->codigo = ERROR;
            
        } //no es un identificador simplemente imprimimos

        /*Lo que hacemos es imprimir la parte del bloque donde se encuentra delantero (parte final)*/
        
        /*Si delantero esta en el buffer A imprimimos la partde del buffer a hasta delantero*/
        if (miBuffer.delantero < TAM -1){
            
            /*Además debemos comprobar que no se este devolviendo 0 caracteter*/

            //si solo devolvemos un caracter devolvemos el buffer anterior
            tamLexema = miBuffer.delantero +1;

            if (tamLexema <= 1){ //devolvemos el otro buffer

                tamLexema = TAM -1; //tamaño máximo
                compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'
                strncpy(compActual->lexema, miBuffer.B, tamLexema); 
                compActual->lexema[tamLexema] = '\0'; //evitar errores
                
            }else{
                compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'
                strncpy(compActual->lexema, miBuffer.A, tamLexema); 
                compActual->lexema[tamLexema] = '\0'; //evitar errores
            }
           

        //si delantero esta en el buffer b lo contrario    
        }else {

            tamLexema = (miBuffer.delantero - TAM)+1;

            if (tamLexema <= 1){ //devolvemos el otro buffer

                tamLexema = TAM -1; //tamaño máximo
                compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'
                strncpy(compActual->lexema, miBuffer.A, tamLexema); 
                compActual->lexema[tamLexema] = '\0'; //evitar errores

             }else{
                compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'
                strncpy(compActual->lexema, miBuffer.B, tamLexema);
                compActual->lexema[tamLexema] = '\0'; //evitar errores

             }   
            
        }
    }   
    //una vez aceptado el lexema debemos de saltarlo
    saltarLexema();
}