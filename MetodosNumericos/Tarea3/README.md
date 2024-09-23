# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 3 de la materia de Metodos Numericos, relativa a la resolucion de sistemas de ecuaciones lineales. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de los archivos tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

# Descripcion de los archivos

Cada codigo sistema*.c esta nombrado relativo al tipo de sistema lineal que es capaz de resolver
- sistemaDiagonal.c esta dedicado a aquellos sistemas que pueden describirse mediante una matriz diagonal
- sistemaTriangularSuperior.c esta dedicado a sistemas que se describen por medio de una matriz triangular superior
- sistemaTriangularInferior.c, a aquellos que se describen con matrices triangulares inferiores
- sistemaGaus.c, a sistemas generales en los que casi ninguno de sus elementos es nulo, o que no coinciden con ninguna de las descripciones anteriores

Para correrlos, se necesita ejecutar el comando "./sistema_requerido archivo_matriz.txt archivo_vector.txt", los cuales deben tener la estructura

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

Por su parte, los archivos vectorX_*.txt contienen las soluciones a los sistemas lineales de cada uno de estos casos, suplementados en la descripcion de la tarea.

Todas las matrices y vectores son leidas y creadas haciendo uso de las funciones definidas en tools.c y tools.h, las cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.
