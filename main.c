#include <stdio.h>
#include <stdlib.h>

//Archivo de cabecera con definición de tipos y constantes simbólicas
#include "lib/definiciones.h"

//Librerías de la base de datos
#include "lib/lib_archivos.h"
#include "lib/lib_bd.h"
#include "lib/lib_clientes.h"
#include "lib/lib_categorias.h"
#include "lib/lib_modificadores.h"
#include "lib/lib_ventas.h"
#include "lib/lib_auxiliares.h"


//Variables puntero globales utilizadas como buffers de lectura
//y escritura de los archivos de la base de datos. El objetivo
//es que siempre repliquen el contenido total de los archivos,
//de modo de tener un acceso más rápido a la información.
cli *buffer_clientes = NULL;
cat *buffer_categorias = NULL;
mod *buffer_modificadores = NULL;
ven *buffer_ventas = NULL;

//Variables globales para almacenar el tamaño actual de los buffers
//de los archivos de la base de datos, ya que los mismos varían dinámicamente.
size_t tam_buffer_clientes = 0;
size_t tam_buffer_categorias = 0;
size_t tam_buffer_modificadores = 0;
size_t tam_buffer_ventas = 0;

//Prototipos para el archivo main.c (funciones del menú de opciones)
void Menu_gestion_base_datos();
void Menu_gestion_clientes();
void Menu_gestion_categorias();
void Menu_gestion_ventas();
void Menu_consultas();

//MAIN
int main(void)
{
    short opcion, ingreso; //Variables para ingreso y validación de datos.

    char buffer_cartel[150];

    Carga_buffer_archivo(ARCHIVO_CLIENTES, sizeof(cli));
    Carga_buffer_archivo(ARCHIVO_CATEGORIAS, sizeof(cat));
    Carga_buffer_archivo(ARCHIVO_MODIFICADORES, sizeof(mod));
    Carga_buffer_archivo(ARCHIVO_VENTAS, sizeof(ven));

//=================================== MENU PRINCIPAL DE OPCIONES ===============================================
    while(1)
    {
        do
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "SISTEMA DE GESTI", 162, "N PARA LA BASE DE DATOS DE CLIENTES");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s%c%s", "MEN", 163, " PRINCIPAL");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s%c%s", "Gesti", 162, "n de la base de datos."
                                                      "Gesti", 162, "n de clientes."
                                                      "Gesti", 162, "n de las categor", 161, "as de clientes."
                                                      "Gesti", 162, "n de ventas y pagos."
                                                      "Consultas a la base de datos");

            Imprime_cartel(buffer_cartel, MENU);

            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &opcion);
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
            case 1:

                Menu_gestion_base_datos();

                break;

            case 2:

                Menu_gestion_clientes();

                break;

            case 3:

                Menu_gestion_categorias();

                break;

            case 4:

                Menu_gestion_ventas();

                break;

            case 5:

                Menu_consultas();

                break;

            case 0:
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%c%s", 173, "HASTA PRONTO!");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, ENCABEZADO);
                printf("\n\n\n\n\n");

                PAUSA
                return 0;
        }
    }
}

//====================== MENÚ de MANEJO de ARCHIVOS=========================

void Menu_gestion_base_datos()
{
    short opcion, ingreso; //Variables para ingreso y validación de datos.

    char buffer_cartel[180];

    while(1)
    {
        do
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s", "MEN", 163, " de GESTI", 162, "N de la BASE de DATOS");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s", "Crear/reiniciar archivos de la base de datos."
                                  "Guardar una copia de la actual base de datos."
                                  "Cargar una base de datos guardada previamente");

            Imprime_cartel(buffer_cartel, MENU);

            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &opcion);
            fflush(stdin);

            if(opcion < 0 || opcion > 4 || ingreso == 0)
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 4.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n\n");
                PAUSA
            }
        }while(opcion < 0 || opcion > 4 || ingreso == 0);


        switch(opcion)
        {
            case 1:

                if(Crear_archivos())
                {
                    sprintf(buffer_cartel, "%s", "Los archivos fueron reinicializados exitosamente y cargados a la base de datos vigente.");

                    BORRAR_PANTALLA
                    printf("\n\n\n\n\n\n\n");
                    Imprime_cartel(buffer_cartel, DOBLE);
                    printf("\n\n\n\n\n");
                }

                PAUSA
                break;

            case 2:
                BORRAR_PANTALLA

                if(Guardar_BD())
                {
                    BORRAR_PANTALLA

                    sprintf(buffer_cartel, "%s", "La base de datos actual se ha guardado correctamente.");

                    printf("\n\n\n\n\n\n\n");
                    Imprime_cartel(buffer_cartel, DOBLE);
                    printf("\n\n\n\n");
                }

                PAUSA
                break;

            case 3:
                BORRAR_PANTALLA

                if(Cargar_BD())
                {
                    BORRAR_PANTALLA

                    sprintf(buffer_cartel, "%s", "La base de datos fue cargada correctamente.");

                    printf("\n\n\n\n\n\n\n");
                    Imprime_cartel(buffer_cartel, DOBLE);
                }

                printf("\n\n\n\n");
                PAUSA
                break;

            case 0:
                return;
        }
    }
}

