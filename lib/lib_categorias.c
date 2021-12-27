#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "definiciones.h"

#include "lib_categorias.h"
#include "lib_modificadores.h"
#include "lib_bd.h"
#include "lib_auxiliares.h"

unsigned int Agregar_nueva_categoria()
{
    cat nueva_categoria = {0, "", {0}}; // buffer para grabar los datos de la nueva categoría;

    unsigned int id_agregado; //Toma el ID de la nueva categoría

    short ingreso;

    char buffer_cartel[160];

    sprintf(buffer_cartel, "%s%c%s", "INGRESO DE NUEVA CATEGOR", 161, "A");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

//PIDE NOMBRE
    sprintf(buffer_cartel, "%s%c%s%c%s%d%s%c%s%c%s%c%s", "Ingrese el NOMBRE de la nueva categor", 161,
                            "a (m", 160, "ximo ", LONGITUD_NOMBRE_CATEGORIA - 2, " caracteres), o 0 para volver al men",
                            163, " de gesti", 162, "n de categor", 161, "as.");

    Imprime_cartel(buffer_cartel, SIMPLE);

    do
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_CATEGORIA - 1, "[^\n]");
        scanf(buffer_cartel, nueva_categoria.nombre);
        fflush(stdin);

        if(!strcmp(nueva_categoria.nombre, "0"))
            return 0;

        if(!Validar_cadena(nueva_categoria.nombre, LONGITUD_NOMBRE_CATEGORIA - 2, IMP_SI, ALFANUMERICO))
        {
            id_agregado = 1;
            nueva_categoria.nombre[0] = '\0';
        }
        else if(Buscar_categoria_X_nombre(&id_agregado, nueva_categoria.nombre, COIN_EXACTA))
        {
            sprintf(buffer_cartel, "%s%c%s", "Ya existe una categor", 161, "a con el NOMBRE solicitado. Por favor, proponga otro distinto.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);

            nueva_categoria.nombre[0] = '\0';
        }
    }while(id_agregado);

//Registro de la nueva categoría
    if((id_agregado = Insertar_registro(ARCHIVO_CATEGORIAS, &nueva_categoria)) > NRO_CATEGORIAS)
    {
        if(Eliminar_registro(ARCHIVO_CATEGORIAS, id_agregado))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s", "Se ha alcanzado el l", 161, "mite de categor", 161,
                                    "as permitidas. Solicite al programador una ampliaci", 162,
                                    "n, si no quiere eliminar categor", 161, "as existentes.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n");

            return 0;
        }else
        {
            sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Se ha alcanzado el l", 161, "mite de categor", 161,
                                    "as permitidas, y hubo un error en la reescritura del archivo. Verifique el estado de la lista de categor", 161, "as.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n");

            return 0;
        }
    }else
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s", "La nueva categor", 161, "a fue incorporada exitosamente a la base de datos. ", 168,
                                        "Desea asociar alg", 163, "n modificador con la nueva categor", 161, "a? (s/n)");

        printf("\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);

    //Eventual vinculación de la nueva categoría con algún modificador ya registrado.
        do
        {
            PEDIR_INGRESO

            ingreso = scanf("%c", &buffer_cartel[0]);
            fflush(stdin);

            buffer_cartel[0] = tolower(buffer_cartel[0]);

            if(ingreso == 0 || (buffer_cartel[0] != 's' && buffer_cartel[0] != 'n'))
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.", 168,
                                                "Desea asociar alg", 163, "n modificador con la nueva categor", 161, "a? (s/n)");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
            }

        }while(ingreso == 0 || (buffer_cartel[0] != 's' && buffer_cartel[0] != 'n'));

        if(buffer_cartel[0] == 's')
            Vincular_categoria_modificador(id_agregado);

        return id_agregado; //Devuelve el ID atribuido a la nueva categoría.
    }
}

//=========================================================================================================

