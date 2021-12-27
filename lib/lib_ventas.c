#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "definiciones.h"

#include "lib_ventas.h"
#include "lib_clientes.h"
#include "lib_bd.h"
#include "lib_auxiliares.h"

unsigned int Agregar_nueva_venta()
{
    ven nueva_venta = {0, 0, "", 0.00, 0.00, 0.00}; // buffer para grabar los datos de la nueva venta

    unsigned int id_agregado;//Toma el ID de la nueva venta

    short ingreso;

    char buffer_cartel[220], confirmacion;

    float pago;


//PIDE EL CLIENTE ASOCIADO A LA VENTA

    BORRAR_PANTALLA

    sprintf(buffer_cartel, "%s", "INGRESO DE NUEVA VENTA");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

//PIDE CLIENTE
    sprintf(buffer_cartel, "%s", "CLIENTE ASOCIADO A LA NUEVA VENTA");
    Imprime_cartel(buffer_cartel, ENCABEZADO);

    nueva_venta.cliente = Pedir_cliente();

    BORRAR_PANTALLA

    sprintf(buffer_cartel, "%s", "CLIENTE SELECCIONADO");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    Desplegar_clientes(&nueva_venta.cliente, 1);

//PIDE DESCRIPCION de la VENTA

    sprintf(buffer_cartel, "%s%c%s%c%s%d%s", "Ingrese la descripci", 162, "n de la venta (m", 160, "ximo ", LONGITUD_DESCRIPCION_VENTAS - 2,
                            " caracteres), o 0 para saltar este paso.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        nueva_venta.descripcion[LONGITUD_DESCRIPCION_VENTAS - 1] = 'a'; //Anulo la condición de permanencia en el ciclo.

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_DESCRIPCION_VENTAS - 1, "[^\n]");
        scanf(buffer_cartel, nueva_venta.descripcion);
        fflush(stdin);

        if(!strcmp(nueva_venta.descripcion, "0"))
            nueva_venta.descripcion[0] = '\0';

        else if(nueva_venta.descripcion[LONGITUD_DESCRIPCION_VENTAS - 1] == '\0')
        {
            sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ha excedido la extensi", 162, "n permitida para este campo (", LONGITUD_DESCRIPCION_VENTAS - 2, " caracteres)."
                                                    "Reduzca la entrada o solicite al programador una ampliaci", 162, "n del l", 161, "mite.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            nueva_venta.descripcion[0] = '\0';
        }
    }while(nueva_venta.descripcion[LONGITUD_DESCRIPCION_VENTAS - 1] == '\0');

//SOLICITA EL MONTO BRUTO DE LA VENTA

    sprintf(buffer_cartel, "%s%c%s%c%s", "Ingrese el MONTO BRUTO de la nueva venta,"
                            " o 0 para volver al men", 163, " de gesti", 162, "n de ventas.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &nueva_venta.precio_base);
        fflush(stdin);

        if(!ingreso || nueva_venta.precio_base < 0)
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real positivo.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n");

        }else if(nueva_venta.precio_base == 0)
            return 0;

    }while(!ingreso || nueva_venta.precio_base < 0);

