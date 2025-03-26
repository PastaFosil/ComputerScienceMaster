# Examen parcial 1. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en el primer examen parcial de la materia de Metodos Numericos. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contienen los archivos .c que resuelven los problemas del examen.
- problema1_PerezRamirez_JuanCarlos.c contiene la resolucion del problema 1, sobre la graficacion de los resultados obtenidos de la evaluacion de una matriz
- problema2_PerezRamirez_JuanCarlos.c contiene la resolucion del problema 2, sobre la resolucion de un sistema de ecuaciones descrito por una matriz banda, asi como la determinacion de sus 10 mayores y 10 menores valores propios, junto con sus vectores propios asociados.

## Ejecucion

Para correr los dos codigos es necesario ejecutar el comando "./problema".

Los resultados son impresos en la terminal, y en adicion a ello se generan archivos txt que contienen el vector solucion de la matriz por los dos diferentes metodos ("resolucion_gs.txt" para el metodo de Gauss-Seidel, y "resolucion_LLT.txt", para el metodo por factorizacion LLT); tambien los archivos con los valores y vectores propios ("mayores_vectores_propios.txt" para los mayores, y "menores_vectores_propios.txt", para los menores), escritos en forma de matriz de manera que cada renglon es un vector, y el ultimo elemento del renglon es su valor propio asociado.
