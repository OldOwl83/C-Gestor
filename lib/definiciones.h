#ifndef DEFINICIONES_H_INCLUDED
#define DEFINICIONES_H_INCLUDED

//Llamadas al sistema
#define BORRAR_PANTALLA system("cls");
#define PAUSA system("pause");

//MACROS

#define PEDIR_INGRESO printf("\n\n");for(int i = 0; i < ESPACIOS_MARGEN + 3; i++)putchar(' ');printf("Ingreso: ");

//Longitudes para arreglos num�ricos. Establece el m�ximo de categor�as
//y modificadores que pueden declararse para una base de datos particular
#define NRO_CATEGORIAS 10
#define NRO_MODIFICADORES 60

//Longitudes para almacenamiento de cadenas (por regla general,
//la extensi�n m�xima permitida a las cadenas ser� de dos caracteres
//menos que la longitud indicada en estas constantes simb�licas).
//Si se modifican estas longitudes, la base de datos debe ser reinicializada.
#define LONGITUD_APELLIDO_CLIENTE 22
#define LONGITUD_NOMBRE_CLIENTE 16
#define LONGITUD_NOMBRE_CATEGORIA 18
#define LONGITUD_NOMBRE_MODIFICADOR 18
#define LONGITUD_DATOS_ADICIONALES 100
#define LONGITUD_DESCRIPCION_VENTAS 50
#define LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO 25

//Nombres de archivos
#define ARCHIVO_CLIENTES "data\\clientes.dat" //La longitud del nombre de los archivos, incluida la extensi�n, no puede superar
                                        //la declarada en LONGITUD_MAXIMA_NOMBRE_ARCHIVOS_MAS_NULO - 1
#define ARCHIVO_CATEGORIAS "data\\categorias.dat"
#define ARCHIVO_MODIFICADORES "data\\modificadores.dat"
#define ARCHIVO_VENTAS "data\\ventas.dat"

//MEDIDAS
#define ANCHO_CARTEL 80 //M�nimo 40
#define ESPACIOS_MARGEN 10 //Establece el margen izquierdo para todas las salidas por pantalla

//Definiciones de tipos de datos

//Enumeraciones
typedef enum tipos_modificadores{PORCENTUAL = 1, FIJO}t_mod; //Para el struct "modificador_categoria"

typedef enum pide_impresion{IMP_NO, IMP_SI}imp; //Para la funci�n "Validar_cadena()"

typedef enum criterio_validacion{ALFABETICO, ALFANUMERICO}cri_v; //Para la funci�n "Validar_cadena()"

typedef enum precision_busqueda{COIN_EXACTA, COIN_PARCIAL}pre_b; //Para las funciones "Buscar_cliente_X_apellido()", "Buscar_categoria_X_nombre()" y "Buscar_modificador_X_nombre()"

typedef enum tipo_cartel{SIMPLE, DOBLE, ENCABEZADO, MENU}tip_c; //Para la funci�n "Imprime_cartel()"

//Estructuras
typedef struct cliente
{
    unsigned int id;
    char apellido[LONGITUD_APELLIDO_CLIENTE];
    char nombre[LONGITUD_NOMBRE_CLIENTE];
    short categorias[NRO_CATEGORIAS]; //Cada posici�n del arreglo corresponde al ID de una eventual categor�a, y su valor booleano (1/0) indica si el cliente est� o no asociado a la categor�a correspondiente.
    float facturacion_total;
    float pagos_totales;
    char datos_adicionales[LONGITUD_DATOS_ADICIONALES];
}cli;

typedef struct categoria
{
    unsigned int id;
    char nombre[LONGITUD_NOMBRE_CATEGORIA];
    short modificadores[NRO_MODIFICADORES]; //Cada posici�n del arreglo corresponde al ID de un eventual modificador, y su valor booleano (1/0) indica si la categor�a est� o no asociada al modificador correspondiente.
}cat;

typedef struct modificador_categoria
{
    unsigned int id;
    char nombre[LONGITUD_NOMBRE_MODIFICADOR];
    t_mod tipo;
    float valor;
}mod;

typedef struct venta
{
    unsigned int id;
    unsigned int cliente;
    char descripcion[LONGITUD_DESCRIPCION_VENTAS];
    float precio_base;
    float precio_modificado;
    float abonado;
}ven;

//Variables puntero globales utilizadas como buffers de lectura
//y escritura de los archivos de la base de datos. El objetivo
//es que siempre repliquen el contenido total de los archivos,
//de modo de tener un acceso m�s r�pido a la informaci�n.
extern cli *buffer_clientes;
extern cat *buffer_categorias;
extern mod *buffer_modificadores;
extern ven *buffer_ventas;

//Variables globales para almacenar el tama�o actual de los buffers
//de los archivos de la base de datos, ya que los mismos var�an din�micamente.
extern size_t tam_buffer_clientes;
extern size_t tam_buffer_categorias;
extern size_t tam_buffer_modificadores;
extern size_t tam_buffer_ventas;

#endif // DEFINICIONES_H_INCLUDED
