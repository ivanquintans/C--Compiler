#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"

// Función que acepta o lexema que se está lendo actualmente
void aceptarLexema(compLexico *comp);

//funcion que inicializa el sistema de entrada
void iniciarSistemaEntrada();

//funcion que devuelve el siguiente caracter al lexico
char sigCaracter();

//funcion que se encarga de devolver el ultimo caracter que aceptó la cadena
void retroceder();

//funcion que se encarga de colocar bien los punteros una vez le llega un caracter como /n o /t o espacios en blanco
void saltarCaracter();

//funcion que se encarga de colocar bien los punteros una vez se salta un lexema
void saltarLexema();

//funcion auxiliar para mostrar la posicion actual de delantero y de inicio
void mostrarInicioYDelantero();