short Modificar_categoria()
{
    char registro_ingresado[LONGITUD_NOMBRE_CATEGORIA];

    unsigned int opcion, id_modificar, busqueda_ids[NRO_CATEGORIAS];

    char buffer_cartel[200];

    short ingreso;

    void *ptr_buffer = buffer_categorias, *ptr_dato; //Punteros para obtener el desplazamiento del dato a modificar en el archivo.

    sprintf(buffer_cartel, "%s%c%s", "MODIFICAR CATEGOR", 161, "A");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    id_modificar = Pedir_categoria(); //Pide el nombre o el ID de la categoría a modificar.

    do //Despliega las opciones de modificación
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s", "CATEGOR", 161, "A SELECCIONADA");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        Desplegar_categorias(&id_modificar, 1);

        sprintf(buffer_cartel, "%s%c%s", "MODIFICAR CATEGOR", 161, "A");

        Imprime_cartel(buffer_cartel, ENCABEZADO);

        sprintf(buffer_cartel, "%s%c%s", "Cambiar NOMBRE."
                                            "Agregar/remover MODIFICADORES."
                                            "ELIMINAR categor", 161, "a");

        Imprime_cartel(buffer_cartel, MENU);

        PEDIR_INGRESO

        ingreso = scanf("%d", &opcion);
        fflush(stdin);

        if(opcion < 0 || opcion > 3 || ingreso == 0)
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s%c%s", "Ingreso incorrecto. Debe introducir una opci", 162, "n v", 160, "lida entre 0 y 3.");

            printf("\n\n\n\n\n\n\n");
            Imprime_cartel(buffer_cartel, SIMPLE);
            printf("\n\n\n\n\n");

            PAUSA
        }
    }while(opcion < 0 || opcion > 3 || ingreso == 0);

    switch(opcion)
    {
    case 1: //Opción "Cambiar nombre"
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s%c%s%c%s%d%s%c%s%c%s%c%s", "Ingrese el nuevo NOMBRE de la categor", 161, "a (m", 160, "ximo ", LONGITUD_NOMBRE_CATEGORIA - 2, " caracteres),"
                                    " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

        Imprime_cartel(buffer_cartel, DOBLE);

        do
        {
            PEDIR_INGRESO

            sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_CATEGORIA - 1, "[^\n]");
            scanf(buffer_cartel, registro_ingresado);
            fflush(stdin);

            if(!strcmp(registro_ingresado, "0"))
                return 0;

            if(!Validar_cadena(registro_ingresado, LONGITUD_NOMBRE_CATEGORIA - 2, IMP_SI, ALFANUMERICO))
            {
                busqueda_ids[0] = 1;
                registro_ingresado[0] = '\0';
            }
            else if(Buscar_categoria_X_nombre(busqueda_ids, registro_ingresado, COIN_EXACTA))
            {
                sprintf(buffer_cartel, "%s%c%s", "Ya existe una categor", 161, "a con el nombre solicitado."
                                                "Por favor, proponga otro distinto.");

                printf("\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);

                registro_ingresado[0] = '\0';
            }
        }while(busqueda_ids[0]);

        ptr_dato = buffer_categorias[id_modificar].nombre; //Apunta a la dirección del dato a modificar, para poder calcular su offset.

    //Modificación
        if(Modificar_dato(ARCHIVO_CATEGORIAS, (size_t)(ptr_dato - ptr_buffer), registro_ingresado, LONGITUD_NOMBRE_CATEGORIA))
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "CATEGOR", 161, "A SELECCIONADA");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_categorias(&id_modificar, 1);

            sprintf(buffer_cartel, "%s%c%s", "La categor", 161, "a seleccionada fue modificada exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");
        }else
            return 0;

        break;
    case 2: //Opción "Agregar/remover modificadores"

        BORRAR_PANTALLA

        Vincular_categoria_modificador(id_modificar);

        break;
    case 3: //Opción "Eliminar categoría"
        if(Eliminar_registro(ARCHIVO_CATEGORIAS, id_modificar)) //Eliminación
        {
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%s%c%s", "CATEGOR", 161, "A SELECCIONADA");

            Imprime_cartel(buffer_cartel, ENCABEZADO);

            Desplegar_categorias(&id_modificar, 1);

            sprintf(buffer_cartel, "%s%c%s", "La categor", 161, "a seleccionada fue eliminada exitosamente.");

            printf("\n\n\n");
            Imprime_cartel(buffer_cartel, DOBLE);
            printf("\n\n\n\n");

            for(int i = 1; i < tam_buffer_clientes; i++) //Recorre todos los clientes, para disociarlos de la categoría eliminada.
                if(buffer_clientes[i].id && buffer_clientes[i].categorias[id_modificar])
                    if(!Alternar_vinculo_cliente_categoria(i, id_modificar))
                    {
                        sprintf(buffer_cartel, "%s%c%s%c%s", "Hubo un error en la disociaci", 162, "n de la categor", 161, "a eliminada con sus clientes asociados. Por favor, pruebe a disociarlos manualmente.\n\n");

                        Imprime_cartel(buffer_cartel, SIMPLE);
                    }
        }

        break;
    }

    return 1; //Devuelve 1 si ha pasado todas las validaciones.
}

