#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"



/**
 * Función que se encarga de inicializar el sistema de entrada
 * 1- Abre el fichero a analizar
 * 2- Inicializa los valores del buffer
 * 3- Carga el primer bloque
 */
void iniciarSistemaEntrada();

/**
 * Función que se encargade proporcionar un caracter que es pedido por el lexico.c
 * Ademas controla de una forma adecuada el posicionamiento de los lexemas en ambos buffers
 */
char sigCaracter();

/**
 * 
 * Función que se encarga de retroceder el puntero del caracter a leer
 * Esto puede ser dado porque la cadena se acepta en el siguiente caracter y es necesario procesar individualmente el caracter 
 */
void retroceder();


/**
 * Función que se encarga de colocar bien los punteros una vez le llega un caracter como /n o /t o espacios en blanco
 */
void saltarCaracter();

/**
 * Función que se encarga de colocar bien los punteros una vez se acepto un lexema y es necesario saltarlo
 */
void saltarLexema();

/**
 * Funcion que se encarga de aceptar el lexema, que sera finalmente finalmente enviado como comp al analizador sintactico
 * @param compActual componente que se devolverá en el analizador léxico.
 * @param identificador tiene valor de cero o 1. Su valor sera uno cuando el lexema a aceptar sea un identificador.
 */

void aceptarLexema(compLexico *compActual, int identificador);

void finalizarSistemaEntrada(); 



