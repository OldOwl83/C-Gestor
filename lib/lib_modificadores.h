#ifndef LIB_MODIFICADORES_H_INCLUDED
#define LIB_MODIFICADORES_H_INCLUDED

#include "definiciones.h"

//Funciones de gestión de los registros de modificadores de categorías

//PROTOTIPOS

unsigned int Agregar_nuevo_modificador();
                            //Gestiona el ingreso de un nuevo modificador a la base de datos.
                            //No recibe parámetros.
                            //Devuelve el ID atribuido al nuevo modificador, o 0 si no se concretó el ingreso.

short Modificar_modificador();
                            //Gestiona la modificación de los datos de modificador, o la eliminación de sus registros.
                            //No recibe parámetros.
                            //Devuelve 1 si la modificación/eliminación fue exitosa, o 0 en caso contrario.

void Desplegar_modificadores(unsigned int ids[NRO_CATEGORIAS], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con los modificadores indicados.
                            //Parámetros:
                                //"ids": arreglo con la lista de IDs de los modificadores a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

unsigned int Buscar_modificador_X_nombre(unsigned int buffer_ids[NRO_MODIFICADORES], char nombre[LONGITUD_NOMBRE_CATEGORIA], pre_b precision);
                            //Busca el o los modificadores cuyo nombre coincide con el nombre indicado por parámetro.
                            //Si se solicita una coincidencia exacta, se obtendrá el primer nombre idéntico al nombre
                            //indicado. Si se solicita una coincidencia parcial, de obtendrá la lista de todos los
                            //modificadores cuyo nombre comience con el nombre indicado.
                            //Parámetros:
                                //"buffer_ids": arreglo en el que se almacenarán los IDs de los modificadores cuyo nombre
                                //coincida con los criterios de búsqueda.
                                //"nombre": cadena con la que se compararán los nombres de los modificadores registrados.
                                //"precision": indica si la coincidencia debe ser exacta o parcial, mediante el enum
                                //'precision_busqueda'.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no las hubo.

short Vincular_modificador_categoria(unsigned int id_modificador);
                            //Gestiona la vinculación o desvinculación de los modificadores con las categorías disponibles.
                            //Parámetro:
                                //"id_modificador": ID del modificador a vincular o desvincular con una o más categorías.
                            //Devuelve 1 si las operaciones solicitadas se realizaron con éxito, o 0 en caso contrario.

unsigned int Pedir_modificador();
                            //Gestiona la búsqueda de un modificador particular por NOMBRE o por ID en un mismo ingreso.
                            //Detecta automáticamente si se ingresó una cadena alfabética o un entero. En el primer caso,
                            //si se hallaron múltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe parámetros.
                            //Devuelve el ID del modificador hallado, o 0 si se renunció a la búsqueda.

#endif // LIB_MODIFICADORES_H_INCLUDED
