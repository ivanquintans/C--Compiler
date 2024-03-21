#include <stdio.h>
#include <string.h>
#include "TS.h"
#include "abb.h"

TABB tablaSimbolos;

//funcion encargada de inicializar la tabla de simbolos
void inicializarTablaSimbolos(){

    //hacemos un array de Componentes léxicos para añadir a la tabla de simbolos
    TIPOELEMENTO inicio[] = {

        {FALSE,"false"},{AWAIT,"await"},{ELSE,"else"},{IMPORT,"import"},
        {PASS,"pass"},{NONE,"none"},{BREAK,"break"},{EXCEPT,"except"},
        {IN,"in"},{RAISE,"raise"},{TRUE,"true"},{CLASS,"class"},
        {FINALLY,"finally"},{IS,"is"},{RETURN,"return"},{AND,"and"},
        {CONTINUE,"continue"},{FOR,"for"},{LAMBDA,"lambda"},{TRY,"try"},
        {AS,"as"},{DEF,"def"},{FROM,"from"},{NONLOCAL,"nonlocal"},
        {WHILE,"while"},{ASSERT,"assert"},{DEL,"del"},{GLOBAL,"global"},
        {NOT,"not"},{WITH,"with"},{ASYNC,"async"},{ELIF,"elif"},
        {IF,"if"},{OR,"or"},{YIELD,"yield"},
    };

    //creamos el arbol binario

    crearAbb(&tablaSimbolos);
    
    //rellenamos la tabla de simbolos con el tamaño total de la lista entre lo que ocupa un elemento
    for (int i = 0; i < sizeof(inicio)/sizeof(TIPOELEMENTO); i++){
        //añadimos en el arbol cada uno de los keywords
        insertarElementoAbb(&tablaSimbolos,inicio[i]);
    }

    imprimirTablaSimbolos();
    
}

void imprimirTablaSimbolos(){
    printf("\n------ TABLA DE SIMBOLOS ------\n\n");
    //funcion proporcionada por mariajose
    inorden(tablaSimbolos);
    printf("\n---- FIN TABLA DE SIMBOLOS ----\n");

}

void finalizarTablaSimbolos(){
    //imprimimos la tabla de simbolos
    imprimirTablaSimbolos();
    //destruimos el arbol y la tabla de simbolos
    destruirAbb(&tablaSimbolos);
}

void insertarTablaSimbolos(compLexico *compLex){
    //esta funcion busca en el arbol, si se encuentra el elemento con la clave dada
    //en nuestro caso la clave es el componente léxico
    
    //creamos un auxiliar de busqueda
    TIPOELEMENTO aux = {0, NULL};
    buscarNodoAbb(tablaSimbolos,compLex->lexema, &aux);

    /*Ahora si el auxiliar ya existe pues lo devolvemos,
     si no pues lo añadimos a la tabla de simbolos*/
     if (aux.lexema != NULL){
        compLex->codigo = aux.codigo;
     }else{
        //tenemos que añadirlo como un identificador 
        compLex->codigo = ID;
        //insertamos el elemnto en el arbol
        insertarElementoAbb(&tablaSimbolos,*compLex);

     }

}

