#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lib_modificadores.h"
#include "lib_categorias.h"
#include "lib_auxiliares.h"
#include "lib_bd.h"

unsigned int Agregar_nuevo_modificador()
{
    mod nuevo_modificador = {0, "", PORCENTUAL, 0.00}; // buffer para grabar los datos del nuevo modificador;

    unsigned int id_agregado; //Toma el ID del nuevo modificador

    short ingreso;

    char buffer_cartel[180];


//SOLICITA NOMBRE DE MODIFICADOR

    sprintf(buffer_cartel, "%s", "INGRESO DE NUEVO MODIFICADOR");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s%c%s", "Ingrese el NOMBRE del nuevo modificador"
                            "(m", 160, "ximo ", LONGITUD_NOMBRE_MODIFICADOR - 2, " caracteres), o 0 para volver al men",
                            163, " de gesti", 162, "n de categor", 161, "as.");
    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_MODIFICADOR - 1, "[^\n]");
        scanf(buffer_cartel, nuevo_modificador.nombre);
        fflush(stdin);

        if(!strcmp(nuevo_modificador.nombre, "0"))
            return 0;

        if(!Validar_cadena(nuevo_modificador.nombre, LONGITUD_NOMBRE_MODIFICADOR - 2, IMP_SI, ALFANUMERICO))
        {
            id_agregado = 1;
            nuevo_modificador.nombre[0] = '\0';
        }
        else if(Buscar_modificador_X_nombre(&id_agregado, nuevo_modificador.nombre, COIN_EXACTA))
        {
            sprintf(buffer_cartel, "%s", "Ya existe un modificador con el NOMBRE solicitado. Por favor, proponga otro distinto.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);

            nuevo_modificador.nombre[0] = '\0';
        }
    }while(id_agregado);


//SOLICITA TIPO DE MODIFICADOR

    sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingrese el TIPO del nuevo modificador (1 = porcentual; 2 = fijo),"
                            " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%d", (int*)&nuevo_modificador.tipo);
        fflush(stdin);

        if(nuevo_modificador.tipo == 0 && ingreso)
            return 0;

        if(nuevo_modificador.tipo < 0 || nuevo_modificador.tipo > 2 || ingreso == 0)
        {
            sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 2.");

            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n");
        }

    }while(nuevo_modificador.tipo < 0 || nuevo_modificador.tipo > 2 || ingreso == 0);


//SOLICITA EL VALOR DEL MODIFICADOR

    sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingrese el VALOR del nuevo modificador ('-' = descuenta; '+' = aumenta),"
                            " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do
    {
        PEDIR_INGRESO

        ingreso = scanf("%f", &nuevo_modificador.valor);
        fflush(stdin);

        if(!ingreso)
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n");

        }else if(nuevo_modificador.valor == 0)
            return 0;

    }while(!ingreso);


//INTENTA EL REGISTRO DEL MODIFICADOR

    if((id_agregado = Insertar_registro(ARCHIVO_MODIFICADORES, &nuevo_modificador)) > NRO_MODIFICADORES - 1)
    {
        if(Eliminar_registro(ARCHIVO_MODIFICADORES, id_agregado))
        {
            sprintf(buffer_cartel, "%s%c%s%c%s", "Se ha alcanzado el l", 161, "mite de modificadores"
                                    " permitidos. Solicite al programador una ampliaci", 162,
                                    "n, si no quiere eliminar modificadores existentes.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n");

            return 0;
        }else
        {
            sprintf(buffer_cartel, "%s%c%s", "Se ha alcanzado el l", 161, "mite de modificadores"
                                    "permitidos, y hubo un error en la reescritura del archivo. Verifique el estado de la lista de modificadores.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n");

            return 0;
        }
    }else
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%c%s", "El nuevo modificador fue incorporado exitosamente a la base de datos. ", 168,
                                        "Desea asociar alguna categor", 161, "a con el nuevo modificador? (s/n)");

        printf("\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);

    //Eventual vinculación del nuevo modificador con alguna categoría ya registrada
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
                                                "Desea asociar alguna categor", 161, "a con el nuevo modificador? (s/n)");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
            }

        }while(ingreso == 0 || (buffer_cartel[0] != 's' && buffer_cartel[0] != 'n'));

        if(buffer_cartel[0] == 's')
            Vincular_modificador_categoria(id_agregado);

        return id_agregado; //Devuelve el ID atribuido al nuevo modificador.
    }
}