//=========================================================================================================

void Desplegar_categorias(unsigned int ids[NRO_CATEGORIAS], size_t tamanio_ids)
{
    char formato_encabezado[80]; //Buffers de los formatos de salida para las distintas filas de la tabla a desplegar.
    char formato_fila[40];
    char formato_fila_modif[40];

    unsigned int posicion_primer_modif;

    sprintf(formato_encabezado, "%s%d%s%d%s%d%s", "%c %", 8, "s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c %", LONGITUD_NOMBRE_MODIFICADOR, "s %c");
    sprintf(formato_fila, "%s%d%s%d%s%d%s", "%c %", 8, "u %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c %", LONGITUD_NOMBRE_MODIFICADOR, "s %c");
    sprintf(formato_fila_modif, "%s%d%s%d%s%d%s", "%c %", 8, "s %c %", LONGITUD_NOMBRE_CATEGORIA, "s %c %", LONGITUD_NOMBRE_MODIFICADOR, "s %c");

//ENCABEZADOS
    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218,196,196,196,196,196,196,196,196,196,196,194,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
        printf("%c", 196);
    printf("%c%c%c", 194, 196, 196);
    for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
        printf("%c", 196);
    printf("%c", 191);

    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf(formato_encabezado, 179, "ID", 179, "NOMBRE", 179, "MODIFICADORES", 179);

    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
        printf("%c", 196);
    printf("%c%c%c", 197, 196, 196);
    for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
        printf("%c", 196);
    printf("%c", 180);
//FILAS
    for(int i = 0; i < tamanio_ids; i++)
    {   //Busca el índice del primer modificador asociado para cada categoría.
        for(posicion_primer_modif = 0; !buffer_categorias[ids[i]].modificadores[posicion_primer_modif] && posicion_primer_modif < NRO_MODIFICADORES; posicion_primer_modif++);

        putchar('\n');
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 195,196,196,196,196,196,196,196,196,196,196,197,196,196);
        for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
            printf("%c", 196);
        printf("%c%c%c", 197, 196, 196);
        for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
            printf("%c", 196);
        printf("%c", 180);

        putchar('\n');
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

    //Datos de cada categoría
        printf(formato_fila, 179, buffer_categorias[ids[i]].id, 179, buffer_categorias[ids[i]].nombre, 179, posicion_primer_modif < NRO_MODIFICADORES ? buffer_modificadores[posicion_primer_modif].nombre : "S/M", 179);

        for(int j = ++posicion_primer_modif; j < NRO_MODIFICADORES; j++) //Imprime más renglones dentro de una misma fila, para enlistar los modificadores vinculados con cada categoría.
            if(buffer_categorias[ids[i]].modificadores[j])
            {
                putchar('\n');
                for(int i = 0; i < ESPACIOS_MARGEN; i++)
                    putchar(' ');

                printf(formato_fila_modif, 179, "", 179, "", 179, buffer_modificadores[j].nombre, 179);
            }
    }

