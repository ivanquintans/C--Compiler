#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistemaEntrada.h"
FILE *archivo;
char caracter_actual;

// funcion que inicializa el sistema de entrada

void iniciarSistemaEntrada(char *fichero)
{
  printf(fichero);
  archivo = fopen(fichero, "r");

  // comprobamos que este se abre de forma correcta
  if (archivo == NULL){
    printf("Error a  la hora de abrir er archivo\n");
    exit(EXIT_FAILURE);
  }
}

// funcion que se encarga de cerrar el archivo
void finalizarSistemaEntrada(){
  fclose(archivo);
}

// funcion que se encarga de enviar un caracter al analizadorlexico
char sigCaracter(){
  // Lee un caracter del archivo
  caracter_actual = fgetc(archivo);
  //if (caracter_actual == NULL){
   // printf("Error al leer el caracter\n");
  //}else{
    return caracter_actual;
 // }
}
