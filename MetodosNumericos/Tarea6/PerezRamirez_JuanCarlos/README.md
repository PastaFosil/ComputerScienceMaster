# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 6 de la materia de Metodos Numericos, relativa a la implementacion de metodos iterativos para encontrar los valores y vectores propios de una matrix dada. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contienen los archivos .c que resuelven los ejercicios de la tarea.
- metodo_potencia.c calcula los k mayores valores propios de la matriz, asi como sus respectivos vectores propios.
- metodo_potencia_inversa.c calcula los k menores vectores propios de la matriz, asi como sus respectivos vectores propios.
- jacobi_eigen.c diagonaliza la matriz ingresada, de manera que sus elementos no nulos son los valores propios, mientras que la matriz que logra la diagonalizacion a traves de transformaciones de similaridad posee los vectores propios en sus columnas.
-funcion_matriz.c contiene funciones para calcular el cuadrado y la raiz cuadrada de la matriz ingresada.

## Ejecucion

Para correr los primeros dos codigos, es necesario ejecutar el comando "./metodo matriz.txt k" para calcular los k valores y vectores propios deseados.

Por su parte, para correr los otros dos codigos es necesario ejecutar el comando "./codigo matriz.txt".

Todos los resultados son impresos en la terminal.

Los archivos txt que contienen las matrices deben tener la estructura

dim_1 dim_2
a11 a12 ... a1[dim2]
a21 a22 ... a2[dim2]
... ...     ...
a[dim1]1 a[dim1]2 ... a[dim1][dim2]