//=================== MENÚ de GESTIÓN de CLIENTES===================================

void Menu_gestion_clientes()
{
    short opcion, ingreso; //Variables para ingreso y validación de datos.
    char buffer_cartel[200];

    unsigned int id_registro, *registros_activos, contador;

    while(1)
    {
        do
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s", "MEN", 163, " de GESTI", 162, "N de CLIENTES");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s", "Mostrar lista de clientes."
                                  "Alta nuevos clientes."
                                  "Modificar datos de clientes");

            Imprime_cartel(buffer_cartel, MENU);

            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &opcion);
            fflush(stdin);

            if(opcion < 0 || opcion > 4 || ingreso == 0)
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 4.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n\n");
                PAUSA
            }
        }while(opcion < 0 || opcion > 4 || ingreso == 0);


        switch(opcion)
        {
            case 1:
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s", "LISTA de CLIENTES");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                if((registros_activos = calloc(tam_buffer_clientes, sizeof(unsigned int))) == NULL)
                {
                    printf("No hay memoria disponible para ejecutar esta operaci%cn", 162);
                    PAUSA
                    return;
                }

                contador = 0;

                for(int i = 1; i < tam_buffer_clientes; i++)
                    if(buffer_clientes[i].id)
                        registros_activos[contador++] = i;

                Desplegar_clientes(registros_activos, contador);

                free(registros_activos);

                PAUSA
                break;

            case 2:
                BORRAR_PANTALLA

                if((id_registro = Agregar_nuevo_cliente()))
                {
                    BORRAR_PANTALLA

                    sprintf(buffer_cartel, "%s", "NUEVO CLIENTE");

                    Imprime_cartel(buffer_cartel, ENCABEZADO);

                    Desplegar_clientes(&id_registro, 1);
                }

                PAUSA
                break;

            case 3:
                BORRAR_PANTALLA

                Modificar_cliente();

                PAUSA
                break;
            case 4:
                break;

            case 0:
                return;
        }
    }
}

//=================== MENÚ de GESTIÓN de las CATEGORÍAS de CLIENTES y sus MODIFICADORES ===========================================

void Menu_gestion_categorias()
{
    short opcion, ingreso; //Variables para ingreso y validación de datos.
    char buffer_cartel[250];

    unsigned int id_registro, *registros_activos, contador;

    while(1)
    {
        do
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "MEN", 163, " de GESTI", 162, "N de las CATEGOR", 161, "AS de CLIENTES");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s%c%s%c%s", "Mostrar categor", 161, "as vigentes."
                                  "Agregar nueva categor", 161, "a."
                                  "Modificar/eliminar categor", 161, "a."
                                  "Mostrar modificadores de categor", 161, "as vigentes."
                                  "Agregar modificadores de categor", 161, "as."
                                  "Modificar/eliminar modificadores de categor", 161, "as existentes");

            Imprime_cartel(buffer_cartel, MENU);

            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &opcion);
            fflush(stdin);

            if(opcion < 0 || opcion > 6 || ingreso == 0)
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 6.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n\n");
                PAUSA
            }
        }while(opcion < 0 || opcion > 6 || ingreso == 0);

        switch(opcion)
        {
            case 1:
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s", "CATEGOR", 161, "AS VIGENTES");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                if((registros_activos = calloc(tam_buffer_categorias, sizeof(unsigned int))) == NULL)
                {
                    printf("No hay memoria disponible para ejecutar esta operaci%cn", 162);
                    PAUSA
                    return;
                }

                contador = 0;

                for(int i = 1; i < tam_buffer_categorias; i++)
                    if(buffer_categorias[i].id)
                        registros_activos[contador++] = i;

                Desplegar_categorias(registros_activos, contador);

                free(registros_activos);

                PAUSA
                break;

            case 2:
                BORRAR_PANTALLA

                if((id_registro = Agregar_nueva_categoria()))
                {
                    BORRAR_PANTALLA

                    sprintf(buffer_cartel, "%s%c%s", "NUEVA CATEGOR", 161, "A");

                    Imprime_cartel(buffer_cartel, ENCABEZADO);

                    Desplegar_categorias(&id_registro, 1);
                }

                PAUSA

                break;

            case 3:
                BORRAR_PANTALLA

                Modificar_categoria();

                PAUSA
                break;

            case 4:
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s", "MODIFICADORES VIGENTES");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                if((registros_activos = calloc(tam_buffer_modificadores, sizeof(unsigned int))) == NULL)
                {
                    printf("No hay memoria disponible para ejecutar esta operaci%cn", 162);
                    PAUSA
                    return;
                }

                for(int i = 1; i < tam_buffer_modificadores; i++)
                    if(buffer_modificadores[i].id)
                        registros_activos[contador++] = i;

                Desplegar_modificadores(registros_activos, contador);

                free(registros_activos);
                contador = 0;

                PAUSA
                break;

            case 5:
                BORRAR_PANTALLA

                Agregar_nuevo_modificador();

                PAUSA
                break;

            case 6:
                BORRAR_PANTALLA

                Modificar_modificador();

                PAUSA

                break;
            case 0:
                return;
        }
    }
}