//Cierre de la tabla
    putchar('\n');
    for(int i = 0; i < ESPACIOS_MARGEN; i++)
        putchar(' ');

    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192,196,196,196,196,196,196,196,196,196,196,193,196,196);
    for(int i = 0; i < LONGITUD_NOMBRE_CATEGORIA; i++)
        printf("%c", 196);
    printf("%c%c%c", 193, 196, 196);
    for(int i = 0; i < LONGITUD_NOMBRE_MODIFICADOR; i++)
        printf("%c", 196);
    printf("%c", 217);

    printf("\n\n");
}

//======================================================================================================

//Siempre se debe validar la cadena antes de pasarla como argumento a esta función
size_t Buscar_categoria_X_nombre(unsigned int buffer_ids[NRO_CATEGORIAS], char nombre[LONGITUD_NOMBRE_CATEGORIA], pre_b precision)
{
    size_t longitud_nombre_ingresado;

    unsigned int id, contador = 0;

    buffer_ids[0] = 0; //Esta inicialización garantiza que, si no se encuentran coincidencias, la dirección de memoria
                        //recibida como buffer queda nula.

    if(precision == COIN_EXACTA) //Devuelve la primera coincidencia exacta con el nombre ingresado
    {
        for(id = 1; id < tam_buffer_categorias; id++)
            if(!strcmp(buffer_categorias[id].nombre, nombre))
            {
                buffer_ids[0] = id;
                return 1;
            }
    }else //Devuelve la lista de todas las categorías que comienzan con el nombre indicado.
    {
        longitud_nombre_ingresado = strlen(nombre);

        for(id = 1; id < tam_buffer_categorias; id++)
            if(!strncmp(buffer_categorias[id].nombre, nombre, longitud_nombre_ingresado)) // Evalúo en todos los casos si el número de coincidencias entre
                                                                                        // la cadena ingresada y los primeros caracteres del campo APELLIDO
                                                                                        // de cada registro coincide con la longitud de la cadena ingresada.
            {
                buffer_ids[contador++] = id;
            }
    }

    return contador; //Devuelve la cantidad de coincidencias.
}

//========================================================================================================

short Vincular_categoria_modificador(unsigned int id_categoria)
{
    char modificador_buscado[LONGITUD_NOMBRE_MODIFICADOR];

    char buffer_cartel[180];

    unsigned int id_modificador;

    short ingreso;

    do
    {
        id_modificador = Pedir_modificador(); //Pregunta por el modificador a vincular o desvincular.

        if(Alternar_vinculo_categoria_modificador(id_categoria, id_modificador)) //Realiza la operación
        {
            BORRAR_PANTALLA

            Desplegar_modificadores(&id_modificador, 1);

            sprintf(buffer_cartel, "%s", "El modificador seleccionado ha sido asociado/disociado exitosamente.");

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
            BORRAR_PANTALLA

            sprintf(buffer_cartel, "%c%s%c%s", 168, "Desea asociar o disociar otro modificador con la categor", 161, "a seleccionada? (s/n)");

            Imprime_cartel(buffer_cartel, SIMPLE);

            PEDIR_INGRESO

            ingreso = scanf("%c", &modificador_buscado[0]);
            fflush(stdin);

            modificador_buscado[0] = tolower(modificador_buscado[0]);

            if(ingreso == 0 || (modificador_buscado[0] != 's' && modificador_buscado[0] != 'n'))
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.", 168,
                                                "Desea asociar/disociar alg", 163, "n modificador con la categor", 161, "a seleccionada? (s/n)");

                printf("\n\n\n\n\n\n\n");
                Imprime_cartel(buffer_cartel, SIMPLE);
            }
        }while(ingreso == 0 || (modificador_buscado[0] != 's' && modificador_buscado[0] != 'n'));

    }while(modificador_buscado[0] != 'n');

    return 1; //Devuelve 1 si las operaciones se han realizado con éxito.
}