//INTENTA EL REGISTRO DE LA VENTA
    if((id_agregado = Insertar_registro(ARCHIVO_VENTAS, &nueva_venta)))
    {
        if(Calcular_precio_modificado_venta(id_agregado))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "La venta fue registrada exitosamente en la base de datos y su precio final"
                                                    " calculado y sumado a la facturaci", 162, "n total del cliente correspondiente. ", 168,
                                                    "Desea ingresar alg", 163, "n pago relativo a esta venta? (s/n)");

            printf("\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);

        //Eventual ingreso de un pago asociado a la venta
            do
            {
                PEDIR_INGRESO

                ingreso = scanf("%c", &confirmacion);
                fflush(stdin);

                confirmacion = tolower(confirmacion);

                if(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'))
                {
                    BORRAR_PANTALLA

                    sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.", 168,
                                                    "Desea ingresar alg", 163, "n pago relativo a esta venta? (s/n)");

                    printf("\n\n\n\n\n\n\n");
                    Imprime_cartel(buffer_cartel, SIMPLE);
                }

            }while(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'));

            if(confirmacion == 's')
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s", "Ingrese el PAGO correspondiente a la venta ingresada,"
                                        " o 0 para saltearse este paso.");

                Imprime_cartel(buffer_cartel, DOBLE);

                do
                {
                    PEDIR_INGRESO

                    ingreso = scanf("%f", &pago);
                    fflush(stdin);

                    if(pago == 0 && ingreso)
                        return 0;

                    if(!ingreso || pago < 0)
                    {
                        sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real positivo.");

                        printf("\n\n\n\n\n\n\n");
                        Imprime_cartel(buffer_cartel, SIMPLE);
                        printf("\n\n");
                    }

                }while(!ingreso || pago < 0);

                if(Acreditar_pago(id_agregado, pago))
                {
                    sprintf(buffer_cartel, "%s", "El pago fue registrado exitosamente y los datos de la venta y del cliente correctamente actualizados.");

                    printf("\n\n\n\n");
                    Imprime_cartel(buffer_cartel, DOBLE);
                    printf("\n\n\n\n");

                    PAUSA
                }
            }

            return id_agregado; //Devuelve el ID atribuido a la nueva venta.

        }else if(Eliminar_registro(ARCHIVO_VENTAS, id_agregado)) //Si hubo errores en el cálculo de la facturación, se elimina el registro.
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s%s%s", "Hubo un error en la actualizaci", 162, "n del precio neto de la venta o de la"
                                                "facturaci", 162, "n total del cliente ", buffer_clientes[nueva_venta.cliente].apellido, ", de modo que la venta no fue registrada.");

            printf("\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);

            return 0;
        }else
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "Hubo un error en el registro de la venta. Por favor, revise el estado actual de la base de datos.");

            printf("\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);

            return 0;
        }
    }else
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "No se pudo registrar la venta correctamente. Revise el estado actual de la base de datos.");

        printf("\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);

        return 0;
    }
}

//===========================================================================================================

