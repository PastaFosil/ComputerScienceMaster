# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 5 de la materia de Metodos Numericos, relativa a la resolucion de la ecuacion de calor mediante su analisis por diferencias finitas, asi como a la implementacion de metodos iterativos de resolucion de sistemas de ecuaciones lineales. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contienen los archivos .c que resuelven los ejercicios de la tarea.
- ec_calor.c toma los parametros Q, K, phi_0, phi_n, L y n para resolver la ecuacion de calor, habiendo hecho uso del metodo de diferencias finitas para su analisis.
- metodo_jacobi.c resuelve un sistema lineal de ecuaciones mediante el metodo iterativo de Jacobi, tomando como parametros unicamente los nombres de los archivos txt que contienen la matriz y el vector de resultados del sistema.
- metodo_gaus_seidel.c resuelve un sistema lineal de ecuaciones mediante el metodo iterativo de Gauss-Seidel, tomando como parametros unicamente los nombres de los archivos txt que contienen la matriz y el vector de resultados del sistema.

## Ejecucion

Para correr ec_calor.c se necesita ejecutar el comando "./calor Q K phi_0 phi_n L n", con todos los parametros en formato de double, excepto n, que es int.

Por su parte, para correr los codigos de los metodos iterativos, se necesita ejecutar el comando "./metodo_deseado archivo_matriz.txt archivo_vector.txt", los cuales deben tener la estructura


dim_1 dim_2
a11 a12 ... a1[dim2]
a21 a22 ... a2[dim2]
... ...     ...
a[dim1]1 a[dim1]2 ... a[dim1][dim2]


para el caso de las matrices, y


dim
b1
b2
...
b[dim]


para el caso de los vectores.

## Resultados

En cada carpeta se encuentran los resultados obtenidos en cada codigo.

En Calor, cada archivo "heat_solution_n_[n].txt" posee el vector solucion del sistema con los parametros Q=1000, K=1, phi_0=0, phi_n=100, L=10, con [n] tomando alguno de los valores 4, 10, 30, 50, 70 y 100. 

Por su parte, en GS y en Jacobi se encuentran los vectores solucion al ingresar los sistemas contenidos en BigMatrix.txt, SmallMatrix.txt, BigVector.txt, y SmallVector.txt, a los codigos metodo_gauss_seidel.c y metodo_jacobi.c, respectivamente. 
