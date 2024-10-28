# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 9 de la materia de Metodos Numericos, relativa a la implementacion de diferentes metodos de interpolacion. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contienen los codigos: 
- taylor.c que calcula la aproximacion de la funcion exponencial por un polinomio de Taylor de grado definido
- lagrange.c que calcula la aproximacion de la funcion exponencial en una serie de coordenadas a partir de la interpolacion de otro conjunto de puntos
- 


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
