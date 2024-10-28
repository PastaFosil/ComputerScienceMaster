# Tarea 3. Metodos Numericos

Este zip contiene los codigos que resuelven los problemas planteados en la Tarea 9 de la materia de Metodos Numericos, relativa a la implementacion de diferentes metodos de interpolacion. Todos los codigos son autoria de Juan Carlos Perez Ramirez, a excepcion de las funciones de lectura, escritura y generacion de matrices en tools.c y tools.h, cortesia de Jairo Saul Diaz Soto.

Todas las funciones empleadas en la lectura, escritura y operacion sobre las matrices y los vectores empleados se encuentran contenidas en tools.c y tools.h, los cuales requieren estar en el mismo directorio en el que se ubiquen el resto de codigos.

# Descripcion de los archivos

## Codigos

Se contienen los codigos: 
- taylor.c que calcula la aproximacion de la funcion exponencial por un polinomio de Taylor de grado definido
- lagrange.c que calcula la aproximacion de la funcion exponencial en una serie de coordenadas a partir de la interpolacion de otro conjunto de puntos
- neville.c que calcula la aproximacion de la funcion exponencial en una serie de coordenadas a partir de la interpolacion de otro conjunto de puntos
- diferencias_divididas.c que calcula la aproximacion de la funcion exponencial en una serie de coordenadas a partir de la interpolacion de otro conjunto de puntos


## Ejecucion

Para correr el codigo taylor.c es necesario ejecutar el comando "./taylor z x0 n", con z el punto donde se calcula la aproximacion, x0 el punto donde se centra, y n el orden del polinomio de Taylor deseado.

Para el resto de codigos, basta con ejecutar "./codigo".

Los vectores donde se almacenan los conjuntos de observaciones deben tener la estructura

x1 x2 ... xn f(x1) f(x2) ... f(xn)