short Modificar_venta()
{
    char registro_ingresado[LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE];

    unsigned int opcion, id_modificar;

    char buffer_cartel[LONGITUD_DESCRIPCION_VENTAS < 280 ? 280 : LONGITUD_DESCRIPCION_VENTAS];

    float nuevo_precio, pago;

    short ingreso;

    void *ptr_buffer = buffer_ventas, *ptr_dato; //Punteros para obtener el desplazamiento del dato a modificar en el archivo.

    sprintf(buffer_cartel, "%s", "MODIFICAR VENTA");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    id_modificar = Pedir_venta(); //Pide el ID, palabras clave de la descripción o el apellido del cliente asociado a la venta.

    do //Despliega las opciones de modificación
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "VENTA SELECCIONADA");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        Desplegar_ventas(&id_modificar, 1);

        sprintf(buffer_cartel, "%s", "MODIFICAR VENTA");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s%c%s%c%s", "Modificar descripci", 162, "n."
                                                    "Modificar precio base (ADVERTENCIA: el monto neto se actualizar", 160,
                                                    " autom", 160, "ticamente en base al estado actual de la categor", 161, "a del cliente)."
                                                    "Actualizar monto neto seg", 163, "n cambios en la categor", 161, "a del cliente."
                                                    "Descontar pago."
                                                    "Eliminar registro de la venta");

        Imprime_cartel(buffer_cartel, MENU);

        PEDIR_INGRESO

        ingreso = scanf("%d", &opcion);
        fflush(stdin);

        if(opcion < 0 || opcion > 5 || ingreso == 0)
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 5.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n\n");

            PAUSA
        }
    }while(opcion < 0 || opcion > 5 || ingreso == 0);

    switch(opcion)
    {
    case 1: //Opción "Modificar descripción"
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s", "DESCRIPCI", 162, "N ACTUAL DE LA VENTA"); //Muestra la vieja descripción, para tenerla de referencia.

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        sprintf(buffer_cartel, "%s", buffer_ventas[id_modificar].descripcion);

        Imprime_cartel(buffer_cartel, SIMPLE);

        sprintf(buffer_cartel, "%s%c%s%c%s%d%s%c%s", "Escriba la nueva descripci", 162, "n para esta venta (m", 160,
                                                        "ximo ", LONGITUD_DESCRIPCION_VENTAS - 2, " caracteres) o 0 para volver al men",
                                                        163, " anterior.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do //Pide la nueva descripción.
        {
            PEDIR_INGRESO

            registro_ingresado[LONGITUD_DESCRIPCION_VENTAS - 1] = 'a'; //Anulo la condición de permanencia en el ciclo.

            sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_DESCRIPCION_VENTAS - 1, "[^\n]");
            scanf(buffer_cartel, registro_ingresado);
            fflush(stdin);

            if(!strcmp(registro_ingresado, "0"))
                return 0;

            else if(registro_ingresado[LONGITUD_DESCRIPCION_VENTAS - 1] == '\0')
            {
                sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ha excedido la extensi", 162, "n permitida para este campo (",
                                                LONGITUD_DESCRIPCION_VENTAS - 2, " caracteres). Reduzca la entrada o solicite"
                                                " al programador una ampliaci", 162, "n del l", 161, "mite.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                registro_ingresado[0] = '\0';
            }
        }while(registro_ingresado[LONGITUD_DESCRIPCION_VENTAS - 1] == '\0');

        ptr_dato = &buffer_ventas[id_modificar].descripcion; //Apunta a la dirección del dato a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_VENTAS, (size_t)(ptr_dato - ptr_buffer), registro_ingresado, LONGITUD_DESCRIPCION_VENTAS))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "REGISTRO DE VENTA MODIFICADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_ventas(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "La venta seleccionada fue modificada exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 2: //Opción "Modificar precio base"
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s", "Ingrese el nuevo PRECIO BASE para la venta seleccionada,"
                            " o 0 para volver al men", 163, " anterior.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            ingreso = scanf("%f", &nuevo_precio);
            fflush(stdin);

            if(nuevo_precio == 0 && ingreso)
                return 0;

            if(!ingreso || nuevo_precio < 0)
            {
                sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real positivo.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n");
            }

        }while(!ingreso || nuevo_precio < 0);

        ptr_dato = &buffer_ventas[id_modificar].precio_base; //Apunta a la dirección del dato a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_VENTAS, (size_t)(ptr_dato - ptr_buffer), &nuevo_precio, sizeof(float)))
        {
            BORRAR_PANTALLA

            if(!Calcular_precio_modificado_venta(id_modificar))
            {
                sprintf(buffer_cartel, "%s%c%s", "Hubo un error en la actualizaci", 162, "n del monto neto. Por favor, verifique el estado del registro.");

                printf("\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n");
            }

            sprintf(buffer_cartel, "%s", "VENTA SELECCIONADA");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_ventas(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "La venta seleccionada fue modificada exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");

        }else
            return 0;

        break;
    case 3: //Opción "Actualizar monto neto" (pensado para los casos en que un cliente haya cambiado de categoría y se quiera actualizar el monto de una venta retrospectivamente)

        BORRAR_PANTALLA

        if(Calcular_precio_modificado_venta(id_modificar))
        {
            sprintf(buffer_cartel, "%s", "VENTA SELECCIONADA");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_ventas(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El monto neto de la venta fue actualizado correctamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
        {
            sprintf(buffer_cartel, "%s%c%s", "Hubo un error en la actualizaci", 162, "n del monto neto. Por favor, verifique el estado del registro.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n");
        }

        break;
    case 4: //Opción "Descontar pago"
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s", "Ingrese el monto a descontar en el pago de la venta seleccionada,"
                                " o 0 para volver al men", 163, " anterior.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            ingreso = scanf("%f", &pago);
            fflush(stdin);

            if(pago == 0 && ingreso)
                return 0;

            if(!ingreso || pago < 0)
            {
                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real positivo (el mismo se descontar", 160, ".");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n");
            }

        }while(!ingreso || pago < 0);

        if(Acreditar_pago(id_modificar, -pago)) //Anulación del pago
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "VENTA MODIFICADA");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_ventas(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El monto ingresado fue descontado exitosamente del pago de la venta, y los datos del cliente correctamente actualizados.");

            printf("\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 5: //Opción "Eliminar venta"

    //Calculo la nueva facturación del cliente involucrado en caso de que la eliminación sea exitosa.
        nuevo_precio = buffer_clientes[buffer_ventas[id_modificar].cliente].facturacion_total - buffer_ventas[id_modificar].precio_modificado;

        ptr_buffer = buffer_clientes; //Puntero al buffer para calcular el offset del dato a modificar.
        ptr_dato = &buffer_clientes[buffer_ventas[id_modificar].cliente].facturacion_total; //Apunta a la dirección del dato a modificar, para poder calcular su offset.

        if(Modificar_dato(ARCHIVO_CLIENTES, (size_t)(ptr_dato - ptr_buffer), &nuevo_precio, sizeof(float))) //Si la actualización fue exitosa, procede a eliminar el registro de la venta.
        {
            if(Eliminar_registro(ARCHIVO_VENTAS, id_modificar))
            {
                BORRAR_PANTALLA
                sprintf(buffer_cartel, "%s", "La venta seleccionada fue eliminada exitosamente.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, DOBLE);
                printf("\n\n\n\n");
            }
        }
        else
        {
            sprintf(buffer_cartel, "%s%c%s%c%s", "Hubo un error en la actualizaci", 162, "n de la facturaci", 162, "n del cliente involucrado, y por tanto el registro de la venta no fue eliminado.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");

            return 0;
        }

        break;
    }

    return 1; //Devuelve 1 si ha pasado todas las validaciones.
}

//=========================================================================================================
// Modifica el monto neto de la venta y la facturación total del cliente correspondiente.
short Calcular_precio_modificado_venta(unsigned int id_venta)
{
    float viejo_monto = buffer_ventas[id_venta].precio_modificado, precio_modificado = buffer_ventas[id_venta].precio_base; //Variables para calcular la diferencia entre el viejo valor y el nuevo.

    void *ptr_buffer = buffer_ventas, *ptr_dato; //Punteros para modificar el dato en el archivo de ventas.

    for(int i = 1; i < NRO_CATEGORIAS; i++) //Recorre las categorías para ver cuáles corresponden al cliente de la venta.
        if(buffer_clientes[buffer_ventas[id_venta].cliente].categorias[i])
            for(int j = 1; j < NRO_MODIFICADORES; j++) //Recorre los modificadores para ver cuáles aplican a las categorías del cliente.
                if(buffer_categorias[i].modificadores[j])
                {
                    if(buffer_modificadores[j].tipo == PORCENTUAL) //Modifica el nuevo precio según los tipos de modificadores.
                        precio_modificado += buffer_ventas[id_venta].precio_base * buffer_modificadores[j].valor / 100;
                    else
                        precio_modificado += buffer_modificadores[j].valor;
                }

    ptr_dato = &buffer_ventas[id_venta].precio_modificado; //Apunta al dato para poder calcular el offset.

//Modificación
    if(Modificar_dato(ARCHIVO_VENTAS, (size_t)(ptr_dato - ptr_buffer), &precio_modificado, sizeof(float)))
    { //Si la modificación es exitosa, debe actualizar la facturación total del cliente.
        precio_modificado -= viejo_monto; //Calcula la diferencia entre el precio nuevo y el viejo.
        precio_modificado += buffer_clientes[buffer_ventas[id_venta].cliente].facturacion_total; //Calcula la nueva facturación total del cliente.

        ptr_buffer = buffer_clientes; //Apunta al buffer del archivo de clientes.
        ptr_dato = &buffer_clientes[buffer_ventas[id_venta].cliente].facturacion_total; //Apunta al dato para calcular el offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_CLIENTES, (size_t)(ptr_dato - ptr_buffer), &precio_modificado, sizeof(float)))
            return 1; //Devuelve 1 dado que todas las operaciones fueron exitosas.
        else
            return 0;
    }else
        return 0;
}

//==================================================================================================================

short Acreditar_pago(unsigned int id_venta, float pago)
{
    char letreros[150];

    float total_abonado = pago + buffer_ventas[id_venta].abonado; //Calcula el total abonado para esta venta

    void *ptr_buffer = buffer_ventas, *ptr_dato; //Punteros para modificar el dato.

    if(total_abonado < 0) //Validación del total resultante.
    {
        sprintf(letreros, "%s", "El monto final en concepto de pagos para una venta no puede resultar negativo.");

        printf("\n\n");
        Imprime_cartel(letreros, SIMPLE);
        printf("\n\n");

        PAUSA

        return 0;
    }else if(total_abonado > buffer_ventas[id_venta].precio_modificado)
    {
        sprintf(letreros, "%s", "Los pagos totales correspondientes a esta venta no pueden superar su precio neto.");

        printf("\n\n");
        Imprime_cartel(letreros, SIMPLE);
        printf("\n\n");

        PAUSA

        return 0;
    }else
    {
        ptr_dato = &buffer_ventas[id_venta].abonado; //Apunta al dato para calcular el offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_VENTAS, (size_t)(ptr_dato - ptr_buffer), &total_abonado, sizeof(float)))
        { //Si la modificación es exitosa, actualiza los pagos totales del clientes correspondiente a esta venta.
            total_abonado = buffer_clientes[buffer_ventas[id_venta].cliente].pagos_totales + pago;

            ptr_buffer = buffer_clientes;
            ptr_dato = &buffer_clientes[buffer_ventas[id_venta].cliente].pagos_totales;

            if(Modificar_dato(ARCHIVO_CLIENTES, (size_t)(ptr_dato - ptr_buffer), &total_abonado, sizeof(float)))
                return 1; //Devuelve 1, dado que se cumplieron todas las operaciones.
            else
                return 0;
        }else
            return 0;
    }
}

//===================================================================================================================

short Introducir_pago()
{
    unsigned int id_modificar;

    char buffer_cartel[250];

    float pago;

    short ingreso;

    sprintf(buffer_cartel, "%s", "INGRESAR PAGO");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    id_modificar = Pedir_venta(); //Pide la venta para la que se destina el pago.

    BORRAR_PANTALLA

    sprintf(buffer_cartel, "%s", "VENTA SELECCIONADA");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    Desplegar_ventas(&id_modificar, 1);

    sprintf(buffer_cartel, "%s%c%s", "Ingrese el nuevo PAGO correspondiente a la venta seleccionada,"
                            " o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do //Pide el pago a ingresar.
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &pago);
        fflush(stdin);

        if(pago == 0 && ingreso)
            return 0;

        if(!ingreso || pago < 0)
        {
            sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real positivo.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n");
        }

    }while(!ingreso || pago < 0);

    if(Acreditar_pago(id_modificar, pago)) //Acredita el pago.
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "VENTA ABONADA");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        Desplegar_ventas(&id_modificar, 1);

        sprintf(buffer_cartel, "%s", "El pago fue registrado exitosamente y los datos de la venta y del cliente correctamente actualizados.");

        printf("\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);
        printf("\n\n\n\n");
    }else
        return 0;

    return 1; //Devuelve 1 si pasa todas las validaciones.
}

