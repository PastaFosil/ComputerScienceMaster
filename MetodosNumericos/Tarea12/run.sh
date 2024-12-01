#!/bin/bash

N=$1

sed -i -E "14 s/^(int N = )[0-9]+;/\1$N;/" sistema_rk.cpp

g++ sistema_rk.cpp -o rk

./rk > "aprox.txt"

gnuplot -persist <<-EOF
    set terminal pngcairo enhanced size 800,600
    set output 'poblaciones_N_${N}.png'

    set title "Poblacion de conejos y linces, N=${N}"
    set xlabel "Año"
    set ylabel "Poblacion"
    set grid

    plot "vamos_linces.txt" using 1:2 with points pt 5 lc "red" title "Datos conejos", \
         "aprox.txt" using 1:2 with lines lc "red" title "Ajuste conejos", \
         "vamos_linces.txt" using 1:3 with points pt 5 lc "blue" title "Datos linces",\
         "aprox.txt" using 1:3 with lines lc "blue" title "Ajuste linces"
EOF

gnuplot -persist <<-EOF
    set terminal pngcairo enhanced size 800,600
    set output 'orbitas_N_${N}.png'

    set title "Orbitas de conejos y linces, N=${N}"
    set xlabel "Poblacion de conejos"
    set ylabel "Poblacion de linces"
    set grid
    unset key

    plot "aprox.txt" using 2:3 with lines lc "red"
EOF