//=================== MENÚ de GESTIÓN de los DATOS sobre VENTAS y PAGOS =============================================

void Menu_gestion_ventas()
{
    short opcion, ingreso; //Variables para ingreso y validación de datos.
    char buffer_cartel[200];

    unsigned int id_registro, *registros_activos, contador;

    while(1)
    {
        do
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s", "MEN", 163, " de GESTI", 162, "N de VENTAS y COBROS");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s", "Mostrar ventas."
                                  "Introducir nueva venta."
                                  "Corregir/actualizar/anular venta."
                                  "Introducir nuevo pago");

            Imprime_cartel(buffer_cartel, MENU);

            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &opcion);
            fflush(stdin);

            if(opcion < 0 || opcion > 4 || ingreso == 0)
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 4.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n\n");
                PAUSA
            }
        }while(opcion < 0 || opcion > 4 || ingreso == 0);


        switch(opcion)
        {
            case 1:
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s", "VENTAS REGISTRADAS");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                if((registros_activos = calloc(tam_buffer_ventas, sizeof(unsigned int))) == NULL)
                {
                    printf("No hay memoria disponible para ejecutar esta operaci%cn", 162);
                    PAUSA
                    return;
                }

                for(int i = 1; i < tam_buffer_ventas; i++)
                    if(buffer_ventas[i].id)
                        registros_activos[contador++] = i;

                Desplegar_ventas(registros_activos, contador);

                free(registros_activos);
                contador = 0;

                PAUSA
                break;

            case 2:
                BORRAR_PANTALLA

                if((id_registro = Agregar_nueva_venta()))
                {
                    BORRAR_PANTALLA

                    sprintf(buffer_cartel, "%s", "VENTA REGISTRADA");

                    Imprime_cartel(buffer_cartel, ENCABEZADO);

                    Desplegar_ventas(&id_registro, 1);
                }

                PAUSA
                break;

            case 3:
                BORRAR_PANTALLA

                Modificar_venta();

                PAUSA
                break;

            case 4:
                BORRAR_PANTALLA

                Introducir_pago();

                PAUSA
                break;

            case 0:
                return;
        }
    }
}

//=================== MENÚ para CONSULTAS a la BASE de DATOS ========================================

void Menu_consultas()
{
    short opcion, ingreso; //Variables para ingreso y validación de datos.
    char buffer_cartel[380];

    while(1)
    {
        do
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "MEN", 163, " de CONSULTAS a la BASE de DATOS");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s",
                                    "Lista de clientes por l", 161, "mite de facturaci", 162, "n."
                                    "Lista de clientes por l", 161, "mite de deuda."
                                    "Ventas correspondientes a un cliente."
                                    "Lista de ventas por l", 161, "mite de su monto base."
                                    "Lista de ventas por l", 161, "mite de su monto neto."
                                    "Facturaci", 162, "n total (bruta y neta), y facturaci", 162, "n por categor", 161, "a."
                                    "Deuda total"

                                    );

            Imprime_cartel(buffer_cartel, MENU);

            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &opcion);
            fflush(stdin);

            if(opcion < 0 || opcion > 9 || ingreso == 0)
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 9.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n\n");
                PAUSA
            }
        }while(opcion < 0 || opcion > 9 || ingreso == 0);


        switch(opcion)
        {
            case 1:
                BORRAR_PANTALLA

                Consulta_clientes_X_facturacion();

                PAUSA
                break;

            case 2:
                BORRAR_PANTALLA

                Consulta_clientes_X_deuda();

                PAUSA
                break;

            case 3:
                BORRAR_PANTALLA

                Consulta_ventas_X_cliente();

                PAUSA
                break;

            case 4:
                BORRAR_PANTALLA

                Consulta_ventas_X_montoBase();

                PAUSA
                break;

            case 5:
                BORRAR_PANTALLA

                Consulta_ventas_X_montoNeto();

                PAUSA
                break;

            case 6:
                BORRAR_PANTALLA

                Facturacion_total();

                Facturacion_total_X_categoria();

                PAUSA
                break;

            case 7:
                BORRAR_PANTALLA

                Calcular_deuda_total();

                PAUSA
                break;

            case 0:
                return;
        }
    }
}