//==================================================================================================================

size_t Buscar_venta_X_descripcion_apellido(unsigned int buffer_ids[], char cadena[LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE])
{
    unsigned int i, id, contador = 0;

    short bandera;

    char *cadena_descompuesta[20], auxiliar_cadena[LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE], auxiliar_descripcion[LONGITUD_DESCRIPCION_VENTAS], auxiliar_apellido[LONGITUD_APELLIDO_CLIENTE];

    for(i = 0; (i < LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE) && (cadena[i] != '\0'); i++)
        auxiliar_cadena[i] = tolower(cadena[i]);
    auxiliar_cadena[i] = '\0';

    cadena_descompuesta[0] = strtok(auxiliar_cadena, " ");
    cadena_descompuesta[1] = cadena_descompuesta[0]; //Evita que la posición 1 del arreglo sea nula por azar, y no ingrese al ciclo siguiente.

    for(i = 1; i < 20 && (cadena_descompuesta[i - 1] != NULL); i++)
        cadena_descompuesta[i] = strtok(NULL, " ");

    buffer_ids[0] = 0; //Esta inicialización garantiza que, si no se encuentran coincidencias, la dirección de memoria
                        //recibida como buffer queda nula.

    for(id = 1; id < tam_buffer_ventas; id++)
    {
        bandera = 1;

        for(i = 0; (i < LONGITUD_DESCRIPCION_VENTAS) && (buffer_ventas[id].descripcion[i] != '\0'); i++)
            auxiliar_descripcion[i] = tolower(buffer_ventas[id].descripcion[i]);
        auxiliar_descripcion[i] = '\0';

        for(i = 0; (i < LONGITUD_APELLIDO_CLIENTE) && (buffer_clientes[buffer_ventas[id].cliente].apellido[i] != '\0'); i++)
            auxiliar_apellido[i] = tolower(buffer_clientes[buffer_ventas[id].cliente].apellido[i]);
        auxiliar_apellido[i] = '\0';

        for(i = 0; i < 20 && cadena_descompuesta[i] != NULL; i++)
            if(!strstr(auxiliar_descripcion, cadena_descompuesta[i]) && !strstr(auxiliar_apellido, cadena_descompuesta[i]))
            {
                bandera = 0;
                break;
            }

        if(bandera)
            buffer_ids[contador++] = id;
    }

    return contador;
}

