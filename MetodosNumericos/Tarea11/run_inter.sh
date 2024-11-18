#!/bin/bash

gcc interpolacion.c -o i -lm

outfile="inter.txt"

> $outfile
./i 1 >> $outfile

gnuplot -persist <<-EOF
    set terminal pngcairo size 800,600
    set output 'inter_cos.png'

    set title "Aproximacion de la funcion f(x)=P_4(x) con Cos(n{\Symbol p}x/6)"
    set xlabel "x"
    set ylabel "f(x)"
    set grid

    plot "data_pol.txt" using 1:2 with points title "Observaciones", \
         "inter.txt" using 1:2 with lines title "Interpolacion"
EOF

: <<'COMMENT'
> $outfile
./i 2 >> $outfile

gnuplot -persist <<-EOF
    set terminal pngcairo enhanced size 800,600
    set output 'cos.png'

    set title "Aproximacion de la funcion f(x)=cos(k{/Symbol p}x/6)"
    set xlabel "x"
    set ylabel "f(x)"
    set grid

    plot "data_cos.txt" using 1:2 with points title "Observaciones", \
         "inter.txt" using 1:2 with lines title "Interpolacion"
COMMENT
EOF