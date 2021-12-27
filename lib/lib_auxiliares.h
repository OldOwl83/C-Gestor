#ifndef LIB_AUXILIARES_H_INCLUDED
#define LIB_AUXILIARES_H_INCLUDED

#include "definiciones.h"

//Funciones auxiliares (lib_auxiliares.h)
short Validar_cadena(char cadena[], short long_max, imp imprime_carteles, cri_v criterio);
                            //Valida las cadenas de caracteres para los campos de los distintos registros y las pasa a Uppercase.
                            //Par�metros:
                                //"cadena": la cadena a ser validada.
                                //"long_max": la longitud m�xima permitida para el campo correspondiente a la cadena.
                                //"imprime_cartel": indica si deben o no imprimirse por pantalla los carteles de la validaci�n,
                                //mediante el enum "pide_impresion".
                                //"criterio": indica si la cadena debe ser alfab�tica o alfanum�rica, mediante el enum "criterio_validaci�n".
                            //Devuelve 1 si la "cadena" es v�lida, o 0 en caso contrario.

int Validar_cadena_como_entero(const char cadena[]);
                            //Valida si una cadena representa un n�mero entero y lo convierte a ese tipo de dato.
                            //Par�metro:
                                //"cadena": la cadena a ser validada y convertida.
                            //Devuelve un "int" equivalente al entero representado por la cadena, si �sta es validada; o 0 en caso contrario.

unsigned int Tomar_ultimo_registro(const char nombre_archivo[], size_t tamanio_dato);
                            //Busca el �ltimo registro activo en el archivo indicado por par�metros.
                            //Par�metros:
                                //"nombre_archivo": nombre del archivo a registrar.
                                //"tamanio_dato": tama�o del struct al que pertenecen los registros del archivo "nombre_archivo".
                            //Devuelve el ID del �ltimo registro activo del archivo, o 0 si no hay ninguno, o hubo un error.

void Imprime_cartel(char *ptr_cadena, tip_c tipo_cartel);
                            //Recuadra y alinea la cadena indicada por par�metros seg�n los distintos tipos de carteles disponibles
                            //(SIMPLE: bordes simples con alineaci�n a la izquierda; DOBLE: bordes dobles con alineaci�n a la izquierda;
                            //ENCABEZADO: bordes dobles con alineaci�n centrada; y MENU: recuadro donde se enlistan y numeran las
                            //cadenas separadas por ".")
                            //Par�metros:
                                //"ptr_cadena": cadena a ser impresa en un cartel.
                                //"tipo_cartel": indica el tipo de cartel que se desea.

#endif // LIB_AUXILIARES_H_INCLUDED