//==================================================================================================================

void Desplegar_ventas(unsigned int ids[], size_t tamanio_ids)
{
    char formato_encabezado[80];
    char formato_fila[80];
    char formato_fila_descripcion[80];

    char fragmento_descripcion[26], *ptr_descripcion;
    fragmento_descripcion[25] = '\0'; //Asegura que las cadenas copiadas no impriman basura (como ocurría con el despliegue de datos al ingresar un nuevo cliente).

    unsigned short continua_descripcion;

    sprintf(formato_encabezado, "%s%d%s%d%s%d%s", "\n   %c %", 8, "s %c %", 8, "s %c %", LONGITUD_APELLIDO_CLIENTE, "s %c %25s %c %12s %c %12s %c %12s %c");
    sprintf(formato_fila, "%s%d%s%d%s%d%s", "\n   %c %", 8, "u %c %", 8, "u %c %", LONGITUD_APELLIDO_CLIENTE, "s %c %-25s %c %12.2f %c %12.2f %c %12.2f %c");
    sprintf(formato_fila_descripcion, "%s%d%s%d%s%d%s", "\n   %c %", 8, "s %c %", 8, "s %c %", LONGITUD_APELLIDO_CLIENTE, "s %c %-25s %c %12s %c %12s %c %12s %c");

        printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218,196,196,196,196,196,196,196,196,196,196,194,196,196,196,196,196,196,196,196,196,196,194,196,196);
        for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 25; i++) //Corresponde a la columna de DESCRIPCION
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de MONTO BRUTO
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de MONTO NETO
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c", 191);

        printf(formato_encabezado, 179, "ID", 179, "CLIENTE", 179, "APELLIDO CLIENTE", 179, "DESCRIPCION", 179, "MONTO BRUTO", 179, "MONTO NETO", 179, "ABONADO", 179);

        printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196,196,196,196,196,196,196,196,196,197,196,196);
        for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 25; i++)
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de ABONADO
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c", 180);

        for(int i = 0; i < tamanio_ids; i++)
        {
            ptr_descripcion = buffer_ventas[ids[i]].descripcion;

            printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196,196,196,196,196,196,196,196,196,197,196,196);
            for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 25; i++)
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 12; i++) //Corresponde a la columna de ABONADO
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
                printf("%c", 196);
            printf("%c", 180);

            printf(formato_fila, 179, buffer_ventas[ids[i]].id, 179, buffer_ventas[ids[i]].cliente,
                   179, buffer_clientes[buffer_ventas[ids[i]].cliente].apellido,
                   179, strncpy(fragmento_descripcion, ptr_descripcion, 25), 179, buffer_ventas[ids[i]].precio_base,
                   179, buffer_ventas[ids[i]].precio_modificado, 179, buffer_ventas[ids[i]].abonado, 179);

            if(strlen(fragmento_descripcion) >= 25)
            {
                ptr_descripcion += 25;
                continua_descripcion = 1;
            }
            else
                continua_descripcion = 0;

            while(continua_descripcion)
            {
                printf(formato_fila_descripcion, 179, "", 179, "", 179, "",
                        179, strncpy(fragmento_descripcion, ptr_descripcion, 25), 179, "", 179, "", 179, "", 179);

                if(strlen(fragmento_descripcion) < 25)
                   continua_descripcion = 0;
                else
                    ptr_descripcion += 25;
            }
        }

        printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192,196,196,196,196,196,196,196,196,196,196,193,196,196,196,196,196,196,196,196,196,196,193,196,196);
        for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 25; i++)
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c", 217);

        printf("\n\n\n");
}

