#!/bin/bash

gcc taylor.c -o t -lm

z_values=(0 0.5 1 1.5 2.0)
n_values=(1 3 5 10)
x0=0

for n in "${n_values[@]}"; do
    out_file="taylor_$n.txt"
    > $out_file
    for z in "${z_values[@]}"; do
        ./t $z $x0 $n >> $out_file
    done
done

gnuplot -persist <<-EOF
    set terminal pngcairo size 800,600
    set output 'taylor.png'

    set title "Aproximaciones de la funcion exponencial"
    set xlabel "z"
    set ylabel "f(z)"
    set grid

    set xrange [0:2]

    set key top left

    plot exp(x) title "Real", \
         "taylor_1.txt" using 1:2 with linespoints title "n = 1", \
         "taylor_3.txt" using 1:2 with linespoints title "n = 3", \
         "taylor_5.txt" using 1:2 with linespoints title "n = 5", \
         "taylor_10.txt" using 1:2 with linespoints title "n = 10"
EOF