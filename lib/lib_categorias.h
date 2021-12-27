#ifndef LIB_CATEGORIAS_H_INCLUDED
#define LIB_CATEGORIAS_H_INCLUDED

//Funciones de gesti�n de los registros de categor�as

//PROTOTIPOS

unsigned int Agregar_nueva_categoria();
                            //Gestiona el ingreso de una nueva categor�a a la base de datos.
                            //No recibe par�metros.
                            //Devuelve el ID atribuido a la nueva categor�a, o 0 si no se concret� el ingreso.

short Modificar_categoria();
                            //Gestiona la modificaci�n de los datos de categor�a, o la eliminaci�n de sus registros.
                            //No recibe par�metros.
                            //Devuelve 1 si la modificaci�n/eliminaci�n fue exitosa, o 0 en caso contrario.

void Desplegar_categorias(unsigned int ids[NRO_CATEGORIAS], size_t tamanio_ids);
                            //Despliega por pantalla una tabla con las categor�as indicadas.
                            //Par�metros:
                                //"ids": arreglo con la lista de IDs de las categor�as a mostrar.
                                //"tamanio_ids": longitud del arreglo "ids".

size_t Buscar_categoria_X_nombre(unsigned int buffer_ids[NRO_CATEGORIAS], char nombre[LONGITUD_NOMBRE_CATEGORIA], pre_b precision);
                            //Busca la o las categor�as cuyo nombre coincide con el nombre indicado por par�metro.
                            //Si se solicita una coincidencia exacta, se obtendr� el primer nombre id�ntico al nombre
                            //indicado. Si se solicita una coincidencia parcial, de obtendr� la lista de todas las
                            //categor�as cuyo nombre comience con el nombre indicado.
                            //Par�metros:
                                //"buffer_ids": arreglo en el que se almacenar�n los IDs de las categor�as cuyo nombre
                                //coincida con los criterios de b�squeda.
                                //"nombre": cadena con la que se comparar�n los nombres de las categor�as registradas.
                                //"precision": indica si la coincidencia debe ser exacta o parcial, mediante el enum
                                //'precision_busqueda'.
                            //Devuelve la cantidad de coincidencias almacenadas en "buffer_ids", o 0 si no las hubo.

short Vincular_categoria_modificador(unsigned int id_categoria);
                            //Gestiona la vinculaci�n o desvinculaci�n de las categor�as con los modificadores disponibles.
                            //Par�metro:
                                //"id_categor�a": ID de la categor�a a vincular o desvincular con uno o m�s modificadores.
                            //Devuelve 1 si las operaciones solicitadas se realizaron con �xito, o 0 en caso contrario.

unsigned int Pedir_categoria();
                            //Gestiona la b�squeda de una categor�a particular por NOMBRE o por ID en un mismo ingreso.
                            //Detecta autom�ticamente si se ingres� una cadena alfab�tica o un entero. En el primer caso,
                            //si se hallaron m�ltiples coincidencias, se las muestra para que el usuario elija el ID
                            //correcto.
                            //No recibe par�metros.
                            //Devuelve el ID de la categor�a hallada, o 0 si se renunci� a la b�squeda.

#endif // LIB_CATEGORIAS_H_INCLUDED
