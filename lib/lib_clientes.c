#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lib_clientes.h"
#include "lib_bd.h"
#include "lib_auxiliares.h"
#include "lib_categorias.h"
#include "lib_modificadores.h"

unsigned int Agregar_nuevo_cliente()
{
    cli nuevo_cliente = {0, "", "", {0}, 0.00, 0.00, ""}; // buffer para grabar los datos del nuevo cliente;

    unsigned int id_agregado; //Toma el ID del nuevo cliente

    short ingreso;

    char buffer_cartel[180];

    sprintf(buffer_cartel, "%s", "INGRESO DE NUEVO CLIENTE");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

//PIDE APELLIDO

    sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ingrese el APELLIDO del nuevo cliente (m", 160, "ximo ", LONGITUD_APELLIDO_CLIENTE - 2, " caracteres) o 0 para volver al men", 163, " de gesti", 162, "n de clientes.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_APELLIDO_CLIENTE - 1, "[^\n]");
        scanf(buffer_cartel, nuevo_cliente.apellido);
        fflush(stdin);

        if(!strcmp(nuevo_cliente.apellido, "0"))
            return 0;

        if(!(ingreso = Validar_cadena(nuevo_cliente.apellido, LONGITUD_APELLIDO_CLIENTE - 2, IMP_SI, ALFABETICO)))
            nuevo_cliente.apellido[0] = '\0';

    }while(!ingreso);

//PIDE NOMBRE

    sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ingrese el NOMBRE del nuevo cliente (m", 160, "ximo ", LONGITUD_APELLIDO_CLIENTE - 2, " caracteres) o 0 para volver al men", 163, " de gesti", 162, "n de clientes.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_CLIENTE - 1, "[^\n]");
        scanf(buffer_cartel, nuevo_cliente.nombre);
        fflush(stdin);

        if(!strcmp(nuevo_cliente.nombre, "0"))
            return 0;

        if(!(ingreso = Validar_cadena(nuevo_cliente.nombre, LONGITUD_NOMBRE_CLIENTE - 2, IMP_SI, ALFABETICO)))
            nuevo_cliente.nombre[0] = '\0';

    }while(!ingreso);

//PIDE DATOS ADICIONALES

    sprintf(buffer_cartel, "%s%c%s%d%s", "Agregue datos adicionales para este cliente, si lo desea (m", 160, "ximo ", LONGITUD_DATOS_ADICIONALES - 2,
                            " caracteres), o ingrese 0 para saltar este paso.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        nuevo_cliente.datos_adicionales[LONGITUD_DATOS_ADICIONALES - 1] = 'a'; //Anulo la condición de permanencia en el ciclo.

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_DATOS_ADICIONALES - 1, "[^\n]");
        scanf(buffer_cartel, nuevo_cliente.datos_adicionales);
        fflush(stdin);

        if(!strcmp(nuevo_cliente.datos_adicionales, "0"))
            nuevo_cliente.datos_adicionales[0] = '\0';

        else if(nuevo_cliente.datos_adicionales[LONGITUD_DATOS_ADICIONALES - 1] == '\0')
        {
            sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ha excedido la extensi", 162, "n permitida para este campo (", LONGITUD_DATOS_ADICIONALES - 2, " caracteres)."
                                                    "Reduzca la entrada o solicite al programador una ampliaci", 162, "n del l", 161, "mite.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            nuevo_cliente.datos_adicionales[0] = '\0';
        }
    }while(nuevo_cliente.datos_adicionales[LONGITUD_DATOS_ADICIONALES - 1] == '\0');

//Registro del nuevo cliente
    if(!(id_agregado = Insertar_registro(ARCHIVO_CLIENTES, &nuevo_cliente)))
    {
        sprintf(buffer_cartel, "%s%c%s", "Hubo un error en la manipulaci", 162, "n del archivo de clientes."
                                            "Por favor, revise el estado actual de la base de datos.");
        printf("\n\n\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, SIMPLE);
        printf("\n\n\n\n");

        return 0;
    }else
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%c%s", "El nuevo cliente fue incorporado exitosamente a la base de datos. ", 168,
                                            "Desea asociarlo con alguna categor", 161, "a existente? (s/n)");
        printf("\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);

    //Eventual vinculación del nuevo cliente con alguna categoría ya registrada
        do
        {
            PEDIR_INGRESO

            ingreso = scanf("%c", &buffer_cartel[0]);
            fflush(stdin);

            buffer_cartel[0] = tolower(buffer_cartel[0]);

            if(ingreso == 0 || (buffer_cartel[0] != 's' && buffer_cartel[0] != 'n'))
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.", 168,
                                                "Desea asociar el nuevo cliente con alguna categor", 161, "a? (s/n)");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
            }

        }while(ingreso == 0 || (buffer_cartel[0] != 's' && buffer_cartel[0] != 'n'));

        if(buffer_cartel[0] == 's')
            Vincular_cliente_categoria(id_agregado);

        return id_agregado; //Devuelve el ID atribuido al nuevo cliente.
    }
}

