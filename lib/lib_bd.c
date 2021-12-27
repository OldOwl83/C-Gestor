#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "lib_bd.h"
#include "lib_auxiliares.h"
#include "lib_clientes.h"
#include "lib_ventas.h"
#include "lib_categorias.h"
#include "lib_modificadores.h"

unsigned int Insertar_registro(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], void *registro)
{
    FILE *archivo;

    void *buffer_archivo; //Puntero al buffer en memoria del archivo correspondiente a la tabla a modificar.
    size_t *tam_buffer; //Puntero con el que modificar la variable global que almacena la longitud de la tabla a modificar.
    size_t tamanio_registro; //Tamaño correspondiente al tipo de dato de la tabla a modificar.
    unsigned int id, *ptr_id = &id; //Puntero a los IDs de los registros de la tabla a modificar.

    if(!strcmp(nombre_archivo, ARCHIVO_CLIENTES)) //Toma el buffer, la longitud y el tipo de dato de la tabla a manipular
    {
        buffer_archivo = buffer_clientes;
        tam_buffer = &tam_buffer_clientes;
        tamanio_registro = sizeof(cli);
    }
    else if(!strcmp(nombre_archivo, ARCHIVO_CATEGORIAS))
    {
        buffer_archivo = buffer_categorias;
        tam_buffer = &tam_buffer_categorias;
        tamanio_registro = sizeof(cat);
    }
    else if(!strcmp(nombre_archivo, ARCHIVO_MODIFICADORES))
    {
        buffer_archivo = buffer_modificadores;
        tam_buffer = &tam_buffer_modificadores;
        tamanio_registro = sizeof(mod);
    }
    else if(!strcmp(nombre_archivo, ARCHIVO_VENTAS))
    {
        buffer_archivo = buffer_ventas;
        tam_buffer = &tam_buffer_ventas;
        tamanio_registro = sizeof(ven);
    }

    for(id = 1; *ptr_id != 0 && id < *tam_buffer; id++) //Busca el primer registro vacío (ID = 0) en la tabla, y deja a "ptr_id" apuntando a su posición inicial.
        ptr_id = buffer_archivo + id * tamanio_registro;

    if(id >= *tam_buffer) //Si no se encontró un registro vacío, se debe agregar uno al final
    {
        buffer_archivo = realloc(buffer_archivo, ++id * tamanio_registro); //Por tanto, se reasigna más memoria al buffer del archivo correspondiente
        (*tam_buffer)++; //Aumento la longitud de la tabla

        if(!strcmp(nombre_archivo, ARCHIVO_CLIENTES)) //Apunto el buffer correspondiente al nuevo espacio asignado.
            buffer_clientes = buffer_archivo;
        else if(!strcmp(nombre_archivo, ARCHIVO_CATEGORIAS))
            buffer_categorias = buffer_archivo;
        else if(!strcmp(nombre_archivo, ARCHIVO_MODIFICADORES))
            buffer_modificadores = buffer_archivo;
        else if(!strcmp(nombre_archivo, ARCHIVO_VENTAS))
            buffer_ventas = buffer_archivo;
    }

    id--;
    memcpy(registro, &id, sizeof(unsigned int)); //Copio el ID correspondiente a la posición libre hallada en los datos del registro que se intenta insertar.
    memcpy(buffer_archivo + id * tamanio_registro, registro, tamanio_registro); //Copio el registro en el lugar correspondiente del buffer del archivo

    if((archivo = fopen(nombre_archivo, "rb+"))) //Procedo a grabar el registro en el archivo correspondiente a la tabla a modificar.
    {
        if(fseek(archivo, id * tamanio_registro, SEEK_SET)) //Ubico el cursor en la posición correspondiente al ID del registro nuevo.
        {
            fprintf(stderr, "%s %s", "Error en el posicionamiento del cursor interno del archivo", nombre_archivo);

            fclose(archivo);
            return 0;
        }

        if(fwrite(registro, tamanio_registro, 1, archivo) == 1)
        {
            fclose(archivo);

            return id; //Si la escritura del registro es exitosa, devuelve el ID asignado.
        }else
        {
            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", nombre_archivo);

            fclose(archivo);
            return 0;
        }
    }
    else // Control de error en la apertura del archivo (lo que ocurriría si el puntero "archivo" == NULL).
    {
        fprintf(stderr, "%s%s%s%c%s", "Error de apertura del archivo ", nombre_archivo, ". Aseg", 163, "rese de que existe.");

        return 0;
    }
}

