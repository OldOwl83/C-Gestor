#ifndef LIB_CATEGORIAS_H_INCLUDED
#define LIB_CATEGORIAS_H_INCLUDED

//Funciones de gestión de los registros de categorías

//PROTOTIPOS

unsigned int Agregar_nueva_categoria();
                            //Gestiona el ingreso de una nueva categoría a la base de datos.
                            //No recibe parámetros.
                            //Devuelve el ID atribuido a la nueva categoría, o 0 si no se concretó el ingreso.

short Modificar_categoria();
                            //Gestiona la modificación de los datos de categoría, o la eliminación de sus registros.
                            //No recibe parámetros.
                            //Devuelve 1 si la modificación/eliminación fue exitosa, o 0 en caso contrario.

void Desplegar_categorias(unsigned int ids[NRO_CATEGORIAS], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con las categorías indicadas.
                            //Parámetros:
                                //"ids": arreglo con la lista de IDs de las categorías a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

size_t Buscar_categoria_X_nombre(unsigned int buffer_ids[NRO_CATEGORIAS], char nombre[LONGITUD_NOMBRE_CATEGORIA], pre_b precision);
                            //Busca la o las categorías cuyo nombre coincide con el nombre indicado por parámetro.
                            //Si se solicita una coincidencia exacta, se obtendrá el primer nombre idéntico al nombre
                            //indicado. Si se solicita una coincidencia parcial, de obtendrá la lista de todas las
                            //categorías cuyo nombre comience con el nombre indicado.
                            //Parámetros:
                                //"buffer_ids": arreglo en el que se almacenarán los IDs de las categorías cuyo nombre
                                //coincida con los criterios de búsqueda.
                                //"nombre": cadena con la que se compararán los nombres de las categorías registradas.
                                //"precision": indica si la coincidencia debe ser exacta o parcial, mediante el enum
                                //'precision_busqueda'.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no las hubo.

short Vincular_categoria_modificador(unsigned int id_categoria);
                            //Gestiona la vinculación o desvinculación de las categorías con los modificadores disponibles.
                            //Parámetro:
                                //"id_categoría": ID de la categoría a vincular o desvincular con uno o más modificadores.
                            //Devuelve 1 si las operaciones solicitadas se realizaron con éxito, o 0 en caso contrario.

unsigned int Pedir_categoria();
                            //Gestiona la búsqueda de una categoría particular por NOMBRE o por ID en un mismo ingreso.
                            //Detecta automáticamente si se ingresó una cadena alfabética o un entero. En el primer caso,
                            //si se hallaron múltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe parámetros.
                            //Devuelve el ID de la categoría hallada, o 0 si se renunció a la búsqueda.

#endif // LIB_CATEGORIAS_H_INCLUDED
