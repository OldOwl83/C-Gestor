#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lib_auxiliares.h"

short Validar_cadena(char cadena[], short longitud_max, imp imprime_carteles, cri_v criterio)
{
    int i;

    short tiene_letra = 0;

    char texto_cartel[120];

    if(!strlen(cadena)) //Valida que la cadena tenga longitud distinta de cero.
    {
        if(imprime_carteles)
        {
            sprintf(texto_cartel, "%s%c%s", "No se ha efectuado ning", 163, "n ingreso.");

            Imprime_cartel(texto_cartel, SIMPLE);
        }
        return 0;
    }

    if(isspace(cadena[0])) //Evita espacios al inicio de un campo.
    {
        if(imprime_carteles)
        {
            sprintf(texto_cartel, "%s", "No se permiten espacios al comienzo de este campo.");

            Imprime_cartel(texto_cartel, SIMPLE);
        }
        return 0;
    }

    if(criterio == ALFABETICO) //Recorre la cadena para verificar que sólo se componga de letras y espacios.
    {
        for(i = 0; cadena[i] != '\0'; i++)
        {
            if(!isalpha(cadena[i]) && cadena[i] != ' ')
            {
                if(imprime_carteles)
                {
                    sprintf(texto_cartel, "%s%c%s%c%s", "Ingreso inv", 160, "lido. Este campo s", 162, "lo puede contener letras y espacios.");

                    Imprime_cartel(texto_cartel, SIMPLE);
                }
                return 0;
            }
            else
            {
                cadena[i] = toupper(cadena[i]); // Aprovecho el ciclo para pasar a mayúsculas los caracteres de
                                                // cadena, en la medida en que aun no fue invalidada.
            }
        }
    }else if(criterio == ALFANUMERICO) //Recorre la cadena para verificar que sólo se componga de letras, números y espacios.
    {
        for(i = 0; cadena[i] != '\0'; i++)
        {
            if(!isalnum(cadena[i]) && cadena[i] != ' ')
            {
                if(imprime_carteles)
                {
                    sprintf(texto_cartel, "%s%c%s%c%s%c%s", "Ingreso inv", 160, "lido. Este campo s", 162, "lo puede contener letras, n", 163, "meros y espacios.");

                    Imprime_cartel(texto_cartel, SIMPLE);
                }
                return 0;
            }
            else
            {
                cadena[i] = toupper(cadena[i]); // Aprovecho el ciclo para pasar a mayúsculas los caracteres de
                                                // cadena, en la medida en que aun no fue invalidada.
                if(isalpha(cadena[i]))
                    tiene_letra = 1;
            }
        }

        if(!tiene_letra) //Impide campos meramente numéricos.
        {
            sprintf(texto_cartel, "%s%c%s", "Ingreso inv", 160, "lido. Este campo debe tener al menos una letra de modo de no confundirse con un ID.");

            Imprime_cartel(texto_cartel, SIMPLE);

            return 0;
        }
    }
    if(i == longitud_max + 1) // Si el caracter nulo fue encontrado en la última posición del vector cadena,
                         // el usuario superó la cantidad máxima permitida, ya que ese caracter nulo
                         // lo introdujo la función scanf con el especificador %(longitud_max)s,
    {                    // ya sea que el usuario haya superado por uno o por más la longitud máxima permitida.
                         // Por tanto, el ingreso es inválido.

        if(imprime_carteles)
        {
            sprintf(texto_cartel, "%s%c%s%d%s", "Ingreso demasiado extenso. El campo actual tiene una longitud m", 160, "xima de ", longitud_max, " caracteres.");

            Imprime_cartel(texto_cartel, SIMPLE);
        }
        return 0;
    }else
        return 1; //Si la cadena pasa la validación, devuelvo 1.
}

//==============================================================================================

