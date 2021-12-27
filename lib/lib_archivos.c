#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "definiciones.h"
#include "lib_archivos.h"
#include "lib_auxiliares.h"

short Crear_archivos()
{
//Registros nulos para inicializar la primera posición de los archivos de datos.
    cli cliente_nulo = {0, "", "", {0}, 0, 0, ""};
    cat categoria_nula = {0, "", {0}};
    mod modificador_nulo = {0, "", PORCENTUAL, 0};
    ven venta_nula = {0, 0, "", 0, 0, 0};

//Variables para rotar los datos en el ciclo de creación e inicialización de los archivos de la base de datos.
    FILE *archivo;  // Handler del archivo


    char nombre_archivo[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO];
    void *ptr_escritura_nulo;
    size_t tipo_nulo;


//Otras variables
    short archivos_creados = 0; //Contador de archivos creados e inicializados exitosamente.
    char buffer_cartel[200];

//Variables para el ingreso y validación de la petición del usuario.
    char confirmacion;
    short ingreso;

    do // Ciclo para que el usuario confirme la creación de la nueva base de datos.
    {
        sprintf(buffer_cartel, "%c%s%c%s%c%s", 168, "Est", 160, " seguro/a que desea generar los archivos de una nueva base de datos? (s/n) Se perder", 160, "n los datos de la actual, si no guarda una copia.");

        Imprime_cartel(buffer_cartel, DOBLE);

        PEDIR_INGRESO

        ingreso = scanf("%c", &confirmacion);
        fflush(stdin);

        confirmacion = tolower(confirmacion);

        if(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'))
        {
            BORRAR_PANTALLA

            printf("\n\n\n\n\n\n\n");
            sprintf(buffer_cartel, "%s", "Ingreso incorrecto. Debe responder con 's' en caso afirmativo, o con 'n' en caso contrario.");
            printf("\n\n\n\n");

            Imprime_cartel(buffer_cartel, SIMPLE);
        }

    }while(ingreso == 0 || (confirmacion != 's' && confirmacion != 'n'));

//Si se confirmó la petición, se procede a la inicialización de los archivos.
    if(confirmacion == 's')
    {
        for(int i = 0; i < 4; i++)
        {
            switch(i) //Rotación de los valores para la creación y escritura de los archivos.
            {
            case 0:
                strcpy(nombre_archivo, ARCHIVO_CLIENTES);
                ptr_escritura_nulo = &cliente_nulo;
                tipo_nulo = sizeof(cli);

                break;

            case 1:
                strcpy(nombre_archivo, ARCHIVO_CATEGORIAS);
                ptr_escritura_nulo = &categoria_nula;
                tipo_nulo = sizeof(cat);

                break;

            case 2:
                strcpy(nombre_archivo, ARCHIVO_MODIFICADORES);
                ptr_escritura_nulo = &modificador_nulo;
                tipo_nulo = sizeof(mod);

                break;

            case 3:
                strcpy(nombre_archivo, ARCHIVO_VENTAS);
                ptr_escritura_nulo = &venta_nula;
                tipo_nulo = sizeof(ven);

                break;
            }

//Creación e inicialización de los archivos.
            if((archivo = fopen(nombre_archivo, "wb"))) // Si el archivo es creado con éxito, se escribe el registro nulo
                                                     // en la posición 0.
            {
                if(fwrite(ptr_escritura_nulo, tipo_nulo, 1, archivo) != 1)
                {
                    fprintf(stderr, "%s%s", "Error al escribir el archivo", nombre_archivo);

                    fclose(archivo);
                    return 0;
                }
                else // Confirmación de la correcta creación del archivo.
                    archivos_creados++;

                fclose(archivo);

                Carga_buffer_archivo(nombre_archivo, tipo_nulo);
            }
            else // Control de error en la creación del archivo (lo que ocurriría si el puntero "archivo" == NULL).
            {
                fprintf(stderr, "%s%s", "Error al crear el archivo", nombre_archivo);

                fclose(archivo);
                return 0;
            }
        }
    }else // Salida de rechazo de la petición.
    {
        BORRAR_PANTALLA

        sprintf(buffer_cartel, "%s", "No se ha creado una nueva base de datos ni cerrado la anterior.");

        printf("\n\n\n\n\n\n\n");
        Imprime_cartel(buffer_cartel, DOBLE);
        printf("\n\n\n\n");

        return 0;
    }

    BORRAR_PANTALLA

    sprintf(buffer_cartel, "%s", "Se ha iniciado una nueva base de datos y se encuentra cargada en el sistema.");

    printf("\n\n\n\n\n\n\n");
    Imprime_cartel(buffer_cartel, DOBLE);
    printf("\n\n\n\n");

    return 1; // Si pasó todas las validaciones, devuelvo 1.
}

//===================================================================================================

short Carga_buffer_archivo(char *const nombre_archivo, size_t tamanio_dato)
{
    FILE *archivo;

    void *buffer; //Almacena temporalmente los registros leídos del archivo.

    unsigned int tamanio_buffer = Tomar_ultimo_registro(nombre_archivo, tamanio_dato) + 1;

    if((archivo = fopen(nombre_archivo, "rb")))
    {
        if((buffer = calloc(tamanio_buffer, tamanio_dato)) == NULL) //Asignación de la memoria necesaria para cargar el archivo
        {
            fprintf(stderr, "%s%c%s", "No hay memoria disponible para ejecutar esta operaci", 162, "n.");

            PAUSA
            return 0;
        }

        if(fread(buffer, tamanio_dato, tamanio_buffer, archivo) != tamanio_buffer) //Lectura completa del archivo
        {
            fprintf(stderr, "%s%s%s%c%s", "Error de lectura del archivo ", nombre_archivo, ". Puede estar da", 164, "ado.");

            fclose(archivo);
            free(buffer); //En caso de error, libero la memoria asignada
            return 0;
        }else
        {
            fclose(archivo);

            if(!strcmp(nombre_archivo, ARCHIVO_CLIENTES)) //Libero la memoria usada previamente por el buffer correspondiente al archivo, y lo apunto al nuevo arreglo
            {
                free(buffer_clientes);
                buffer_clientes = buffer;

                tam_buffer_clientes = tamanio_buffer;
            }
            else if(!strcmp(nombre_archivo, ARCHIVO_CATEGORIAS))
            {
                free(buffer_categorias);
                buffer_categorias = buffer;
                tam_buffer_categorias = tamanio_buffer;
            }
            else if(!strcmp(nombre_archivo, ARCHIVO_MODIFICADORES))
            {
                free(buffer_modificadores);
                buffer_modificadores = buffer;
                tam_buffer_modificadores = tamanio_buffer;
            }
            else if(!strcmp(nombre_archivo, ARCHIVO_VENTAS))
            {
                free(buffer_ventas);
                buffer_ventas = buffer;
                tam_buffer_ventas = tamanio_buffer;
            }

            return 1;
        }
    }else
    {
        fprintf(stderr, "%s%s%s%c%s", "Error de apertura del archivo", nombre_archivo, ". Aseg", 163, "rese de que existe.");

        return 0;
    }
}

//==============================================================

short Guardar_BD()
{
    FILE *archivo_BD;  // Handler del archivo

    cli separador_cli = {-1, "", "", {0}, 0, 0, ""}; //Registros separadores de los registros guardados para cada uno de los 4 archivos de soporte de la base de datos.
    cat separador_cat = {-1, "", {0}};
    mod separador_mod = {-1, "", PORCENTUAL, 0};
    ven separador_ven = {-1, 0, "", 0, 0, 0};

    char buffer_cartel[200];

    char nombre_BD[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], nombre_aux[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO]; //Almacenan y manipulan el nombre dado a la base de datos guardada

    short ingreso;

    sprintf(buffer_cartel, "%s%c%s%d%s%c%s", "Escriba un nombre para la nueva copia (m", 160, "ximo de ", LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO - 7," caracteres), o 0 para volver al men", 163, " anterior.");

    Imprime_cartel(buffer_cartel, DOBLE);

    do //Pide el nombre al usuario y lo valida
    {
        PEDIR_INGRESO

        sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO - 7, "[^\n]");
        scanf(buffer_cartel, nombre_BD);
        fflush(stdin);

        strcpy(nombre_aux, nombre_BD);

        if(!strcmp(nombre_aux, "0"))
            return 0;

        if(!(ingreso = Validar_cadena(nombre_aux, LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO - 8, IMP_SI, ALFANUMERICO)))
            nombre_aux[0] = '\0';

    }while(!ingreso);

    strcpy(nombre_aux, nombre_BD); //Agrega al nombre la ruta y la extensión del archivo a guardar
    strcpy(nombre_BD, "bbdd\\");
    strcat(nombre_BD, nombre_aux);
    strcat(nombre_BD, ".bdc");

    if((archivo_BD = fopen(nombre_BD, "wb")))
    {
        if(fwrite(buffer_clientes, sizeof(cli), tam_buffer_clientes, archivo_BD) != tam_buffer_clientes) //Graba todos los registros de ARCHIVO_CLIENTES
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(&separador_cli, sizeof(cli), 1, archivo_BD) != 1) //Escribe el separador
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(buffer_categorias, sizeof(cat), tam_buffer_categorias, archivo_BD) != tam_buffer_categorias) //Graba todos los registros de ARCHIVO_CATEGORIAS
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(&separador_cat, sizeof(cat), 1, archivo_BD) != 1) //Escribe el separador
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(buffer_modificadores, sizeof(mod), tam_buffer_modificadores, archivo_BD) != tam_buffer_modificadores) //Graba todos los registros de ARCHIVO_MODIFICADORES
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(&separador_mod, sizeof(mod), 1, archivo_BD) != 1) //Escribe el separador
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(buffer_ventas, sizeof(ven), tam_buffer_ventas, archivo_BD) != tam_buffer_ventas) //Graba todos los registros de ARCHIVO_VENTAS
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        if(fwrite(&separador_ven, sizeof(ven), 1, archivo_BD) != 1) //Escribe el separador
        {
            fprintf(stderr, "%s%s", "Error al escribir el archivo ", nombre_BD);

            fclose(archivo_BD);
            return 0;
        }

        fclose(archivo_BD);

        if((archivo_BD = fopen("data\\nom_bbdd.txt", "r+"))) //Guarda el nombre del archivo creado en "nom_bbdd.txt" para la función "Cargar_BD()"
        {
            strcpy(nombre_BD, nombre_aux);
            strcat(nombre_BD, ".");

            int i = 0;

            while(!feof(archivo_BD)) //Examina si el nombre ya existe, en cuyo caso no hace falta escribirlo (la vieja base de datos fue sobreescrita).
            {
                if(fread(&nombre_aux[i], sizeof(char), 1, archivo_BD) != 1 && !feof(archivo_BD))
                {
                    fprintf(stderr, "%s", "Error en la lectura del archivo data\\nom_bbdd.txt.");

                    fclose(archivo_BD);

                    return 0;
                }else
                {
                    if(nombre_aux[i++] == '.') //"." se usa como separador de los nombres, ya que es utilizado para imprimir menús con la función "Imprime_cartel()"
                    {
                        nombre_aux[i] = '\0';

                        if(!strcmp(nombre_aux, nombre_BD))
                        {
                            fclose(archivo_BD);

                            return 1;
                        }
                        else
                            i = 0;
                    }
                }
            }

            if(fwrite(nombre_BD, strlen(nombre_BD), 1, archivo_BD) != 1) //Si no hubo coincidencias, guarda el nuevo nombre
            {
                fprintf(stderr, "%s", "Error en la escritura del archivo data\\nom_bbdd.txt.");

                fclose(archivo_BD);
                return 0;
            }
        }else
        {
            fprintf(stderr, "%s%c%s", "Error en la gesti", 162, "n del archivo data\\nom_bbdd.txt.");

            return 0;
        }
        fclose(archivo_BD);

        return 1; //Si no hubo errores, vuelve con 1.
    }else
    {
        fprintf(stderr, "%s%c%s%s%s", "Error en la creaci", 162, "n del archivo", nombre_BD, ".");

        fclose(archivo_BD);
        return 0;
    }
}