//=========================================================================================================

short Modificar_cliente()
{
    char registro_ingresado[LONGITUD_DATOS_ADICIONALES > LONGITUD_APELLIDO_CLIENTE ? LONGITUD_DATOS_ADICIONALES : LONGITUD_APELLIDO_CLIENTE];

    unsigned int opcion, id_modificar;

    char buffer_cartel[LONGITUD_DATOS_ADICIONALES < 200 ? 200 : LONGITUD_DATOS_ADICIONALES];

    short ingreso;

    void *ptr_buffer = buffer_clientes, *ptr_dato; //Punteros para obtener el desplazamiento del dato a modificar en el archivo.

    sprintf(buffer_cartel, "%s", "MODIFICAR CLIENTE");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    id_modificar = Pedir_cliente(); //Pide el nombre o el ID del cliente a modificar.

    do //Despliega las opciones de modificación
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "CLIENTE SELECCIONADO");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        Desplegar_clientes(&id_modificar, 1);

        sprintf(buffer_cartel, "%s", "MODIFICAR CLIENTE");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        sprintf(buffer_cartel, "%s%c%s", "Cambiar apellido."
                                            "Cambiar nombre."
                                            "Agregar/remover categor", 161, "as."
                                            "Modificar datos adicionales."
                                            "Eliminar cliente");

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
    case 1: //Opción "Cambiar apellido"
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ingrese el nuevo APELLIDO del cliente seleccionado (m", 160, "ximo ", LONGITUD_APELLIDO_CLIENTE - 2, " caracteres) o 0 para volver al men", 163, " de gesti", 162, "n de clientes.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_APELLIDO_CLIENTE - 1, "[^\n]");
            scanf(buffer_cartel, registro_ingresado);
            fflush(stdin);

            if(!strcmp(registro_ingresado, "0"))
                return 0;

            if(!(ingreso = Validar_cadena(registro_ingresado, LONGITUD_APELLIDO_CLIENTE - 2, IMP_SI, ALFABETICO)))
                registro_ingresado[0] = '\0';

        }while(!ingreso);

        ptr_dato = buffer_clientes[id_modificar].apellido; //Apunta a la dirección del apellido a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_CLIENTES, (size_t)(ptr_dato - ptr_buffer), registro_ingresado, LONGITUD_APELLIDO_CLIENTE))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "CLIENTE MODIFICADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_clientes(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El cliente seleccionado fue modificado exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 2: //Opción "Cambiar nombre"
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ingrese el nuevo NOMBRE del cliente seleccionado (m", 160,
                                                        "ximo ", LONGITUD_NOMBRE_CLIENTE - 2, " caracteres) o 0 para volver al men",
                                                        163, " de gesti", 162, "n de clientes.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_CLIENTE - 1, "[^\n]");
            scanf(buffer_cartel, registro_ingresado);
            fflush(stdin);

            if(!strcmp(registro_ingresado, "0"))
                return 0;

            if(!(ingreso = Validar_cadena(registro_ingresado, LONGITUD_NOMBRE_CLIENTE - 2, IMP_SI, ALFABETICO)))
                registro_ingresado[0] = '\0';

        }while(!ingreso);

        ptr_dato = buffer_clientes[id_modificar].nombre; //Apunta a la dirección del apellido a modificar, para poder calcular su offset.

        if(Modificar_dato(ARCHIVO_CLIENTES, (size_t)(ptr_dato - ptr_buffer), registro_ingresado, LONGITUD_NOMBRE_CLIENTE))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "CLIENTE MODIFICADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_clientes(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El cliente seleccionado fue modificado exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 3: //Opción "Agregar/remover categoría"

        BORRAR_PANTALLA

        Vincular_cliente_categoria(id_modificar);

        break;
    case 4: //Opción "Modificar datos adicionales"

        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "DATOS ADICIONALES ACTUALES");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        sprintf(buffer_cartel, "%s", buffer_clientes[id_modificar].datos_adicionales);

        Imprime_cartel(buffer_cartel, SIMPLE);

        sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Escriba los nuevos datos adicionales para este cliente (m", 160,
                                                        "ximo ", LONGITUD_DATOS_ADICIONALES - 2, " caracteres) o 0 para volver al men",
                                                        163, " de gesti", 162, "n de clientes.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do //Solicita los nuevos datos adicionales y los valida.
        {
            PEDIR_INGRESO

            registro_ingresado[LONGITUD_DATOS_ADICIONALES - 1] = 'a'; //Anulo la condición de permanencia en el ciclo.

            sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_DATOS_ADICIONALES - 1, "[^\n]");
            scanf(buffer_cartel, registro_ingresado);
            fflush(stdin);

            if(!strcmp(registro_ingresado, "0"))
                return 0;

            else if(registro_ingresado[LONGITUD_DATOS_ADICIONALES - 1] == '\0')
            {
                sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s", "Ha excedido la extensi", 162, "n permitida para este campo (",
                                                LONGITUD_DATOS_ADICIONALES - 2, " caracteres). Reduzca la entrada o solicite"
                                                " al programador una ampliaci", 162, "n del l", 161, "mite.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                registro_ingresado[0] = '\0';
            }
        }while(registro_ingresado[LONGITUD_DATOS_ADICIONALES - 1] == '\0');

        ptr_dato = &buffer_clientes[id_modificar].datos_adicionales; //Apunta a la dirección de los datos a modificar, para poder calcular su offset.

//Modificación
        if(Modificar_dato(ARCHIVO_CLIENTES, (size_t)(ptr_dato - ptr_buffer), registro_ingresado, LONGITUD_DATOS_ADICIONALES))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "CLIENTE MODIFICADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_clientes(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El cliente seleccionado fue modificado exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 5: //Opción "Eliminar cliente"
        if(Eliminar_registro(ARCHIVO_CLIENTES, id_modificar))
        {
            BORRAR_PANTALLA
            sprintf(buffer_cartel, "%s", "El cliente seleccionado fue eliminado exitosamente.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }

        break;
    }

    return 1; //Devuelve 1 si ha pasado todas las validaciones.
}

//=========================================================================================================

void Desplegar_clientes(unsigned int ids[], size_t tamanio_ids)
{
    char formato_encabezado[80]; //Buffers de los formatos de salida para las distintas filas de la tabla a desplegar.
    char formato_fila[80];
    char formato_fila_categ[80];

    char fragmento_datos_ad[26], *ptr_datos_ad; //Variables para particionar los datos adicionales dentro de una misma fila.
    fragmento_datos_ad[25] = '\0'; //Asegura que las cadenas copiadas no impriman basura (como ocurría con el despliegue de datos al ingresar un nuevo cliente).

    unsigned short continua_datos; //Bandera

    unsigned int categorias_asociadas[NRO_CATEGORIAS], contador; //Guarda las categorías a ser desplegadas dentro de una misma fila.

    sprintf(formato_encabezado, "%s%d%s%d%s%d%s%d%s", "\n   %c %", 8, "s %c %", LONGITUD_APELLIDO_CLIENTE, "s %c %", LONGITUD_NOMBRE_CLIENTE, "s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c %12s %c %12s %c %25s %c");
    sprintf(formato_fila, "%s%d%s%d%s%d%s%d%s", "\n   %c %", 8, "u %c %", LONGITUD_APELLIDO_CLIENTE, "s %c %", LONGITUD_NOMBRE_CLIENTE, "s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c %12.2f %c %12.2f %c %-25s %c");
    sprintf(formato_fila_categ, "%s%d%s%d%s%d%s%d%s", "\n   %c %", 8, "s %c %", LONGITUD_APELLIDO_CLIENTE, "s %c %", LONGITUD_NOMBRE_CLIENTE, "s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c %12s %c %12s %c %-25s %c");

//ENCABEZADOS
        printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218,196,196,196,196,196,196,196,196,196,196,194,196,196);
        for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de ABONADO
            printf("%c", 196);
        printf("%c%c%c", 194, 196, 196);
        for(int i = 0; i < 25; i++) //Corresponde a la columna de DATOS ADICIONALES
            printf("%c", 196);
        printf("%c", 191);

        printf(formato_encabezado, 179, "ID", 179, "APELLIDO", 179, "NOMBRE", 179, "CATEGORIAS", 179, "FACTURADO", 179, "ABONADO", 179, "DATOS ADICIONALES", 179);

        printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196);
        for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de ABONADO
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < 25; i++) //Corresponde a la columna de DATOS ADICIONALES
            printf("%c", 196);
        printf("%c", 180);