//===================================================================

unsigned int Pedir_categoria()
{
    char registro_ingresado[LONGITUD_NOMBRE_CATEGORIA];

    unsigned int opcion, id_categoria, busqueda_ids[NRO_CATEGORIAS];

    char buffer_cartel[200];

    short ingreso;

    sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s", "Ingrese el NOMBRE o el ID de la categor", 161, "a que desee modificar,"
                                                    " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do //Pide el nombre o el ID de la categoría a buscar.
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_NOMBRE_CATEGORIA - 1, "[^\n]");
        scanf(buffer_cartel, registro_ingresado);
        fflush(stdin);

        if(!strcmp(registro_ingresado, "0"))
            return 0;

        if((id_categoria = Validar_cadena_como_entero(registro_ingresado))) //Verifica si se ingresó un ID.
        {
            if(id_categoria >= tam_buffer_categorias || buffer_categorias[id_categoria].id == 0) //Valida el ID.
            {
                sprintf(buffer_cartel, "%s%c%s%c%s", "No existe ninguna categor", 161, "a con el ID ingresado."
                                                    " Por favor, revise la lista de categor", 161, "as vigente.");

                Imprime_cartel(buffer_cartel, SIMPLE);

                id_categoria = 0;
            }
        }else if(Validar_cadena(registro_ingresado, LONGITUD_NOMBRE_CATEGORIA - 2, IMP_NO, ALFANUMERICO)) //Si no se ingresó un ID válido, valida la cadena como NOMBRE.
        {
            if(!(id_categoria = Buscar_categoria_X_nombre(busqueda_ids, registro_ingresado, COIN_PARCIAL))) //Busca el nombre.
            {
                sprintf(buffer_cartel, "%s%c%s%c%s", "No existe ninguna categor", 161, "a con el NOMBRE ingresado. Por favor, revise la lista de categor", 161, "as vigentes.");

                Imprime_cartel(buffer_cartel, SIMPLE);

            }else if(id_categoria == 1) //Si hubo una sola coincidencia, se guarda su ID.
                id_categoria = busqueda_ids[0];
            else //Si hubo muchas coincidencias, se muestra la lista y se solicita indicar el ID correcto.
            {
                BORRAR_PANTALLA

                sprintf(buffer_cartel, "%s%c%s%c%s", "CATEGOR", 161, "AS QUE COINCIDEN CON LA B", 163, "SQUEDA");

                Imprime_cartel(buffer_cartel, ENCABEZADO);

                Desplegar_categorias(busqueda_ids, id_categoria);

                sprintf(buffer_cartel, "%s%c%s%c%s%c%s%c%s", "Ingrese el ID correspondiente a la categor", 161, "a buscada,"
                                                " o 0 para volver al men", 163, " de gesti", 162, "n de categor", 161, "as.");

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

                    }else if(!buffer_categorias[opcion].id || opcion >= tam_buffer_categorias)
                    {
                        sprintf(buffer_cartel, "%s%c%s", "No existe ninguna categor", 161, "a con el ID ingresado.");

                        Imprime_cartel(buffer_cartel, SIMPLE);

                        ingreso = 0;
                    }else
                        id_categoria = opcion;

                }while(opcion < 0 || !ingreso);
            }
        }else
        {
            sprintf(buffer_cartel, "%s%c%s%c%s%c%s", "Ingreso inv", 160, "lido. Este campo admite s", 162, "lo letras y n", 163, "meros.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            id_categoria = 0;
        }

        if(!id_categoria)
            registro_ingresado[0] = '\0';

    }while(!id_categoria);

    return id_categoria; //Devuelve el ID de la categoría solicitada.
}