//=========================================================================================================

short Modificar_modificador()
{
    char registro_ingresado[LONGITUD_NOMBRE_MODIFICADOR];
    t_mod tipo_ingresado;
    float valor_ingresado;

    unsigned int opcion, id_modificar, busqueda_ids[NRO_CATEGORIAS];

    char buffer_cartel[200];

    short ingreso;

    void *ptr_buffer = buffer_modificadores, *ptr_dato; //Punteros para obtener el desplazamiento del dato a modificar en el archivo.

    sprintf(buffer_cartel, "%s%c%s", "ALTERAR MODIFICADOR de CATEGOR", 161, "A");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    id_modificar = Pedir_modificador(); //Pide el nombre o el ID del modificador a modificar.

    do //Despliega las opciones de modificación
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "MODIFICADOR SELECCIONADO");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        Desplegar_modificadores(&id_modificar, 1);

        sprintf(buffer_cartel, "%s", "ALTERAR MODIFICADOR");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        sprintf(buffer_cartel, "%s%c%s", "Cambiar NOMBRE."
                                            "Cambiar TIPO."
                                            "Modificar VALOR."
                                            "Asociar/disociar CATEGOR", 161, "AS."
                                            "ELIMINAR modificador");

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
    case 1: //Opción "Cambiar nombre"

        sprintf(buffer_cartel, "%s%c%s%d%s%c%s%c%s%c%s", "Ingrese el nuevo NOMBRE del modificador (m", 160, "ximo ", LONGITUD_NOMBRE_MODIFICADOR - 2, " caracteres),"
                                    " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_MODIFICADOR - 1, "[^\n]");
            scanf(buffer_cartel, registro_ingresado);
            fflush(stdin);

            if(!strcmp(registro_ingresado, "0"))
                return 0;

            if(!Validar_cadena(registro_ingresado, LONGITUD_NOMBRE_MODIFICADOR - 2, IMP_SI, ALFANUMERICO))
            {
                busqueda_ids[0] = 1;
                registro_ingresado[0] = '\0';
            }
            else if(Buscar_modificador_X_nombre(busqueda_ids, registro_ingresado, COIN_EXACTA))
            {
                sprintf(buffer_cartel, "%s", "Ya existe un modificador con el nombre solicitado."
                                                "Por favor, proponga otro distinto.");

                printf("\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);

                registro_ingresado[0] = '\0';
            }
        }while(busqueda_ids[0]);

        ptr_dato = buffer_modificadores[id_modificar].nombre; //Apunta a la dirección del nombre a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_MODIFICADORES, (size_t)(ptr_dato - ptr_buffer), registro_ingresado, LONGITUD_NOMBRE_MODIFICADOR))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "MODIFICADOR SELECCIONADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_modificadores(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El modificador seleccionado fue alterado exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 2: //Opción "Cambiar tipo"

        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingrese el TIPO deseado para el modificador seleccionado (1 = porcentual; 2 = fijo),"
                            " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            ingreso = scanf("%d", (int*) &tipo_ingresado);
            fflush(stdin);

            if(tipo_ingresado == 0 && ingreso)
                return 0;

            if(tipo_ingresado < 0 || tipo_ingresado > 2 || ingreso == 0)
            {
                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 2.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n\n\n\n");
            }
        }while(tipo_ingresado < 0 || tipo_ingresado > 2 || ingreso == 0);

        ptr_dato = &buffer_modificadores[id_modificar].tipo; //Apunta a la dirección del dato a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_MODIFICADORES, (size_t)(ptr_dato - ptr_buffer), &tipo_ingresado, sizeof(t_mod)))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "MODIFICADOR SELECCIONADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_modificadores(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El modificador seleccionado fue alterado exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 3: //Opción "Modificar valor".

        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingrese el nuevo VALOR para el modificador seleccionado ('-' = descuenta; '+' = aumenta),"
                            " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            ingreso = scanf("%f", &valor_ingresado);
            fflush(stdin);

            if(valor_ingresado == 0 && ingreso)
                return 0;

            if(!ingreso)
            {
                sprintf(buffer_cartel, "%s%c%s", "Ingreso incorrecto. Debe introducir un n", 163, "mero real.");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
                printf("\n\n");
            }

        }while(!ingreso);

        ptr_dato = &buffer_modificadores[id_modificar].valor; //Apunta a la dirección del dato a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_MODIFICADORES, (size_t)(ptr_dato - ptr_buffer), &valor_ingresado, sizeof(float)))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s", "MODIFICADOR SELECCIONADO");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_modificadores(&id_modificar, 1);

            sprintf(buffer_cartel, "%s", "El modificador seleccionado fue alterado exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 4: //Opción "Asociar/disociar categoría"

        BORRAR_PANTALLA

        Vincular_modificador_categoria(id_modificar);

        break;
    case 5: //Opción "Eliminar modificador"

        if(Eliminar_registro(ARCHIVO_MODIFICADORES, id_modificar)) //Eliminación
        {
            BORRAR_PANTALLA
            sprintf(buffer_cartel, "%s", "El modificador seleccionado fue eliminado exitosamente.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");

            for(int i = 1; i < tam_buffer_categorias; i++) //Recorre todas las categorías, para disociarlas del modificador eliminado.
                if(buffer_categorias[i].id && buffer_categorias[i].modificadores[id_modificar])
                    if(!Alternar_vinculo_categoria_modificador(i, id_modificar))
                    {
                        sprintf(buffer_cartel, "%s%c%s%c%s", "Hubo un error en la disociaci", 162, "n del modificador eliminado con sus categor", 161, "as asociadas. Por favor, pruebe a disociarlas manualmente.\n\n");

                        Imprime_cartel(buffer_cartel, SIMPLE);
                    }
        }

        break;
    }
    return 1; //Devuelve 1 si ha pasado todas las validaciones.
}

