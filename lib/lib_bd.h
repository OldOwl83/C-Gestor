#ifndef LIB_BD_H_INCLUDED
#define LIB_BD_H_INCLUDED

#include "definiciones.h"

//Funciones de manejo de la base de datos (CRUD y consultas)

//PROTOTIPOS

unsigned int Insertar_registro(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], void *registro);
                            //Graba el nuevo registro pasado por parámetros, tanto en el archivo indicado, como
                            //en su buffer en memoria principal.
                            //Parámetros:
                                //"nombre_archivo": nombre del archivo correspondiente a la tabla que se quiere modificar.
                                //"registro": dirección de la estructura donde se almacenan los datos del registro a ingresar.
                            //Retorna el número de ID asignado al nuevo registro, o 0 en caso de fallo.

short Eliminar_registro(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], unsigned int id_registro);
                            //Elimina el registro pasado por parámetros, tanto del archivo indicado como de su buffer correspondiente.
                            //Parámetros:
                                //"nombre_archivo": nombre del archivo correspondiente a la tabla que se quiere modificar.
                                //"registro": ID del registro que se quiere eliminar.
                            //Retorna 1 si el registro fue eliminado correctamente, o 0 en caso contrario.

short Modificar_dato(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], size_t desplazamiento_dato, void *dato, size_t tamanio_dato);
                            //Modifica un dato particular dentro de algún registro sin especificar de la tabla correspondiente al
                            //archivo indicado. Para ello, el cliente de la función debe detectar el desplazamiento del dato dentro
                            //del archivo a modificar (y que es equivalente al  desplazamiento dentro del buffer).
                            //Parámetros:
                                //"nombre_archivo": nombre del archivo correspondiente a la tabla que se quiere modificar.
                                //"desplazamiento_dato": el offset, medido en bytes, de la dirección del dato respecto de la del
                                //comienzo del buffer a modificar (el desplazamiento dentro del archivo es el mismo). Puede calcularse
                                //fácilmente restando a un puntero que contiene la dirección del dato, otro puntero que contiene la
                                //dirección del buffer.
                                //"dato": el valor del dato a escribir.
                                //"tamanio_dato": el tamaño correspondiente al tipo de dato a modificar (no al struct del registro modificado).
                            //Devuelve 1 si la modificación fue exitosa (tanto en el archivo como en el buffer), o 0 en caso contrario.

short Alternar_vinculo_categoria_modificador(unsigned int id_categoria, unsigned int id_modificador);
                            //Detecta si la categoría indicada está o no asociada al modificador indicado, e invierte su relación,
                            //tanto en el archivo de categorías, como en su buffer en memoria.
                            //Parámetros:
                                //"id_categoria": ID de la categoría a modificar.
                                //"id_modificador": ID del modificador a vincular o desvincular de la categoría indicada en
                                //"id_categoria".
                            //Devuelve 1 si la operación fue exitosa, o 0 en caso contrario.

short Alternar_vinculo_cliente_categoria(unsigned int id_cliente, unsigned int id_categoria);
                            //Detecta si el cliente indicada está o no asociado a la categoría indicada, e invierte su relación,
                            //tanto en el archivo de clientes, como en su buffer en memoria.
                            //Parámetros:
                                //"id_cliente": ID del cliente a modificar.
                                //"id_categoria": ID de la categoría a vincular o desvincular del cliente indicado en
                                //"id_cliente".
                            //Devuelve 1 si la operación fue exitosa, o 0 en caso contrario.

void Consulta_clientes_X_facturacion();
                            //Despliega la lista de clientes cuya facturación total está por encima o por debajo del límite
                            //solicitado al usuario (si el usuario indica un límite negativo, está estableciendo un techo,
                            //si no, un piso).

void Consulta_clientes_X_deuda();
                            //Despliega la lista de clientes cuya deuda total está por encima o por debajo del límite
                            //solicitado al usuario (si el usuario indica un límite negativo, está estableciendo un techo,
                            //si no, un piso).

void Consulta_ventas_X_cliente();
                            //Despliega la lista de ventas correspondientes al cliente solicitado al usuario.

void Consulta_ventas_X_montoNeto();
                            //Despliega la lista de ventas cuya facturación bruta está por encima o por debajo del límite
                            //solicitado al usuario (si el usuario indica un límite negativo, está estableciendo un techo,
                            //si no, un piso).

void Consulta_ventas_X_montoBase();
                            //Despliega la lista de ventas cuya facturación neta está por encima o por debajo del límite
                            //solicitado al usuario (si el usuario indica un límite negativo, está estableciendo un techo,
                            //si no, un piso).

void Facturacion_total();
                            //Despliega los montos totales facturados, tanto brutos como netos.

void Facturacion_total_X_categoria();
                            //Despliega la facturación total, tanto bruta como neta, de todos los clientes correspondientes
                            //a la categoría solicitada al usuario.

void Calcular_deuda_total();
                            //Despliga la deuda total de todos los clientes registrados.

#endif // LIB_BD_H_INCLUDED
