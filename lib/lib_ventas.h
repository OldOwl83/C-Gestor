#ifndef LIB_VENTAS_H_INCLUDED
#define LIB_VENTAS_H_INCLUDED

#include "definiciones.h"

//Funciones de ventas (lib_ventas.h)
unsigned int Agregar_nueva_venta();
                            //Gestiona el ingreso de una nueva venta a la base de datos.
                            //No recibe parámetros.
                            //Devuelve el ID atribuido a la nueva venta, o 0 si no se concretó el ingreso.

short Modificar_venta();
                            //Gestiona la modificación de los datos de ventas, o la eliminación de sus registros.
                            //No recibe parámetros.
                            //Devuelve 1 si la modificación/eliminación fue exitosa, o 0 en caso contrario.

size_t Buscar_venta_X_descripcion_apellido(unsigned int buffer_ids[], char cadena[LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE]);
                            //Busca la o las ventas cuya descripción o cuyo apellido del cliente presentan coincidencias con la
                            //cadena indicada.
                            //Parámetros:
                                //"buffer_ids": arreglo en el que se almacenarán los IDs de las ventas cuya descripción o apellido
                                //del cliente coincidan con los criterios de búsqueda.
                                //"cadena": cadena con la que se compararán la descripción y el apellido del cliente.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no la hubo.

short Calcular_precio_modificado_venta(unsigned int id_venta);
                            //Calcula la facturación neta correspondiente a una venta en base a su precio bruto y a las cateogrías
                            //a las que pertenece el cliente de la venta.
                            //Parámetro:
                                //"id_venta": ID de la venta para la que se quiere calcular la facturación.
                            //Devuelve 1 si la actualización de los datos fue exitosa, o 0 en caso contrario.

short Acreditar_pago(unsigned int id_venta, float pago);
                            //Modifica los datos correspondientes al pago de una venta y a los pagos totales del cliente asociado
                            //a esa venta.
                            //Parámetros:
                                //"id_venta": ID de la venta para la que se declara el pago.
                                //"pago": monto del pago.
                            //Devuelve 1 si los datos fueron modificados correctamente, o 0 en caso contrario.

short Introducir_pago();
                            //Gestiona la declaración de un pago por parte del usuario.
                            //No recibe parámetros.
                            //Devuelve 1 si las operaciones fueron realizadas con éxito, o 0 en caso contrario.

void Desplegar_ventas(unsigned int ids[], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con las ventas indicadas.
                            //Parámetros:
                                //"ids": arreglo con la lista de IDs de las ventas a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

unsigned int Pedir_venta();
                            //Gestiona la búsqueda de una venta particular por su ID, por palabras clave de su descripción o
                            //por el apellido del cliente asociado.
                            //Detecta automáticamente si se ingresó una cadena alfanumérica o un entero. En el primer caso,
                            //si se hallaron múltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe parámetros.
                            //Devuelve el ID de la venta hallada, o 0 si se renunció a la búsqueda.

#endif // LIB_VENTAS_H_INCLUDED
