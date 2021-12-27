#ifndef LIB_MODIFICADORES_H_INCLUDED
#define LIB_MODIFICADORES_H_INCLUDED

#include "definiciones.h"

//Funciones de gesti�n de los registros de modificadores de categor�as

//PROTOTIPOS

unsigned int Agregar_nuevo_modificador();
                            //Gestiona el ingreso de un nuevo modificador a la base de datos.
                            //No recibe par�metros.
                            //Devuelve el ID atribuido al nuevo modificador, o 0 si no se concret� el ingreso.

short Modificar_modificador();
                            //Gestiona la modificaci�n de los datos de modificador, o la eliminaci�n de sus registros.
                            //No recibe par�metros.
                            //Devuelve 1 si la modificaci�n/eliminaci�n fue exitosa, o 0 en caso contrario.

void Desplegar_modificadores(unsigned int ids[NRO_CATEGORIAS], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con los modificadores indicados.
                            //Par�metros:
                                //"ids": arreglo con la lista de IDs de los modificadores a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

unsigned int Buscar_modificador_X_nombre(unsigned int buffer_ids[NRO_MODIFICADORES], char nombre[LONGITUD_NOMBRE_CATEGORIA], pre_b precision);
                            //Busca el o los modificadores cuyo nombre coincide con el nombre indicado por par�metro.
                            //Si se solicita una coincidencia exacta, se obtendr� el primer nombre id�ntico al nombre
                            //indicado. Si se solicita una coincidencia parcial, de obtendr� la lista de todos los
                            //modificadores cuyo nombre comience con el nombre indicado.
                            //Par�metros:
                                //"buffer_ids": arreglo en el que se almacenar�n los IDs de los modificadores cuyo nombre
                                //coincida con los criterios de b�squeda.
                                //"nombre": cadena con la que se comparar�n los nombres de los modificadores registrados.
                                //"precision": indica si la coincidencia debe ser exacta o parcial, mediante el enum
                                //'precision_busqueda'.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no las hubo.

short Vincular_modificador_categoria(unsigned int id_modificador);
                            //Gestiona la vinculaci�n o desvinculaci�n de los modificadores con las categor�as disponibles.
                            //Par�metro:
                                //"id_modificador": ID del modificador a vincular o desvincular con una o m�s categor�as.
                            //Devuelve 1 si las operaciones solicitadas se realizaron con �xito, o 0 en caso contrario.

unsigned int Pedir_modificador();
                            //Gestiona la b�squeda de un modificador particular por NOMBRE o por ID en un mismo ingreso.
                            //Detecta autom�ticamente si se ingres� una cadena alfab�tica o un entero. En el primer caso,
                            //si se hallaron m�ltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe par�metros.
                            //Devuelve el ID del modificador hallado, o 0 si se renunci� a la b�squeda.

#endif // LIB_MODIFICADORES_H_INCLUDED