//==========================================================================================================

short Eliminar_registro(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], unsigned int id_registro)
{
    FILE *archivo;

    void *buffer_archivo; //Puntero al buffer en memoria del archivo correspondiente a la tabla a modificar.
    size_t *tam_buffer; //Puntero con el que modificar la variable global que almacena la longitud de la tabla a modificar
    size_t tamanio_registro; //Tamaño correspondiente al tipo de dato de la tabla a modificar.
    unsigned int id_nulo = 0, *ptr_id = NULL; //Puntero al registro a eliminar

    char buffer_tachador = '\0';

    char buffer_cartel[120];

    if(!strcmp(nombre_archivo, ARCHIVO_CLIENTES)) //Toma el buffer, la longitud y el tipo de dato de la tabla a manipular.
    {
        buffer_archivo = buffer_clientes;
        tam_buffer = &tam_buffer_clientes;
        tamanio_registro = sizeof(cli);
    }
    else if(!strcmp(nombre_archivo, ARCHIVO_CATEGORIAS))
    {
        buffer_archivo = buffer_categorias;
        tam_buffer = &tam_buffer_categorias;
        tamanio_registro = sizeof(cat);
    }
    else if(!strcmp(nombre_archivo, ARCHIVO_MODIFICADORES))
    {
        buffer_archivo = buffer_modificadores;
        tam_buffer = &tam_buffer_modificadores;
        tamanio_registro = sizeof(mod);
    }
    else if(!strcmp(nombre_archivo, ARCHIVO_VENTAS))
    {
        buffer_archivo = buffer_ventas;
        tam_buffer = &tam_buffer_ventas;
        tamanio_registro = sizeof(ven);
    }

    ptr_id = buffer_archivo + id_registro * tamanio_registro; //Apuntando al registro a eliminar

    if(id_registro >= *tam_buffer) //Valida la existencia del registro
    {
        sprintf(buffer_cartel, "%s%s%s", "El ID indicado excede el rango actual del archivo ", nombre_archivo, ".");

        printf("\n\n\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);
        printf("\n\n\n\n");

        return 0;
    }else if(*ptr_id == 0)
    {
        sprintf(buffer_cartel, "%s%c%s%s%s", "El ID indicado no corresponde a ning", 163, "n registro en el archivo ", nombre_archivo, ".");

        printf("\n\n\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);
        printf("\n\n\n\n");

        return 0;
    }

//Procede a eliminar el registro en el archivo indicado por parámetros.
    if((archivo = fopen(nombre_archivo, "rb+")))
    {
        if(fseek(archivo, tamanio_registro * id_registro, SEEK_SET)) //Busca la posición del registro
        {
            fprintf(stderr, "%s %s", "Error en el posicionamiento del cursor interno del archivo", nombre_archivo);

            fclose(archivo);
            return 0;
        }

        if(fwrite(&id_nulo, sizeof(unsigned int), 1, archivo) != 1) //Sobreescribe el ID con 0 (indica ausencia de registro)
        {
            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", nombre_archivo);

            fclose(archivo);
            return 0;
        }else
            memcpy(buffer_archivo + id_registro * tamanio_registro, &id_nulo, sizeof(unsigned int)); //En caso de éxito, anula la posición correspondiente en el buffer en memoria.

        for(int i = 0; i < tamanio_registro - sizeof(unsigned int); i++) //Tacha con caracteres nulos el resto del registro en el archivo y en el buffer.
        {
            if(fwrite(&buffer_tachador, sizeof(char), 1, archivo) != 1)
            {
                fprintf(stderr, "%s%s", "Error en la escritura del archivo ", nombre_archivo);

                fclose(archivo);
                return 0;
            }else
                memcpy(buffer_archivo + id_registro * tamanio_registro + sizeof(unsigned int) + i, &buffer_tachador, sizeof(char));
        }

        if(id_registro == (*tam_buffer) - 1) //Si el registro eliminado es el último, debe decrementar el tamaño declarado del buffer hasta la última posición con registro activo (que no necesariamente es la inmediatamente anterior).
        {
            do
            {
                ptr_id = buffer_archivo + (--(*tam_buffer) - 1) * tamanio_registro;
            }while(*ptr_id == 0);
        }

        fclose(archivo); //En caso de éxito, cierra el archivo y devuelve 1.
        return 1;
    }
    else // Control de error en la creación del archivo (lo que ocurriría si el puntero "archivo" == NULL).
    {
        fprintf(stderr, "%s%s%s%c%s", "Error de apertura del archivo ", nombre_archivo, ". Aseg", 163, "rese de que existe.");

        fclose(archivo);
        return 0;
    }
}

//===========================================================================================================================

short Modificar_dato(const char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], size_t desplazamiento_dato, void *dato, size_t tamanio_dato)
{
    FILE *archivo;

    void *buffer_archivo; //Puntero al buffer correspondiente al archivo a modificar.

    if(!strcmp(nombre_archivo, ARCHIVO_CLIENTES)) //Toma el buffer de la tabla a manipular
        buffer_archivo = buffer_clientes;
    else if(!strcmp(nombre_archivo, ARCHIVO_CATEGORIAS))
        buffer_archivo = buffer_categorias;
    else if(!strcmp(nombre_archivo, ARCHIVO_MODIFICADORES))
        buffer_archivo = buffer_modificadores;
    else if(!strcmp(nombre_archivo, ARCHIVO_VENTAS))
        buffer_archivo = buffer_ventas;

    if((archivo = fopen(nombre_archivo, "rb+")))
    {
        if(fseek(archivo, desplazamiento_dato, SEEK_SET)) //Ubica al cursor en la dirección precisa del dato a modificar independientemente del struct donde se aloja).
        {
            fprintf(stderr, "%s %s", "Error en el posicionamiento del cursor interno del archivo", nombre_archivo);

            fclose(archivo);
            return 0;
        }

        if(fwrite(dato, tamanio_dato, 1, archivo) != 1) //Escribe el dato en el archivo.
        {
            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", nombre_archivo);

            fclose(archivo);
            return 0;
        }else
        {
            memcpy(buffer_archivo + desplazamiento_dato, dato, tamanio_dato); //Escribe el dato en el buffer.

            fclose(archivo);
            return 1; //Devuelve 1 si la operación fue exitosa.
        }
    }
    else // Control de error en la creación del archivo (lo que ocurriría si el puntero "archivo" == NULL).
    {
        fprintf(stderr, "%s%s%s%c%s", "Error de apertura del archivo ", nombre_archivo, ". Aseg", 163, "rese de que existe.");

        fclose(archivo);
        return 0;
    }
}

//======================================================================================================================

short Alternar_vinculo_categoria_modificador(unsigned int id_categoria, unsigned int id_modificador)
{
    void *ptr_buffer = buffer_categorias, *ptr_dato; //Punteros para obtener el desplazamiento del dato a modificar en el archivo.

    if(buffer_categorias[id_categoria].modificadores[id_modificador]) //Examina si la categoría y el modificador indicados por parámetros estaban o no asociados, e invierte su relación
        buffer_categorias[id_categoria].modificadores[id_modificador] = 0;
    else
        buffer_categorias[id_categoria].modificadores[id_modificador] = 1;

    ptr_dato = &buffer_categorias[id_categoria].modificadores[id_modificador]; //Toma la dirección del dato modificado.

    if(Modificar_dato(ARCHIVO_CATEGORIAS, ptr_dato - ptr_buffer, ptr_dato, sizeof(buffer_categorias[0].modificadores[0]))) //Modifica el dato
        return 1; //Indica el éxito o fracaso de la operación.
    else
        return 0;
}

//======================================================================================================================

short Alternar_vinculo_cliente_categoria(unsigned int id_cliente, unsigned int id_categoria)
{
    void *ptr_buffer = buffer_clientes, *ptr_dato; //Punteros para obtener el desplazamiento del dato a modificar en el archivo.

    if(buffer_clientes[id_cliente].categorias[id_categoria]) //Examina si la categoría y el modificador indicados por parámetros estaban o no asociados, e invierte su relación
        buffer_clientes[id_cliente].categorias[id_categoria] = 0;
    else
        buffer_clientes[id_cliente].categorias[id_categoria] = 1;

    ptr_dato = &buffer_clientes[id_cliente].categorias[id_categoria]; //Toma la dirección del dato modificado.

    if(Modificar_dato(ARCHIVO_CLIENTES, ptr_dato - ptr_buffer, ptr_dato, sizeof(buffer_clientes[0].categorias[0])))
        return 1;//Indica el éxito o fracaso de la operación.
    else
        return 0;
}

//========================================================================================================================

void Consulta_clientes_X_facturacion()
{
    char cadena_carteles[200];

    short ingreso;

    float limite_facturacion;

    unsigned int busqueda_ids[tam_buffer_clientes], contador = 0; //Variables para almacenar los IDs correspondientes a los clientes que cumplen con la condición.

    sprintf(cadena_carteles, "%s%c%s%c%s", "LISTA DE CLIENTES SEG", 163, "N FACTURACI", 162, "N");

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    sprintf(cadena_carteles, "%s%c%s%c%s%c%s%c%s%c%s", "Ingrese la facturaci", 162, "n l", 161,
                            "mite para la lista de clientes (valor negativo para establecer un m", 160, "ximo, o positivo para un m", 161,
                            "nimo), o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(cadena_carteles, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &limite_facturacion);
        fflush(stdin);

        if(!ingreso)
        {
            BORRAR_PANTALLA

            sprintf(cadena_carteles, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(cadena_carteles, SIMPLE);
            printf("\n\n");

        }else if(limite_facturacion == 0)
            return;

    }while(!ingreso);

    if(limite_facturacion > 0) //Si se estableció un piso, se busca todos los clientes cuya facturación supere el límite indicado.
    {
        for(int i = 1; i < tam_buffer_clientes; i++)
            if(buffer_clientes[i].id && buffer_clientes[i].facturacion_total > limite_facturacion)
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%c%s%.2f", "CLIENTES CON FACTURACI", 162, "N MAYOR A $ ", limite_facturacion);
    }else //Si se estableció un techo, se busca todos los clientes cuya facturación sea menor al límite indicado.
    {
        for(int i = 1; i < tam_buffer_clientes; i++)
            if(buffer_clientes[i].id && buffer_clientes[i].facturacion_total < fabs(limite_facturacion))
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%c%s%.2f", "CLIENTES CON FACTURACI", 162, "N MENOR A $ ", fabs(limite_facturacion));
    }

    BORRAR_PANTALLA

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    Desplegar_clientes(busqueda_ids, contador);
}

//==================================================================================================================

void Consulta_clientes_X_deuda()
{
    char cadena_carteles[200];

    short ingreso;

    float limite_deuda;

    unsigned int busqueda_ids[tam_buffer_clientes], contador = 0; //Variables para almacenar los IDs correspondientes a los clientes que cumplen con la condición.

    sprintf(cadena_carteles, "%s%c%s", "LISTA DE CLIENTES SEG", 163, "N DEUDA");

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    sprintf(cadena_carteles, "%s%c%s%c%s%c%s%c%s", "Ingrese la deuda l", 161,
                            "mite para la lista de clientes (valor negativo para establecer un m", 160, "ximo, o positivo para un m", 161,
                            "nimo), o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(cadena_carteles, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &limite_deuda);
        fflush(stdin);

        if(!ingreso)
        {
            BORRAR_PANTALLA

            sprintf(cadena_carteles, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(cadena_carteles, SIMPLE);
            printf("\n\n");

        }else if(limite_deuda == 0)
            return;

    }while(!ingreso);

    if(limite_deuda > 0) //Si se estableció un piso, se busca todos los clientes cuya deuda supere el límite indicado.
    {
        for(int i = 1; i < tam_buffer_clientes; i++)
            if(buffer_clientes[i].id && buffer_clientes[i].facturacion_total - buffer_clientes[i].pagos_totales > limite_deuda)
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%.2f", "CLIENTES CON DEUDA MAYOR A $ ", limite_deuda);
    }else //Si se estableció un techo, se busca todos los clientes cuya deuda sea menor al límite indicado.
    {
        for(int i = 1; i < tam_buffer_clientes; i++)
            if(buffer_clientes[i].id && buffer_clientes[i].facturacion_total - buffer_clientes[i].pagos_totales < fabs(limite_deuda)
               && buffer_clientes[i].facturacion_total - buffer_clientes[i].pagos_totales > 0)
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%.2f", "CLIENTES CON DEUDA MENOR A $ ", fabs(limite_deuda));
    }

    BORRAR_PANTALLA

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    Desplegar_clientes(busqueda_ids, contador);
}

//========================================================================================================

void Consulta_ventas_X_cliente()
{
    char cadena_carteles[150];

    unsigned int id_cliente, busqueda_ids[tam_buffer_ventas], contador = 0; //Variables para almacenar los IDs correspondientes a las ventas del cliente indicado.

    sprintf(cadena_carteles, "%s", "VENTAS VINCULADAS A UN CLIENTE PARTICULAR");

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    id_cliente = Pedir_cliente(); //Solicita el cliente cuyas ventas se quieren listar-

    for(int i = 1; i < tam_buffer_ventas; i++) //Busca las ventas del cliente indicado.
        if(buffer_ventas[i].cliente == id_cliente)
            busqueda_ids[contador++] = buffer_ventas[i].id;

    BORRAR_PANTALLA

    sprintf(cadena_carteles, "%s", "VENTAS CORRESPONDIENTES AL CLIENTE SELECCIONADO");

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    Desplegar_ventas(busqueda_ids, contador);
}

//========================================================================================================================

void Consulta_ventas_X_montoBase()
{
    char cadena_carteles[200];

    short ingreso;

    float limite_facturacion;

    unsigned int busqueda_ids[tam_buffer_ventas], contador = 0; //Variables para almacenar los IDs correspondientes a las ventas que cumplen con la condición indicada por el usuario.

    sprintf(cadena_carteles, "%s%c%s", "LISTA DE VENTAS SEG", 163, "N MONTO BASE");

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    sprintf(cadena_carteles, "%s%c%s%c%s%c%s%c%s", "Ingrese el MONTO BASE l", 161,
                            "mite para la lista de ventas (valor negativo para establecer un m", 160, "ximo, o positivo para un m", 161,
                            "nimo), o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(cadena_carteles, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &limite_facturacion);
        fflush(stdin);

        if(!ingreso)
        {
            BORRAR_PANTALLA

            sprintf(cadena_carteles, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(cadena_carteles, SIMPLE);
            printf("\n\n");

        }else if(limite_facturacion == 0)
            return;

    }while(!ingreso);

    if(limite_facturacion > 0) //Si se estableció un piso, se busca todas las ventas cuya facturación bruta supere el límite indicado.
    {
        for(int i = 1; i < tam_buffer_ventas; i++)
            if(buffer_ventas[i].id && buffer_ventas[i].precio_base > limite_facturacion)
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%.2f", "VENTAS POR UN MONTO BASE MAYOR A $ ", limite_facturacion);
    }else //Si se estableció un techo, se busca todas las ventas cuya facturación bruta sea menor al límite indicado.
    {
        for(int i = 1; i < tam_buffer_ventas; i++)
            if(buffer_ventas[i].id && buffer_ventas[i].precio_base < fabs(limite_facturacion))
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%.2f", "VENTAS POR UN MONTO BASE MENOR A $ ", fabs(limite_facturacion));
    }

    BORRAR_PANTALLA

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    Desplegar_ventas(busqueda_ids, contador);
}

//========================================================================================================================

void Consulta_ventas_X_montoNeto()
{
    char cadena_carteles[200];

    short ingreso;

    float limite_facturacion;

    unsigned int busqueda_ids[tam_buffer_ventas], contador = 0; //Variables para almacenar los IDs correspondientes a las ventas que cumplen con la condición indicada por el usuario.

    sprintf(cadena_carteles, "%s%c%s", "LISTA DE VENTAS SEG", 163, "N MONTO NETO");

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    sprintf(cadena_carteles, "%s%c%s%c%s%c%s%c%s", "Ingrese el MONTO NETO l", 161,
                            "mite para la lista de ventas (valor negativo para establecer un m", 160, "ximo, o positivo para un m", 161,
                            "nimo), o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(cadena_carteles, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &limite_facturacion);
        fflush(stdin);

        if(!ingreso)
        {
            BORRAR_PANTALLA

            sprintf(cadena_carteles, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(cadena_carteles, SIMPLE);
            printf("\n\n");

        }else if(limite_facturacion == 0)
            return;

    }while(!ingreso);

    if(limite_facturacion > 0) //Si se estableció un piso, se busca todas las ventas cuya facturación neta supere el límite indicado.
    {
        for(int i = 1; i < tam_buffer_ventas; i++)
            if(buffer_ventas[i].id && buffer_ventas[i].precio_modificado > limite_facturacion)
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%.2f", "VENTAS POR UN MONTO NETO MAYOR A $ ", limite_facturacion);
    }else //Si se estableció un techo, se busca todas las ventas cuya facturación neta sea menor al límite indicado.
    {
        for(int i = 1; i < tam_buffer_ventas; i++)
            if(buffer_ventas[i].id && buffer_ventas[i].precio_modificado < fabs(limite_facturacion))
                busqueda_ids[contador++] = i; //Se almacenan los IDs

        sprintf(cadena_carteles, "%s%.2f", "VENTAS POR UN MONTO NETO MENOR A $ ", fabs(limite_facturacion));
    }

    BORRAR_PANTALLA

    Imprime_cartel(cadena_carteles, ENCABEZADO);

    Desplegar_ventas(busqueda_ids, contador);
}

//=====================================================================================================================

void Facturacion_total()
{
    char cadena_cartel[200];

    float facturacion_neta = 0, facturacion_bruta = 0;

    sprintf(cadena_cartel, "%s%c%s", "FACTURACI", 162, "N TOTAL");

    Imprime_cartel(cadena_cartel, ENCABEZADO);

    for(int i = 1; i < tam_buffer_ventas; i++) //Suma tanto la facturación bruta como la neta de todas las ventas.
        if(buffer_ventas[i].id)
        {
            facturacion_bruta += buffer_ventas[i].precio_base;
            facturacion_neta += buffer_ventas[i].precio_modificado;
        }

    sprintf(cadena_cartel, "%s%.2f", "La suma total del precio base de todas las ventas es de $ ", facturacion_bruta);

    Imprime_cartel(cadena_cartel, DOBLE);

    sprintf(cadena_cartel, "%s%.2f", "La suma total del precio neto de todas las ventas es de $ ", facturacion_neta);

    Imprime_cartel(cadena_cartel, DOBLE);
}

//=====================================================================================================================

void Facturacion_total_X_categoria()
{
    char cadena_cartel[200];

    unsigned int id_categoria;

    float facturacion_neta = 0, facturacion_bruta = 0;

    sprintf(cadena_cartel, "%s%c%s%c%s", "FACTURACI", 162, "N POR CATEGOR", 161, "A");

    Imprime_cartel(cadena_cartel, ENCABEZADO);

    id_categoria = Pedir_categoria();

    for(int i = 1; i < tam_buffer_ventas; i++) //Suma tanto la facturación bruta como la neta de las ventas correspondientes a todos los clientes de la categoría indicada por el usuario.
        if(buffer_ventas[i].id && buffer_clientes[buffer_ventas[i].cliente].categorias[id_categoria])
        {
            facturacion_bruta += buffer_ventas[i].precio_base;
            facturacion_neta += buffer_ventas[i].precio_modificado;
        }

    sprintf(cadena_cartel, "%s%c%s%s%s%.2f", "La suma total del PRECIO BASE de todas las ventas correspondientes a la categor", 161,
                                    "a ", buffer_categorias[id_categoria].nombre, " es de $ ", facturacion_bruta);

    Imprime_cartel(cadena_cartel, DOBLE);

    sprintf(cadena_cartel, "%s%c%s%s%s%.2f", "La suma total del PRECIO NETO de todas las ventas correspondientes a la categor", 161,
                                    "a ", buffer_categorias[id_categoria].nombre, " es de $ ", facturacion_neta);

    Imprime_cartel(cadena_cartel, DOBLE);

    printf("\n\n\n\n");
}

//==============================================================================

void Calcular_deuda_total()
{
    char cadena_cartel[140];

    float deuda_total = 0;

    sprintf(cadena_cartel, "%s", "DEUDA TOTAL");

    Imprime_cartel(cadena_cartel, ENCABEZADO);

    for(int i = 1; i < tam_buffer_clientes; i++) //Suma la deuda de todos los clientes registrados.
        deuda_total += buffer_clientes[i].facturacion_total - buffer_clientes[i].pagos_totales;

    sprintf(cadena_cartel, "%s%.2f", "La deuda acumulada por todos los clientes es de $ ", deuda_total);

    Imprime_cartel(cadena_cartel, DOBLE);

    printf("\n\n\n\n");
}