int Validar_cadena_como_entero(const char cadena[])
{
    int longitud_cadena = strlen(cadena); // Almaceno la longitud de la cadena pasada por parámetro, para no tener que llamar a la función
                                          // "strlen" en cada entrada al ciclo siguiente (obteniendo un mayor rendimiento).


    for(int i = 0; i < longitud_cadena; i++)
        if(!isdigit(cadena[i])) // Si alguno de los caracteres en el vector no es dígito, invalido la entrada y devuelvo 0.
        {
            //printf("\nEl ingreso no representa un n%cmero entero.\n", 163);
            return 0;
        }

    return atoi(cadena); // En cambio, si no encuentro ningún caracter extraño, convierto la cadena en un "int".
}

//==============================================================================================

unsigned int Tomar_ultimo_registro(const char nombre_archivo[], size_t tamanio_dato)
{
    FILE *archivo;

    unsigned int buffer_id; //Buffer de lectura de los IDs de los distintos registros del archivo a considerar.

    fpos_t posic; //Controla que el cursor no esté ubicado en el primer registro nulo.

    if((archivo = fopen(nombre_archivo, "rb")))
    {
        if(fseek(archivo, tamanio_dato * -1, SEEK_END)) //Se recorre el archivo de abajo hacia arriba, buscando el último registro válido (e decir, con un ID distinto de cero).
        {
            fprintf(stderr, "%s%s", "Error en el posicionamiento del cursor interno del archivo ", nombre_archivo);

            fclose(archivo);
            return 0;
        }

        do
        {
            fgetpos(archivo, &posic);

            if(posic < tamanio_dato - 1) //Si el cursor llegó al primer registro nulo, entonces no hay un último registro activo.
                return 0;

            if(fread(&buffer_id, sizeof(unsigned int), 1, archivo) != 1)
            {
                fprintf(stderr, "%s%s", "Error en la lectura del archivo ", nombre_archivo);

                fclose(archivo);
                return 0;
            }else if(buffer_id == 0) //Si el registro leído no tiene ID, debe seguir retrocediendo en el archivo.
            {
                if(fseek(archivo, (sizeof(unsigned int) + tamanio_dato) * -1, SEEK_CUR))
                {
                    fprintf(stderr, "%s%s%s%c%s", "Error en el posicionamiento del cursor interno del archivo ", nombre_archivo, ", o se complet", 162, " su lectura sin encontrar resultados.");

                    fclose(archivo);
                    return 0;
                }
            }
        }while(buffer_id == 0); //Hasta que encuentre un registro activo...

        fclose(archivo);
        return buffer_id; //Si salió del while, encontró un último registro activo y devuelve su ID.
    }
    else // Control de error en la creación del archivo (lo que ocurriría si el puntero "archivo" == NULL).
    {
        fprintf(stderr, "%s%s%s%c%s", "Error de apertura del archivo ", nombre_archivo, ". Aseg", 163, "rese de que existe.");

        fclose(archivo);
        return 0;
    }
}

//===========================================================================================================