//==========================================================

short Cargar_BD()
{
    FILE *archivo_BD, *archivo_escritura;

    cli buffer_cli = {0}; //Buffers de los tipos de registros.
    cat buffer_cat = {0};
    mod buffer_mod = {0};
    ven buffer_ven = {0};

    char nombre_BD[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO], nombre_aux[LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO]; //Buffers para lectura de los nombres de las bases de datos guardadas.

    char buffer_cartel[200];

    short inic_lect = 0; //Bandera para detectar si "lectura_nombres" ya tiene contenido.

    char *lectura_nombres = NULL; //Acumula dinámicamente los nombres de las bases de datos guardadas.

    if((archivo_BD = fopen("data\\nom_bbdd.txt", "r")))
    {
        int i = 0;

        while(!feof(archivo_BD)) //Lee los nombres guardados en "nom_bbdd.txt" para imprimirlos en pantalla
        {
            if(fread(&nombre_BD[i], sizeof(char), 1, archivo_BD) != 1 && !feof(archivo_BD))
            {
                fprintf(stderr, "%s", "Error en la lectura del archivo data\\nom_bbdd.txt.");

                fclose(archivo_BD);

                return 0;
            }else
            {
                if(nombre_BD[i++] == '.') //Cuando llega a un separador, agranda la memoria de "lectura_nombre" e incorpora el nuevo nombre.
                {
                    nombre_BD[i] = '\0';

                    lectura_nombres = (char*) realloc(lectura_nombres, (lectura_nombres != NULL ? strlen(lectura_nombres) : 0) + strlen(nombre_BD));
                    if(!inic_lect)
                    {
                        strcpy(lectura_nombres, nombre_BD);
                        inic_lect = 1;
                    }else
                        strcat(lectura_nombres, nombre_BD);

                    i = 0;
                }
            }
        }

        if(lectura_nombres == NULL)
        {
            sprintf(buffer_cartel, "%s", "No hay bases de datos guardadas.");

            Imprime_cartel(buffer_cartel, SIMPLE);

            return 0;
        }

        lectura_nombres[strlen(lectura_nombres) - 1] = '\0'; //Tacha el último punto, lo que es requerido por la función "Imprime_cartel()"
    }else
    {
        fprintf(stderr, "%s", "Error en la apertura del archivo data\\nom_bbdd.txt.");

        return 0;
    }

    sprintf(buffer_cartel, "%s", "INGRESE EL NOMBRE DE LA BASE DE DATOS A CARGAR");

    Imprime_cartel(buffer_cartel, ENCABEZADO);

    sprintf(buffer_cartel, "%s", lectura_nombres);

    Imprime_cartel(buffer_cartel, MENU);

    PEDIR_INGRESO //Pide al usuario la base de datos a cargar.

    sprintf(buffer_cartel, "%c%d%s", '%', LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO - 11, "[^.\n]");
    scanf(buffer_cartel, nombre_aux);
    fflush(stdin);

    if((strstr(lectura_nombres, nombre_aux)) == NULL) //Valida el nombre ingresado.
    {
        sprintf(buffer_cartel, "%s", "No hay registro de una base de datos con ese nombre.");

        Imprime_cartel(buffer_cartel, SIMPLE);

        return 0;
    }else
    {
        strcpy(nombre_BD, "bbdd\\"); //Copia la ruta y la extensión de la base solicitada.
        strcat(nombre_BD, nombre_aux);
        strcat(nombre_BD, ".bdc");

        if((archivo_BD = fopen(nombre_BD, "rb")))
        {
            if((archivo_escritura = fopen(ARCHIVO_CLIENTES, "wb")))
            {
                while(buffer_cli.id != -1) //Procede a copiar los primeros registros en ARCHIVO_CLIENTES, hasta que encuentre el separador.
                {
                    if(fread(&buffer_cli, sizeof(cli), 1, archivo_BD) != 1)
                    {
                        fprintf(stderr, "%s%s", "Error en la lectura del archivo ", nombre_BD);

                        fclose(archivo_BD);
                        fclose(archivo_escritura);

                        return 0;
                    }else if(buffer_cli.id != -1)
                        if(fwrite(&buffer_cli, sizeof(cli), 1, archivo_escritura) != 1)
                        {
                            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", ARCHIVO_CLIENTES);

                            fclose(archivo_BD);
                            fclose(archivo_escritura);

                            return 0;
                        }
                }
            }else
            {
                fprintf(stderr, "%s%s", "Error en la apertura del archivo ", ARCHIVO_CLIENTES);

                fclose(archivo_BD);

                return 0;
            }

            fclose(archivo_escritura);

            if((archivo_escritura = fopen(ARCHIVO_CATEGORIAS, "wb"))) //Continúa con ARCHIVO_CATEGORIAS
            {
                while(buffer_cat.id != -1)
                {
                    if(fread(&buffer_cat, sizeof(cat), 1, archivo_BD) != 1)
                    {
                        fprintf(stderr, "%s%s", "Error en la lectura del archivo ", nombre_BD);

                        fclose(archivo_BD);
                        fclose(archivo_escritura);

                        return 0;
                    }else if(buffer_cat.id != -1)
                        if(fwrite(&buffer_cat, sizeof(cat), 1, archivo_escritura) != 1)
                        {
                            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", ARCHIVO_CATEGORIAS);

                            fclose(archivo_BD);
                            fclose(archivo_escritura);

                            return 0;
                        }
                }
            }else
            {
                fprintf(stderr, "%s%s", "Error en la apertura del archivo ", ARCHIVO_CATEGORIAS);

                fclose(archivo_BD);

                return 0;
            }

            fclose(archivo_escritura);

            if((archivo_escritura = fopen(ARCHIVO_MODIFICADORES, "wb"))) //Continúa con ARCHIVO_MODIFICADORES
            {
                while(buffer_mod.id != -1)
                {
                    if(fread(&buffer_mod, sizeof(mod), 1, archivo_BD) != 1)
                    {
                        fprintf(stderr, "%s%s", "Error en la lectura del archivo ", nombre_BD);

                        fclose(archivo_BD);
                        fclose(archivo_escritura);

                        return 0;
                    }else if(buffer_mod.id != -1)
                        if(fwrite(&buffer_mod, sizeof(mod), 1, archivo_escritura) != 1)
                        {
                            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", ARCHIVO_MODIFICADORES);

                            fclose(archivo_BD);
                            fclose(archivo_escritura);

                            return 0;
                        }
                }
            }else
            {
                fprintf(stderr, "%s%s", "Error en la apertura del archivo ", ARCHIVO_MODIFICADORES);

                fclose(archivo_BD);

                return 0;
            }

            fclose(archivo_escritura);

            if((archivo_escritura = fopen(ARCHIVO_VENTAS, "wb"))) //Continúa con ARCHIVO_VENTAS
            {
                while(buffer_ven.id != -1)
                {
                    if(fread(&buffer_ven, sizeof(ven), 1, archivo_BD) != 1)
                    {
                        fprintf(stderr, "%s%s", "Error en la lectura del archivo ", nombre_BD);

                        fclose(archivo_BD);
                        fclose(archivo_escritura);

                        return 0;
                    }else if(buffer_ven.id != -1)
                        if(fwrite(&buffer_ven, sizeof(ven), 1, archivo_escritura) != 1)
                        {
                            fprintf(stderr, "%s%s", "Error en la escritura del archivo ", ARCHIVO_VENTAS);

                            fclose(archivo_BD);
                            fclose(archivo_escritura);

                            return 0;
                        }
                }
            }else
            {
                fprintf(stderr, "%s%s", "Error en la apertura del archivo ", ARCHIVO_VENTAS);

                fclose(archivo_BD);

                return 0;
            }
        }else
        {
            fprintf(stderr, "%s%s", "Error en la apertura del archivo ", nombre_BD);

            return 0;
        }

        fclose(archivo_escritura);
    }

    fclose(archivo_BD);

    //Si no hubo errores, carga los registros de los nuevos archivos de soporte en sus respectivos buffers, y devuelve 1.
    if(!Carga_buffer_archivo(ARCHIVO_CLIENTES, sizeof(cli)) || !Carga_buffer_archivo(ARCHIVO_CATEGORIAS, sizeof(cat)) || !Carga_buffer_archivo(ARCHIVO_MODIFICADORES, sizeof(mod)) || !Carga_buffer_archivo(ARCHIVO_VENTAS, sizeof(ven)))
        return 0;
    else
        return 1;
}
