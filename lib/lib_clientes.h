#ifndef LIB_CLIENTES_H_INCLUDED
#define LIB_CLIENTES_H_INCLUDED

#include "definiciones.h"

//Funciones de gestión de los registros de clientes

//PROTOTIPOS

unsigned int Agregar_nuevo_cliente();
                            //Gestiona el ingreso de un nuevo cliente a la base de datos.
                            //No recibe parámetros.
                            //Devuelve el ID atribuido al nuevo cliente, o 0 si no se concretó el ingreso.

short Modificar_cliente();
                            //Gestiona la modificación de los datos de clientes, o la eliminación de sus registros.
                            //No recibe parámetros.
                            //Devuelve 1 si la modificación/eliminación fue exitosa, o 0 en caso contrario.

void Desplegar_clientes(unsigned int ids[], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con los clientes indicados.
                            //Parámetros:
                                //"ids": arreglo con la lista de IDs de los clientes a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

size_t Buscar_cliente_X_apellido(unsigned int buffer_ids[], char apellido[LONGITUD_APELLIDO_CLIENTE], pre_b precision);
                            //Busca el o los clientes cuyo apellido coincide con el nombre indicado. Si se solicita una
                            //coincidencia exacta, se obtendrá el primer apellido idéntico al nombre indicado. Si se
                            //solicita una coincidencia parcial, de obtendrá la lista de todos los clientes cuyo
                            //apellido comience con el nombre indicado.
                            //Parámetros:
                                //"buffer_ids": arreglo en el que se almacenarán los IDs de los clientes cuyo apellido
                                //coincida con los criterios de búsqueda.
                                //"apellido": cadena con la que se compararán los apellidos de los clientes registrados.
                                //"precision": indica si la coincidencia debe ser exacta o parcial, mediante el enum
                                //precision_busqueda.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no la hubo.

short Vincular_cliente_categoria(unsigned int id_cliente);
                            //Gestiona la vinculación o desvinculación de los clientes con las categorías disponibles.
                            //Parámetro:
                                //"id_cliente": ID del cliente a vincular o desvincular con una o más categorías.
                            //Devuelve 1 si las operaciones solicitadas se realizaron con éxito, o 0 en caso contrario.

unsigned int Pedir_cliente();
                            //Gestiona la búsqueda de un cliente particular por APELLIDO o por ID en un mismo ingreso.
                            //Detecta automáticamente si se ingresó una cadena alfabética o un entero. En el primer caso,
                            //si se hallaron múltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe parámetros.
                            //Devuelve el ID del cliente hallado, o 0 si se renunció a la búsqueda.

#endif // LIB_CLIENTES_H_INCLUDED
