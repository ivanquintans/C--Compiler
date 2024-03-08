#include <stdio.h>
#include <stdlib.h>

#include "sintactico.h"
#include "sistemaEntrada.h"

int main(int argc, char **argv)
{
  // comprobamos que el archivo que se pasa por linea de comandos sea valido

  if (argc != 2)
  {
    printf("Error en los parametros de entrada. El comando debe de ser ./main codigoFuente(Fichero con el codigo fuente deseado)\n");
    exit(EXIT_FAILURE);
  }

  //inicializamos el sistema de entrada
  iniciarSistemaEntrada();


  exit(0);
}