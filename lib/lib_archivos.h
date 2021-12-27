#ifndef LIB_ARCHIVOS_H_INCLUDED
#define LIB_ARCHIVOS_H_INCLUDED

//Funciones de manejo de archivos

//PROTOTIPOS

short Crear_archivos();     //Genera los 4 archivos que soportan la base de datos cargada en sistema y los inicializa
                            //con un primer registro nulo (posición 0 de la secuencia).
                            //No recibe parámetros.
                            //Devuelve 1 si la inicialización de los 4 archivos se realizó exitosamente; devuelve 0 en
                            //todos los demás casos.

short Carga_buffer_archivo(char *const nombre_archivo, size_t tamanio_dato);
                            //Carga el buffer correspondiente al archivo indicado, copiando todos sus registros en el
                            //mismo orden.
                            //Parámetros:
                                //"nombre_archivo", ruta y nombre del archivo a cargar. Normalmente se invoca
                                //la constante simbólica declarada en "definiciones.h".
                                //"tamanio_dato", el tamaño de la struct correspondiente a los registros del
                                //archivo (por ejemplo, "sizeof(cli)")
                            // Devuelve 1 si la carga fue exitosa, o 0 en caso contrario.

short Guardar_BD();         //Genera un archivo con el nombre indicado por el usuario, en el que escribe todos los registros
                            //de los 4 archivos de soporte de la base de datos cargada en sistema.
                            //No recibe parámetros.
                            //Devuelve 1 si la creación del archivo fue exitosa, o 0 en caso contrario.

short Cargar_BD();          //Sobreescribe los archivos de soporte de la base de datos con los registros de la seleccionada
                            //entre las guardadas previamente con "Guardar_BD()", y los carga en sistema.
                            //No recibe parámetros.
                            //Devuelve 1 en caso de éxito o 0 en caso contrario.

#endif // LIB_ARCHIVOS_H_INCLUDED