//FILAS
        for(int i = 0; i < tamanio_ids; i++)
        {
            for(int j = 0; j < NRO_CATEGORIAS; j++)
                categorias_asociadas[j] = 0;

            contador = 0;

            for(int j = 1; j < tam_buffer_categorias; j++)
                if(buffer_clientes[ids[i]].categorias[j])
                    categorias_asociadas[contador++] = j;

            ptr_datos_ad = buffer_clientes[ids[i]].datos_adicionales;

            printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196);
            for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < LONGITUD_NOMBRE_CLIENTE; i++)
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 12; i++) //Corresponde a la columna de ABONADO
                printf("%c", 196);
            printf("%c%c%c", 197, 196, 196);
            for(int i = 0; i < 25; i++) //Corresponde a la columna de DATOS ADICIONALES
                printf("%c", 196);
            printf("%c", 180);

        //Datos de cada cliente
            printf(formato_fila, 179, buffer_clientes[ids[i]].id, 179, buffer_clientes[ids[i]].apellido,
                   179, buffer_clientes[ids[i]].nombre,
                   179, categorias_asociadas[0] != 0 ? buffer_categorias[categorias_asociadas[0]].nombre : "S/M",
                   179, buffer_clientes[ids[i]].facturacion_total, 179, buffer_clientes[ids[i]].pagos_totales,
                   179, strncpy(fragmento_datos_ad, ptr_datos_ad, 25), 179);

            if(strlen(fragmento_datos_ad) >= 25) //Verifica la excedencia de caracteres de "datos adicionales" sin imprimir.
            {
                ptr_datos_ad += 25;
                continua_datos = 1;
            }
            else
                continua_datos = 0;

            for(int j = 1; categorias_asociadas[j] != 0 || continua_datos; j++) //Agrega renglones internos a las filas para agregar, o bien categorías, o bien "datos adicionales".
            {

                printf(formato_fila_categ, 179, "", 179, "", 179, "",
                        179, categorias_asociadas[j] ? buffer_categorias[categorias_asociadas[j]].nombre : "", 179, "",
                        179, "", 179, continua_datos ? strncpy(fragmento_datos_ad, ptr_datos_ad, 25) : "", 179);

                if(strlen(fragmento_datos_ad) < 25)
                   continua_datos = 0;
                else
                    ptr_datos_ad += 25;
            }
        }

