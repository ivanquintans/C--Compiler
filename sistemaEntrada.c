#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistemaEntrada.h"
#include "lexico.h"
#define TAM 16
FILE *archivo;
char caracter_actual;

typedef struct {
    char A [TAM];
    char B [TAM];
    int inicio;
    int delantero;
    int current; //Si 0 A sino B
}Buffer;

Buffer miBuffer;

//Inicializamos el buffer principal
void incializarBuffer(){
    miBuffer.inicio=0;
    miBuffer.delantero=0;
    miBuffer.current=0; //A
}

//cargamos el primero de los bloques
void cargarBloque(){
    //en funcion de cual sea el current cargamos el bloque a o el bloque b

    int leidos; //variable que controla los bytes leidos por fread
    
    if (miBuffer.current == 0){
         //si estamos en el bloque A

        //comprobamos para saber si estamos en el eof si el numero de bytes recibidos es menor al tamaño
        if ( (leidos = fread(miBuffer.A,sizeof(char),TAM-1,archivo))< TAM-1){
            /*Si se cumple la condición añadimos el EOF en el buffer*/
            miBuffer.A[leidos] = EOF;

        }

        //ahora añadimos el EOF
        miBuffer.A[TAM -1] = '\0';

        for(int i=0;i<TAM;i++) printf(" BLOQUE A: Esta es la posicion %d %c\n",i,miBuffer.A[i]);
    
    }else{ //Si estamos en el bloque b

        //comprobamos para saber si estamos en el eof si el numero de bytes recibidos es menor al tamaño
        if ( (leidos = fread(miBuffer.B,sizeof(char),TAM-1,archivo)) < TAM-1){
            /*Si se cumple la condición añadimos el EOF en el buffer*/
            miBuffer.A[leidos] = EOF;

        }
        miBuffer.B[TAM -1] = '\0';
        for(int i=0;i<TAM;i++) printf("BLOQUE B:Esta es la posicion %d %c\n",i,miBuffer.B[i]);


  }
  
}

//funcion para alternar el bloque en caso de querer cambiarlo

void alternarBloque(){

    if (miBuffer.current==0){ //Si es a
        printf("Estoy cambiando a B\n");
        miBuffer.current=1;
    }else{
        miBuffer.current=0;
        miBuffer.delantero=0;
        printf("Estoy cambiando a A\n");
         //Devolvemos el puntero delantero al inicio del primer buffer
    } 

}


// funcion que inicializa el sistema de entrada

void iniciarSistemaEntrada(char *fichero){
    archivo = fopen(fichero, "r");

    // comprobamos que este se abre de forma correcta
    if (archivo == NULL){
        printf("Error a  la hora de abrir er archivo\n");
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
    //debemos de diferenciar si estamos en el buffer a o en el b
    //printf("Delantero esta en %d\n",miBuffer.delantero);
    if (miBuffer.current==0){//A

        caracter_actual = miBuffer.A[miBuffer.delantero];
        //printf("Voy a leer el caracter %c\n",caracter_actual);

        /*Para comprobar si se llego al final del archivo vamos a comprobar 
        que el tamaño de bytes recibidos en el fread sea menor que los bytes recibidos */

        if (caracter_actual!= EOF){ //Si no se llego al fin de fichero
            if (caracter_actual!= '\0'){ //si    
                //printf("Estoy aqui dentro\n");
                miBuffer.delantero+=1;
                return caracter_actual;
            }else{ //en caso de ser el EOF del buffer
                alternarBloque();
                cargarBloque();
                //retornamos null en caso de ser el eof ya que no lo quiero procesar
                return caracter_actual;
                
            }

        }else{
            //final del archivo tengo que acabar de leer lo que esta en el buffer antes de retornar el eof

    
            return EOF; //devuelvo el EOF del archivo
        }

    }else{
        caracter_actual = miBuffer.B[miBuffer.delantero - (TAM-1)]; //sirve para modular el segundo buffer

        /*Para comprobar si se llego al final del archivo vamos a comprobar 
        que el tamaño de bytes recibidos en el fread sea menor que los bytes recibidos */

        if (caracter_actual!= EOF){ //Si no se llego al fin de fichero
            if (caracter_actual!= '\0'){ //si 
                miBuffer.delantero+=1;
                return caracter_actual;
            }else{ //en caso de ser el EOF del buffer
                alternarBloque();
                cargarBloque();
                //retornamos null en caso de ser el eof ya que no lo quiero procesar
                return caracter_actual;
            }
        }else{
            //final del archivo de verdad
            return EOF; //devuelvo el EOF del archivo
        }
    }
}

void retroceder(){
    miBuffer.delantero-=1; //tal y como esta implementando nunca va a ser cero por lo que no da problemas
}

void aceptarLexema(compLexico *compActual){

    int tamLexema;

    //varios casos posibles

    //En caso de que el inicio y el fin esten en a o en b
    if( (miBuffer.delantero < TAM -1 && miBuffer.inicio < TAM -1) || (miBuffer.delantero > TAM -1 && miBuffer.inicio > TAM -1)){
        tamLexema = miBuffer.delantero - miBuffer.inicio;
        compActual->lexema = malloc (tamLexema +1); //reservamos memoria para el lexema

        //hacemos la copia en el lexema del buffer que queremos
        //mas la posición inicio para asi empezar desde donde queremos

        if (miBuffer.current == 0){
            strncpy(compActual->lexema, miBuffer.A[miBuffer.inicio], tamLexema);
            compActual->lexema[TAM -1] = '\0'; //evitar errores
        }else {
            strncpy(compActual->lexema, miBuffer.B[miBuffer.inicio], tamLexema);
            compActual->lexema[TAM -1] = '\0'; //evitar errores
        }
    //Inicio en A y delantero en B
    }else if(miBuffer.inicio < TAM -1 && miBuffer.delantero > TAM -1){

        tamLexema = miBuffer.delantero - miBuffer.inicio;
        compActual->lexema = malloc (tamLexema +1);//reservamos memoria para el lexama mas el '\0'

        //copiamos la parte del Bloque A y luego le concatenemos la parte del bloque B
        int tamPrimeraparte = ((tamLexema -1) - miBuffer.inicio);

        strncpy(compActual->lexema,miBuffer.A[miBuffer.inicio], tamPrimeraparte);
        //copiamos la parte del bloque B
        strncpy(compActual->lexema + tamPrimeraparte,miBuffer.B, miBuffer.delantero - (TAM-1)); //le restamos al delantero la parte del buffer A
        //se mete el '\0'
        compActual->lexema[miBuffer.delantero] = '\0';

    //Inicio en B y delantero en A
    }else if(miBuffer.inicio > TAM -1 && miBuffer.delantero < TAM -1){

        tamLexema = 2*TAM - miBuffer.delantero + miBuffer.inicio;

         int tamPrimeraparte = (tamLexema -1) - (miBuffer.inicio - TAM);

        //copiamos la parte del lexema del bloque B
        strncpy(compActual->lexema,miBuffer.B[(miBuffer.inicio - TAM)],tamPrimeraparte); //el numero a copiar es el tamaño de lexema -1 menos la posicón del buffer B
        //copiamos la parte del bloque A
        strncpy(compActual->lexema + tamPrimeraparte, miBuffer.A, miBuffer.delantero +1 );
        //se mete el '\0'
        compActual->lexema[tamPrimeraparte + miBuffer.delantero+1] = '\0';

    }
}