void Imprime_cartel(char *ptr_cadena, tip_c tipo_cartel)
{
    char *fin_renglon;

    unsigned short longitud_cadena = 0, contador = 1, bandera = 1;

//BORDE SUPERIOR
    putchar('\n');

    if(tipo_cartel == SIMPLE)
    {
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar(218);

        for(int i = 0; i < ANCHO_CARTEL; i++)
            putchar(196);

        putchar(191);
    }else if(tipo_cartel == MENU)
    {
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar(218);

        for(int i = 0; i < ANCHO_CARTEL - 8; i++)
            putchar(196);

        putchar(194);

        for(int i = 0; i < 7; i++)
            putchar(196);

        putchar(191);

    }else
    {
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar(201);

        for(int i = 0; i < ANCHO_CARTEL; i++)
            putchar(205);

        putchar(187);
    }

//TEXTO
    if(tipo_cartel == SIMPLE || tipo_cartel == DOBLE)
    {
        do
        {
            putchar('\n');

            for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

            printf("%c      ", tipo_cartel == SIMPLE ? 179 : 186);

            fin_renglon = ptr_cadena + ANCHO_CARTEL - 8;

            while(*fin_renglon != ' ' && *fin_renglon != '\0')
                fin_renglon--;

            contador = 1;

            for(int i = 0; i < ANCHO_CARTEL - 8; i++)
            {
                if(ptr_cadena < fin_renglon && contador == 1)
                    if(*ptr_cadena != '\0')
                        putchar(*ptr_cadena++);
                    else
                    {
                        contador = 0;
                        putchar(' ');
                    }
                else
                    putchar(' ');
            }

            printf("  %c", tipo_cartel == SIMPLE ? 179 : 186);

        }while(*ptr_cadena++ != '\0');
    }else if(tipo_cartel == ENCABEZADO)
    {
        putchar('\n');

        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        printf("%c", 186);

        longitud_cadena = strlen(ptr_cadena);

        for(int i = 0; i < ANCHO_CARTEL; i++)
            if(i < (ANCHO_CARTEL - longitud_cadena) / 2)
                putchar(' ');
            else if(i < longitud_cadena + (ANCHO_CARTEL - longitud_cadena) / 2)
                putchar(*ptr_cadena++);
            else
                putchar(' ');

        printf("%c", 186);
    }else //La cadena que contiene el texto del menú debe separar las opciones con '.'
    {
        do
        {
            putchar('\n');

            for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

            printf("%c      ", 179);

            fin_renglon = ptr_cadena + ANCHO_CARTEL - 16;

            while(*fin_renglon != ' ' && *fin_renglon != '\0')
                fin_renglon--;

            longitud_cadena = 1;

            for(int i = 0; i < ANCHO_CARTEL - 16; i++)
            {
                if(ptr_cadena < fin_renglon && longitud_cadena == 1)
                    if(*ptr_cadena != '.' && *ptr_cadena != '\0')
                        putchar(*ptr_cadena++);
                    else
                    {
                        longitud_cadena = 0;
                        putchar(' ');
                        //ptr_cadena++;
                    }
                else
                    putchar(' ');
            }

            if(bandera)
            {
                printf("  %c  %2d   %c", 179, contador++, 179);
                bandera = 0;
            }
            else
                printf("  %c       %c", 179, 179);

            if(*(ptr_cadena) == '.')
            {
                putchar('\n');

                for(int i = 0; i < ESPACIOS_MARGEN; i++)
                    putchar(' ');

                putchar(195);

                for(int i = 0; i < ANCHO_CARTEL - 8; i++)
                    putchar(196);

                putchar(197);

                for(int i = 0; i < 7; i++)
                    putchar(196);

                putchar(180);

                bandera = 1;
            }

        }while(*ptr_cadena++ != '\0');
    }

    //BORDE INFERIOR
    if(tipo_cartel == SIMPLE)
    {
        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar('\n');

        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar(192);

        for(int i = 0; i < ANCHO_CARTEL; i++)
            putchar(196);

        putchar(217);
    }else if(tipo_cartel == MENU)
    {
        for(int i = 0; i < 2; i++)
        {
            putchar('\n');

            for(int i = 0; i < ESPACIOS_MARGEN; i++)
                putchar(' ');

            putchar(195);

            for(int i = 0; i < ANCHO_CARTEL - 8; i++)
                putchar(196);

            putchar(197);

            for(int i = 0; i < 7; i++)
                putchar(196);

            putchar(180);
        }

        putchar('\n');

        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        printf("%c      Salir", 179);

        for(int i = 0; i < ANCHO_CARTEL - 22; i++)
            putchar(' ');

        printf("   %c   0   %c", 179, 179);

        putchar('\n');

        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar(192);

        for(int i = 0; i < ANCHO_CARTEL - 8; i++)
            putchar(196);

        putchar(193);

        for(int i = 0; i < 7; i++)
            putchar(196);

        putchar(217);
    }else
    {
        putchar('\n');

        for(int i = 0; i < ESPACIOS_MARGEN; i++)
            putchar(' ');

        putchar(200);

        for(int i = 0; i < ANCHO_CARTEL; i++)
            putchar(205);

        putchar(188);
    }
    putchar('\n');
}