//CIERRE DE LA TABLA
        printf("\n   %c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192,196,196,196,196,196,196,196,196,196,196,193,196,196);
        for(int i = 0; i < LONGITUD_APELLIDO_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_CLIENTE; i++)
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de FACTURADO
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 12; i++) //Corresponde a la columna de ABONADO
            printf("%c", 196);
        printf("%c%c%c", 193, 196, 196);
        for(int i = 0; i < 25; i++) //Corresponde a la columna de DATOS ADICIONALES
            printf("%c", 196);
        printf("%c", 217);

        printf("\n\n\n");
}

//======================================================================================================

//Siempre se debe validar la cadena antes de pasarla como argumento a esta función
size_t Buscar_cliente_X_apellido(unsigned int buffer_ids[], char apellido[LONGITUD_APELLIDO_CLIENTE], pre_b precision)
{
    size_t longitud_nombre_ingresado;

    unsigned int id, contador = 0;

    buffer_ids[0] = 0; //Esta inicialización garantiza que, si no se encuentran coincidencias, la dirección de memoria
                        //recibida como buffer queda nula.

    if(precision == COIN_EXACTA) //Devuelve la primera coincidencia exacta con el apellido ingresado
    {
        for(id = 1; id < tam_buffer_clientes; id++)
            if(!strcmp(buffer_clientes[id].apellido, apellido))
            {
                buffer_ids[0] = id;
                return 1;
            }
    }else //Devuelve la lista de todos los apellidos que comienzan con el apellido indicado.
    {
        longitud_nombre_ingresado = strlen(apellido);

        for(id = 1; id < tam_buffer_clientes; id++)
            if(!strncmp(buffer_clientes[id].apellido, apellido, longitud_nombre_ingresado)) // Evalúo en todos los casos si el número de coincidencias entre
                                                                                        // la cadena ingresada y los primeros caracteres del campo APELLIDO
                                                                                        // de cada registro coincide con la longitud de la cadena ingresada.
            {
                buffer_ids[contador++] = id;
            }
    }

    return contador; //Devuelve la cantidad de coincidencias.
}

//========================================================================================================

