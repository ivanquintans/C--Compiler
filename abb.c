#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include "definiciones.h"

/* Se empleo el arbol binario de busqueda proporcionado por Maria Jose en la 
asignatura de algoritmos
Sin embargo se realizo un pequeño ajuste sobre este cambiando lo 
necesario para poder implementarlo en la tabla de simbolos*/

///////////////////////// ESTRUCTURAS DE DATOS

struct celda {
    TIPOELEMENTO info;
    struct celda *izq, *der;
};

//////////////////////// FUNCIONES


/*Extraer la clave de una celda */ //Si la clave es orden alfabético
TIPOCLAVE _clave_elem(TIPOELEMENTO *E) {
    return E->lexema;
}

/* Esta funcion puente nos permite modificar el tipo de
 de datos del TAD sin tener que cambiar todas las 
 comparaciones del resto de la biblioteca y en su lugar
 cambiando solo esta. */

int _comparar_claves(TIPOCLAVE cl1, TIPOCLAVE cl2) {
    if (strcmp(cl1,cl2)>0) return 1;
    else if (strcmp(cl1,cl2)<0) return -1;
    else return 0;
    
}

/////////////////////////// FIN PARTE MODIFICABLE
/////////////////////////////////////////////////////////////

//OPERACIONES DE CREACIÓN Y DESTRUCCIÓN

void crearAbb(TABB *A) {
    *A = NULL;
}

void destruirAbb(TABB *A) {
    if (*A != NULL) {
        destruirAbb(&(*A)->izq);
        destruirAbb(&(*A)->der);
        free(((*A)->info.lexema));
        free(*A);
        *A = NULL;
    }
}

//OPERACIONES DE INFORMACIÓN

unsigned esAbbVacio(TABB A) {
    return A == NULL;
}

TABB izqAbb(TABB A) {
    return A->izq;
}

TABB derAbb(TABB A) {
    return A->der;
}

void leerElementoAbb(TABB A, TIPOELEMENTO *E) {
    *E = A->info;
}
// Función privada para comparar las claves

int _comparar_clave_elem(TIPOCLAVE cl, TIPOELEMENTO E) {
    return _comparar_claves(cl, _clave_elem(&E));
}
//Función privada para informar si una clave está en el árbol

unsigned _es_miembro_clave(TABB A, TIPOCLAVE cl) {
    if (esAbbVacio(A)) {
        return 0;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    if (comp > 0) { //cl > A->info
        return _es_miembro_clave(derAbb(A), cl);
    }
    //cl < A->info
    return _es_miembro_clave(izqAbb(A), cl);
}

//Funciones públicas

unsigned esMiembroAbb(TABB A, TIPOELEMENTO E) {
    return _es_miembro_clave(A, _clave_elem(&E));
}

void buscarNodoAbb(TABB A, TIPOCLAVE cl, TIPOELEMENTO *nodo) {
    if (esAbbVacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) { // cl < A->info
        buscarNodoAbb(A->izq, cl, nodo);
    } else { // cl > A->info
        buscarNodoAbb(A->der, cl, nodo);
    }
}
//OPERACIONES DE MODIFICACIÓN

/* Funcion recursiva para insertar un nuevo nodo 
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void insertarElementoAbb(TABB *A, TIPOELEMENTO E) {
    if (esAbbVacio(*A)) {
        *A = malloc(sizeof (struct celda));
        (*A)->info.codigo = E.codigo;
        (*A)->info.lexema = malloc((strlen(E.lexema)));
        strcpy((*A)->info.lexema, E.lexema);
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }
    TIPOCLAVE cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertarElementoAbb(&(*A)->der, E);
    } else {
        insertarElementoAbb(&(*A)->izq, E);
    }
}

void inorden(TABB A){
    TIPOELEMENTO E;
    if(!esAbbVacio(A)){
        inorden(izqAbb(A));
        leerElementoAbb(A,&E);
        printf("< %d , %s >\n",E.codigo, E.lexema);
        inorden(derAbb(A));
    }
}


