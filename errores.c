#include <stdio.h>

#include "errores.h"


// Función que dado un código de erro imprime por pantalla unha mensaxe concreta
void imprimirError(int codigo) {
    switch (codigo) {
        case ERROR_ABRIR_ARCHIVO:
            fprintf(stderr, "\nError: No se ha encontrado el fichero especificado\n\n");
            break;
        case TAM_LEXEMA_EXCEDIDO:
            fprintf(stderr, "\nError: tamaño máximo de lexema excedido.\n\n");
            break;
        case COMENTARIO_MULTILINEA_NO_CERRADO:
            fprintf(stderr, "\nError: comentario multilinea no cerrado.\n\n");
            break;
     
    }
}