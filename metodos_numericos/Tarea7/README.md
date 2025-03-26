# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 7 de la materia de Metodos Numericos, relativa a la implementacion de la resolucion de un sistema lineal a traves de la factorizacion QR. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contienen los archivos .c que resuelven los ejercicios de la tarea.
- qr.c resuelve un sistema lineal usando factorizacion QR.
- subespacio.c detalla los pasos generales de la implementacion del metodo de subespacio.

## Ejecucion

Para correr qr.c, es necesario ejecutar "./qr matriz.txt vector.txt", y devuelve un archivo con el vector solucion llamado "sol_matriz.txt".

Si bien subespacio.c no devuelve resultados, seria necesario solo ejecutar "./subespacio matriz.txt".

Los archivos txt que contienen las matrices deben tener la estructura

dim_1 dim_2
a11 a12 ... a1[dim2]
a21 a22 ... a2[dim2]
... ...     ...
a[dim1]1 a[dim1]2 ... a[dim1][dim2]

Similarmente, los de los vectores son

dim
v1
v2
...
v3