//=========================================================================================================

void Desplegar_modificadores(unsigned int ids[NRO_CATEGORIAS], size_t tamanio_ids)
{
    char formato_encabezado[80]; //Buffers de los formatos de salida para las distintas filas de la tabla a desplegar.
    char formato_fila[50];
    char formato_fila_categ[50];

    unsigned int categorias_asociadas[NRO_CATEGORIAS], contador; //Guarda las categorías a ser desplegadas dentro de una misma fila.

    sprintf(formato_encabezado, "%s%d%s%d%s%d%s", "%c %", 8, "s %c %", LONGITUD_NOMBRE_MODIFICADOR, "s %c %10s %c %12s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c");
    sprintf(formato_fila, "%s%d%s%d%s%d%s", "%c %", 8, "u %c %", LONGITUD_NOMBRE_MODIFICADOR, "s %c %10s %c %12.2f %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c");
    sprintf(formato_fila_categ, "%s%d%s%d%s%d%s", "%c %", 8, "s %c %", LONGITUD_NOMBRE_MODIFICADOR, "s %c %10s %c %12s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c");

//ENCABEZADOS
    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218,196,196,196,196,196,196,196,196,196,196,194,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
        printf("%c", 196);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 194,196,196,196,196,196,196,196,196,196,196,196,196,194,196,196,196,196,196,196,196,196,196,196,196,196,196,196,194,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
        printf("%c", 196);
    printf("%c", 191);

    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf(formato_encabezado, 179, "ID", 179, "MODIFICADOR", 179, "TIPO", 179, "VALOR", 179, "CATEG. ASOCIADAS", 179);

    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
        printf("%c", 196);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 197,196,196,196,196,196,196,196,196,196,196,196,196,197,196,196,196,196,196,196,196,196,196,196,196,196,196,196,197,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
        printf("%c", 196);
    printf("%c", 180);

 //FILAS
    for(int i = 0; i < tamanio_ids; i++)
    {
        contador = 0;

        for(int j = 1; j < tam_buffer_categorias; j++) //Almacena las categorías asociadas a cada modificador.
            if(buffer_categorias[j].modificadores[ids[i]])
                categorias_asociadas[contador++] = j;

        categorias_asociadas[contador] = 0; //Coloco un cero en una posición más para reconocer el fin del arreglo.

        putchar('\n');
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196);
        for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
            printf("%c", 196);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 197,196,196,196,196,196,196,196,196,196,196,196,196,197,196,196,196,196,196,196,196,196,196,196,196,196,196,196,197,196,196);
        for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
            printf("%c", 196);
        printf("%c", 180);

        putchar('\n');
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

    //Datos de cada modificador
        printf(formato_fila, 179, buffer_modificadores[ids[i]].id, 179, buffer_modificadores[ids[i]].nombre,
                179, buffer_modificadores[ids[i]].tipo == 1 ? "Porcentual" : "Fijo", 179, buffer_modificadores[ids[i]].valor,
                179, categorias_asociadas[0] != 0 ? buffer_categorias[categorias_asociadas[0]].nombre : "S/C", 179);

        for(int i = 1; categorias_asociadas[i] != 0; i++) //Imprime más renglones dentro de una misma fila, para enlistar las categorías.
        {
            putchar('\n');
            for(int i = 0; i < ESPACIOS_MARGEN; i++)
                putchar(' ');

            printf(formato_fila_categ, 179, "", 179, "", 179, "", 179, "", 179, buffer_categorias[categorias_asociadas[i]].nombre, 179);
        }
    }

