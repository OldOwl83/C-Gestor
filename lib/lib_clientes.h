#ifndef LIB_CLIENTES_H_INCLUDED
#define LIB_CLIENTES_H_INCLUDED

#include "definiciones.h"

//Funciones de gesti�n de los registros de clientes

//PROTOTIPOS

unsigned int Agregar_nuevo_cliente();
                            //Gestiona el ingreso de un nuevo cliente a la base de datos.
                            //No recibe par�metros.
                            //Devuelve el ID atribuido al nuevo cliente, o 0 si no se concret� el ingreso.

short Modificar_cliente();
                            //Gestiona la modificaci�n de los datos de clientes, o la eliminaci�n de sus registros.
                            //No recibe par�metros.
                            //Devuelve 1 si la modificaci�n/eliminaci�n fue exitosa, o 0 en caso contrario.

void Desplegar_clientes(unsigned int ids[], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con los clientes indicados.
                            //Par�metros:
                                //"ids": arreglo con la lista de IDs de los clientes a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

size_t Buscar_cliente_X_apellido(unsigned int buffer_ids[], char apellido[LONGITUD_APELLIDO_CLIENTE], pre_b precision);
                            //Busca el o los clientes cuyo apellido coincide con el nombre indicado. Si se solicita una
                            //coincidencia exacta, se obtendr� el primer apellido id�ntico al nombre indicado. Si se
                            //solicita una coincidencia parcial, de obtendr� la lista de todos los clientes cuyo
                            //apellido comience con el nombre indicado.
                            //Par�metros:
                                //"buffer_ids": arreglo en el que se almacenar�n los IDs de los clientes cuyo apellido
                                //coincida con los criterios de b�squeda.
                                //"apellido": cadena con la que se comparar�n los apellidos de los clientes registrados.
                                //"precision": indica si la coincidencia debe ser exacta o parcial, mediante el enum
                                //precision_busqueda.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no la hubo.

short Vincular_cliente_categoria(unsigned int id_cliente);
                            //Gestiona la vinculaci�n o desvinculaci�n de los clientes con las categor�as disponibles.
                            //Par�metro:
                                //"id_cliente": ID del cliente a vincular o desvincular con una o m�s categor�as.
                            //Devuelve 1 si las operaciones solicitadas se realizaron con �xito, o 0 en caso contrario.

unsigned int Pedir_cliente();
                            //Gestiona la b�squeda de un cliente particular por APELLIDO o por ID en un mismo ingreso.
                            //Detecta autom�ticamente si se ingres� una cadena alfab�tica o un entero. En el primer caso,
                            //si se hallaron m�ltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe par�metros.
                            //Devuelve el ID del cliente hallado, o 0 si se renunci� a la b�squeda.

#endif // LIB_CLIENTES_H_INCLUDED
