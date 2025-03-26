# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 8 de la materia de Metodos Numericos, relativa a la implementacion del metodo iterativo de gradiente conjugado precondicionado para resolver sistemas lineales de ecuaciones. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contiene el codigo gradienteConjugado.c, el cual devuelve un archivo txt con el vector solucion y otro con el vector residual del sistema lineal ingresado.


## Ejecucion

Para correr el codigo, es necesario ejecutar el comando "./grad matriz.txt vector.txt".

El vector solucion se almacena en el archivo "sol_matriz.txt", y el residual en "r_sol_matriz.txt".

Los archivos txt que contienen las matrices deben tener la estructura

dim_1 dim_2
a11 a12 ... a1[dim2]
a21 a22 ... a2[dim2]
... ...     ...
a[dim1]1 a[dim1]2 ... a[dim1][dim2]

y el que contiene el vector, la estructura 

dim
v1
v2
...
v[dim]