short Vincular_cliente_categoria(unsigned int id_cliente)
{
    char confirmacion;

    char buffer_cartel[180];

    unsigned int id_categoria;

    short ingreso;

    do
    {
        id_categoria = Pedir_categoria(); //Pregunta por la categoría a vincular o desvincular.

        if(Alternar_vinculo_cliente_categoria(id_cliente, id_categoria)) //Realiza la operación
        {
            BORRAR_PANTALLA

            Desplegar_clientes(&id_cliente, 1);

            sprintf(buffer_cartel, "%s%c%s", "La categor", 161, "a seleccionada ha sido asociada/disociada exitosamente.");

            printf("\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n");
        }else
        {
            sprintf(buffer_cartel, "%s", "Revise el estado actual de la base de datos, antes de continuar.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            return 0;
        }

        do //Pregunta por una nueva operación, y valida la respuesta.
        {
            sprintf(buffer_cartel, "%c%s%c%s", 168, "Desea asociar o disociar otra categor", 161, "a con este cliente? (s/n)");

            Imprime_cartel(buffer_cartel, SIMPLE);

            PEDIR_INGRESO

            ingreso = scanf("%c", &confirmacion);
            fflush(stdin);

            confirmacion = tolower(confirmacion);

            if(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'))
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.", 168,
                                                "Desea asociar/disociar alguna categor", 161, "a con el cliente seleccionado? (s/n)");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
            }
        }while(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'));

    }while(confirmacion != 'n');

    return 1; //Devuelve 1 si las operaciones se han realizado con éxito.
}

//==================================================================

unsigned int Pedir_cliente()
{
    char registro_ingresado[LONGITUD_APELLIDO_CLIENTE];

    unsigned int opcion, id_cliente, busqueda_ids[tam_buffer_clientes];

    char buffer_cartel[200];

    short ingreso;

    sprintf(buffer_cartel, "%s%c%s", "Ingrese el APELLIDO o el ID del cliente buscado,"
                                                    " o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do //Pide APELLIDO o ID del cliente a buscar.
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_APELLIDO_CLIENTE - 1, "[^\n]");
        scanf(buffer_cartel, registro_ingresado);
        fflush(stdin);

        if(!strcmp(registro_ingresado, "0"))
            return 0;

        if((id_cliente = Validar_cadena_como_entero(registro_ingresado))) //Verifica si se ingresó un ID
        {
            if(id_cliente >= tam_buffer_clientes || buffer_clientes[id_cliente].id == 0) //Valida el ID
            {
                sprintf(buffer_cartel, "%s%c%s", "No existe ning", 163, "n cliente con el ID ingresado.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                id_cliente = 0;
            }
        }else if(Validar_cadena(registro_ingresado, LONGITUD_APELLIDO_CLIENTE - 2, IMP_NO, ALFABETICO)) //Si no se ingresó un ID válido, valida la cadena como APELLIDO.
        {
            if(!(id_cliente = Buscar_cliente_X_apellido(busqueda_ids, registro_ingresado, COIN_PARCIAL))) //Busca el APELLIDO
            {
                sprintf(buffer_cartel, "%s%c%s", "No existe ning", 163, "cliente con el APELLIDO ingresado. Por favor, revise la lista de clientes activos.");

                Imprime_cartel(buffer_cartel, SIMPLE);

            }else if(id_cliente == 1) //Si hubo una sola coincidencia, se guarda su ID.
                id_cliente = busqueda_ids[0];
            else //Si hubo muchas coincidencias, se muestra la lista y se solicita indicar el ID correcto.
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s", "CLIENTES QUE COINCIDEN CON LA B", 163, "SQUEDA");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                Desplegar_clientes(busqueda_ids, id_cliente);

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingrese el ID correspondiente al cliente buscado,"
                                                " o 0 para volver al men", 163, " de gesti", 162, "n de clientes.");

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

                    }else if(!buffer_clientes[opcion].id || opcion >= tam_buffer_clientes)
                    {
                        sprintf(buffer_cartel, "%s%c%s", "No existe ning", 163, "n cliente con el ID ingresado.");

                        Imprime_cartel(buffer_cartel, SIMPLE);

                        ingreso = 0;
                    }else
                        id_cliente = opcion;

                }while(opcion < 0 || !ingreso);
            }
        }else
        {
            sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso inv", 160, "lido. Este campo admite s", 162, "lo letras y espacios.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            id_cliente = 0;
        }

        if(!id_cliente)
            registro_ingresado[0] = '\0';

    }while(!id_cliente);

    return id_cliente; //Devuelve el ID del cliente solicitado.
}
