#ifndef LIB_BD_H_INCLUDED
#define LIB_BD_H_INCLUDED

#include "definiciones.h"

//Funciones de manejo de la base de datos (CRUD y consultas)

//PROTOTIPOS

unsigned int Insertar_registro(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], void *registro);
                            //Graba el nuevo registro pasado por par�metros, tanto en el archivo indicado, como
                            //en su buffer en memoria principal.
                            //Par�metros:
                                //"nombre_archivo": nombre del archivo correspondiente a la tabla que se quiere modificar.
                                //"registro": direcci�n de la estructura donde se almacenan los datos del registro a ingresar.
                            //Retorna el n�mero de ID asignado al nuevo registro, o 0 en caso de fallo.

short Eliminar_registro(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], unsigned int id_registro);
                            //Elimina el registro pasado por par�metros, tanto del archivo indicado como de su buffer correspondiente.
                            //Par�metros:
                                //"nombre_archivo": nombre del archivo correspondiente a la tabla que se quiere modificar.
                                //"registro": ID del registro que se quiere eliminar.
                            //Retorna 1 si el registro fue eliminado correctamente, o 0 en caso contrario.

short Modificar_dato(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], size_t desplazamiento_dato, void *dato, size_t tamanio_dato);
                            //Modifica un dato particular dentro de alg�n registro sin especificar de la tabla correspondiente al
                            //archivo indicado. Para ello, el cliente de la funci�n debe detectar el desplazamiento del dato dentro
                            //del archivo a modificar (y que es equivalente al  desplazamiento dentro del buffer).
                            //Par�metros:
                                //"nombre_archivo": nombre del archivo correspondiente a la tabla que se quiere modificar.
                                //"desplazamiento_dato": el offset, medido en bytes, de la direcci�n del dato respecto de la del
                                //comienzo del buffer a modificar (el desplazamiento dentro del archivo es el mismo). Puede calcularse
                                //f�cilmente restando a un puntero que contiene la direcci�n del dato, otro puntero que contiene la
                                //direcci�n del buffer.
                                //"dato": el valor del dato a escribir.
                                //"tamanio_dato": el tama�o correspondiente al tipo de dato a modificar (no al struct del registro modificado).
                            //Devuelve 1 si la modificaci�n fue exitosa (tanto en el archivo como en el buffer), o 0 en caso contrario.

short Alternar_vinculo_categoria_modificador(unsigned int id_categoria, unsigned int id_modificador);
                            //Detecta si la categor�a indicada est� o no asociada al modificador indicado, e invierte su relaci�n,
                            //tanto en el archivo de categor�as, como en su buffer en memoria.
                            //Par�metros:
                                //"id_categoria": ID de la categor�a a modificar.
                                //"id_modificador": ID del modificador a vincular o desvincular de la categor�a indicada en
                                //"id_categoria".
                            //Devuelve 1 si la operaci�n fue exitosa, o 0 en caso contrario.

short Alternar_vinculo_cliente_categoria(unsigned int id_cliente, unsigned int id_categoria);
                            //Detecta si el cliente indicada est� o no asociado a la categor�a indicada, e invierte su relaci�n,
                            //tanto en el archivo de clientes, como en su buffer en memoria.
                            //Par�metros:
                                //"id_cliente": ID del cliente a modificar.
                                //"id_categoria": ID de la categor�a a vincular o desvincular del cliente indicado en
                                //"id_cliente".
                            //Devuelve 1 si la operaci�n fue exitosa, o 0 en caso contrario.

void Consulta_clientes_X_facturacion();
                            //Despliega la lista de clientes cuya facturaci�n total est� por encima o por debajo del l�mite
                            //solicitado al usuario (si el usuario indica un l�mite negativo, est� estableciendo un techo,
                            //si no, un piso).

void Consulta_clientes_X_deuda();
                            //Despliega la lista de clientes cuya deuda total est� por encima o por debajo del l�mite
                            //solicitado al usuario (si el usuario indica un l�mite negativo, est� estableciendo un techo,
                            //si no, un piso).

void Consulta_ventas_X_cliente();
                            //Despliega la lista de ventas correspondientes al cliente solicitado al usuario.

void Consulta_ventas_X_montoNeto();
                            //Despliega la lista de ventas cuya facturaci�n bruta est� por encima o por debajo del l�mite
                            //solicitado al usuario (si el usuario indica un l�mite negativo, est� estableciendo un techo,
                            //si no, un piso).

void Consulta_ventas_X_montoBase();
                            //Despliega la lista de ventas cuya facturaci�n neta est� por encima o por debajo del l�mite
                            //solicitado al usuario (si el usuario indica un l�mite negativo, est� estableciendo un techo,
                            //si no, un piso).

void Facturacion_total();
                            //Despliega los montos totales facturados, tanto brutos como netos.

void Facturacion_total_X_categoria();
                            //Despliega la facturaci�n total, tanto bruta como neta, de todos los clientes correspondientes
                            //a la categor�a solicitada al usuario.

void Calcular_deuda_total();
                            //Despliga la deuda total de todos los clientes registrados.

#endif // LIB_BD_H_INCLUDED