//Cierre de la tabla
    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192,196,196,196,196,196,196,196,196,196,196,193,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
        printf("%c", 196);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 193,196,196,196,196,196,196,196,196,196,196,196,196,193,196,196,196,196,196,196,196,196,196,196,196,196,196,196,193,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
        printf("%c", 196);
    printf("%c", 217);

    printf("\n\n");
}

//======================================================================================================

//Siempre se debe validar la cadena antes de pasarla como argumento a esta función
unsigned int Buscar_modificador_X_nombre(unsigned int buffer_ids[NRO_MODIFICADORES], char nombre[LONGITUD_NOMBRE_CATEGORIA], pre_b precision)
{
    size_t longitud_nombre_ingresado;

    unsigned int id, contador = 0;

    buffer_ids[0] = 0; //Esta inicialización garantiza que, si no se encuentran coincidencias, la dirección de memoria
                        //recibida como buffer queda nula.

    if(precision == COIN_EXACTA) //Devuelve la primera coincidencia exacta con el nombre ingresado
    {
        for(id = 1; id < tam_buffer_modificadores; id++)
            if(!strcmp(buffer_modificadores[id].nombre, nombre))
            {
                buffer_ids[0] = id;
                return 1;
            }
    }else //Devuelve la lista de todos los modificadores que comienzan con el nombre indicado.
    {
        longitud_nombre_ingresado = strlen(nombre);

        for(id = 1; id < tam_buffer_modificadores; id++)
            if(!strncmp(buffer_modificadores[id].nombre, nombre, longitud_nombre_ingresado)) // Evalúo en todos los casos si el número de coincidencias entre
                                                                                        // la cadena ingresada y los primeros caracteres del campo APELLIDO
                                                                                        // de cada registro coincide con la longitud de la cadena ingresada.
            {
                buffer_ids[contador++] = id;
            }
    }

    return contador; //Devuelve la cantidad de coincidencias.
}

//=======================================================================================================

