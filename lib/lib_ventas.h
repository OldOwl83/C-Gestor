#ifndef LIB_VENTAS_H_INCLUDED
#define LIB_VENTAS_H_INCLUDED

#include "definiciones.h"

//Funciones de ventas (lib_ventas.h)
unsigned int Agregar_nueva_venta();
                            //Gestiona el ingreso de una nueva venta a la base de datos.
                            //No recibe par�metros.
                            //Devuelve el ID atribuido a la nueva venta, o 0 si no se concret� el ingreso.

short Modificar_venta();
                            //Gestiona la modificaci�n de los datos de ventas, o la eliminaci�n de sus registros.
                            //No recibe par�metros.
                            //Devuelve 1 si la modificaci�n/eliminaci�n fue exitosa, o 0 en caso contrario.

size_t Buscar_venta_X_descripcion_apellido(unsigned int buffer_ids[], char cadena[LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE]);
                            //Busca la o las ventas cuya descripci�n o cuyo apellido del cliente presentan coincidencias con la
                            //cadena indicada.
                            //Par�metros:
                                //"buffer_ids": arreglo en el que se almacenar�n los IDs de las ventas cuya descripci�n o apellido
                                //del cliente coincidan con los criterios de b�squeda.
                                //"cadena": cadena con la que se comparar�n la descripci�n y el apellido del cliente.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no la hubo.

short Calcular_precio_modificado_venta(unsigned int id_venta);
                            //Calcula la facturaci�n neta correspondiente a una venta en base a su precio bruto y a las cateogr�as
                            //a las que pertenece el cliente de la venta.
                            //Par�metro:
                                //"id_venta": ID de la venta para la que se quiere calcular la facturaci�n.
                            //Devuelve 1 si la actualizaci�n de los datos fue exitosa, o 0 en caso contrario.

short Acreditar_pago(unsigned int id_venta, float pago);
                            //Modifica los datos correspondientes al pago de una venta y a los pagos totales del cliente asociado
                            //a esa venta.
                            //Par�metros:
                                //"id_venta": ID de la venta para la que se declara el pago.
                                //"pago": monto del pago.
                            //Devuelve 1 si los datos fueron modificados correctamente, o 0 en caso contrario.

short Introducir_pago();
                            //Gestiona la declaraci�n de un pago por parte del usuario.
                            //No recibe par�metros.
                            //Devuelve 1 si las operaciones fueron realizadas con �xito, o 0 en caso contrario.

void Desplegar_ventas(unsigned int ids[], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con las ventas indicadas.
                            //Par�metros:
                                //"ids": arreglo con la lista de IDs de las ventas a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

unsigned int Pedir_venta();
                            //Gestiona la b�squeda de una venta particular por su ID, por palabras clave de su descripci�n o
                            //por el apellido del cliente asociado.
                            //Detecta autom�ticamente si se ingres� una cadena alfanum�rica o un entero. En el primer caso,
                            //si se hallaron m�ltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe par�metros.
                            //Devuelve el ID de la venta hallada, o 0 si se renunci� a la b�squeda.

#endif // LIB_VENTAS_H_INCLUDED