//===============================================================================================

unsigned int Pedir_venta()
{
    char registro_ingresado[LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE];

    unsigned int opcion, id_venta, busqueda_ids[NRO_CATEGORIAS];

    char buffer_cartel[200];

    short ingreso;

    sprintf(buffer_cartel, "%s%c%s%c%s", "Ingrese el ID de la venta correspondiente al pago, o palabras clave de su DESCRIPCI", 162,
                                            "n y/o el APELLIDO del cliente asociado. "
                                                "0 para volver al men", 163, " anterior.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do //Pide el ID, palabras claves de su descripción o el apellido del cliente asociado a la venta.
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_DESCRIPCION_VENTAS + LONGITUD_APELLIDO_CLIENTE - 1, "[^\n]");
        scanf(buffer_cartel, registro_ingresado);
        fflush(stdin);

        if(!strcmp(registro_ingresado, "0"))
            return 0;

        if((id_venta = Validar_cadena_como_entero(registro_ingresado))) //Verifica si se ingresó un ID.
        {
            if(id_venta >= tam_buffer_ventas || buffer_ventas[id_venta].id == 0) //Valida el ID.
            {
                sprintf(buffer_cartel, "%s", "No existe ninguna venta con el ID ingresado.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                id_venta = 0;
            }
        }else if(!(id_venta = Buscar_venta_X_descripcion_apellido(busqueda_ids, registro_ingresado))) //Si no se ingresó un ID, busca coincidencias en la descripción de la venta o en el apellido del cliente asociado.
        {
            sprintf(buffer_cartel, "%s", "No existe ninguna venta con los criterios ingresados.");

            Imprime_cartel(buffer_cartel, SIMPLE);

        }else if(id_venta == 1) //Si hubo una sola coincidencia, se guarda su ID.
            id_venta = busqueda_ids[0];
        else //Si hubo muchas coincidencias, se muestra la lista y se solicita indicar el ID correcto.
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "VENTAS QUE COINCIDEN CON LA B", 163, "SQUEDA");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_ventas(busqueda_ids, id_venta);

            sprintf(buffer_cartel, "%s%c%s", "Ingrese el ID correspondiente a la venta buscada,"
                                                " o 0 para volver al men", 163, " previo.");

            Imprime_cartel(buffer_cartel, DOBLE);

            do //Pide un ID de la lista y valida el ingreso.
            {
                PEDIR_INGRESO

                ingreso = scanf("%d", &opcion);
                fflush(stdin);

                if(!opcion)
                    return 0;

                if(opcion < 0 || !ingreso)
                {
                    sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero entero positivo.");

                    Imprime_cartel(buffer_cartel, SIMPLE);

                }else if(!buffer_ventas[opcion].id || opcion >= tam_buffer_ventas)
                {
                    sprintf(buffer_cartel, "%s", "No existe ninguna venta con el ID ingresado.");

                    Imprime_cartel(buffer_cartel, SIMPLE);

                    ingreso = 0;
                }else
                    id_venta = opcion;

            }while(opcion < 0 || !ingreso);
        }
    }while(!id_venta);

    return id_venta; //Devuelve el ID de la venta solicitada.
}