short Vincular_modificador_categoria(unsigned int id_modificador)
{
    char confirmacion;

    char buffer_cartel[180];

    unsigned int id_categoria;

    short ingreso;

    do
    {
        id_categoria = Pedir_categoria(); //Pregunta por la categoría a vincular o desvincular.

        if(Alternar_vinculo_categoria_modificador(id_categoria, id_modificador)) //Realiza la operación
        {
            BORRAR_PANTALLA

            Desplegar_categorias(&id_categoria, 1);

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
            sprintf(buffer_cartel, "%c%s%c%s", 168, "Desea asociar o disociar otra categor", 161, "a con el modificador seleccionado? (s/n)");

            Imprime_cartel(buffer_cartel, SIMPLE);

            PEDIR_INGRESO

            ingreso = scanf("%c", &confirmacion);
            fflush(stdin);

            confirmacion = tolower(confirmacion);

            if(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'))
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.", 168,
                                                "Desea asociar/disociar alguna categor", 161, "a con el modificador seleccionado? (s/n)");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
            }
        }while(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'));

    }while(confirmacion != 'n');

    return 1; //Devuelve 1 si las operaciones se han realizado con éxito.
}

unsigned int Pedir_modificador()
{
    char modificador_buscado[LONGITUD_NOMBRE_MODIFICADOR];

    unsigned int opcion, id_modificador, busqueda_ids[tam_buffer_modificadores];

    char buffer_cartel[180];

    short ingreso;

    sprintf(buffer_cartel, "%s%c%s", "Ingrese el NOMBRE o el ID del modificador deseado, o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do //Pide el nombre o el ID del modificador a buscar.
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_MODIFICADOR - 1, "[^\n]");
        scanf(buffer_cartel, modificador_buscado);
        fflush(stdin);

        if(!strcmp(modificador_buscado, "0"))
            return 0;

        if((id_modificador = Validar_cadena_como_entero(modificador_buscado))) //Verifica si se ingresó un ID.
        {
            if(id_modificador >= tam_buffer_modificadores || !buffer_modificadores[id_modificador].id) //Valida el ID.
            {
                sprintf(buffer_cartel, "%s", "No existe un modificador con el ID indicado. Por favor, revise la lista de modificadores vigente.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                id_modificador = 0;
            }
        }
        else if(Validar_cadena(modificador_buscado, LONGITUD_NOMBRE_MODIFICADOR - 2, IMP_NO, ALFANUMERICO)) //Si no se ingresó un ID válido, valida la cadena como NOMBRE.
        {
            if(!(id_modificador = Buscar_modificador_X_nombre(busqueda_ids, modificador_buscado, COIN_PARCIAL))) //Busca el nombre.
            {
                sprintf(buffer_cartel, "%s", "No existe un modificador con el NOMBRE indicado. Por favor, revise la lista de modificadores vigente.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                id_modificador = 0;

            }else if(id_modificador == 1) //Si hubo una sola coincidencia, se guarda su ID.
                id_modificador = busqueda_ids[0];
            else //Si hubo muchas coincidencias, se muestra la lista y se solicita indicar el ID correcto.
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s", "MODIFICADORES QUE COINCIDEN CON LA B", 163, "SQUEDA");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                Desplegar_modificadores(busqueda_ids, id_modificador);

                sprintf(buffer_cartel, "%s%c%s", "Ingrese el ID correspondiente al modificador buscado,"
                                                " o 0 para volver al men", 163, " anterior.");

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

                    }else if(!buffer_modificadores[opcion].id || opcion >= tam_buffer_modificadores)
                    {
                        sprintf(buffer_cartel, "%s%c%s", "No existe ning", 161, "n modificador con el ID ingresado.");

                        Imprime_cartel(buffer_cartel, SIMPLE);

                        ingreso = 0;
                    }else
                        id_modificador = opcion;

                }while(opcion < 0 || !ingreso);
            }
        }else
        {
            sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingreso inv", 160, "lido. Este campo admite s", 162, "lo letras y n", 163, "meros.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            id_modificador = 0;
        }

        if(!id_modificador)
            modificador_buscado[0] = '\0';

    }while(!id_modificador);

    return id_modificador; //Devuelve el ID del modificador solicitado.
}
