#!/bin/bash

# EJECUTAR "chmod +x run.sh" PARA PODER CORRER ESTE ARCHIVO Y GRAFICAR HISTOGRAMA
file=$1

max=$(head -n 1 hist_${file})
max=$((max+1))

gnuplot -persist <<-EOF
    set terminal qt
    set style data histogram
    set style histogram cluster gap 10
    set style fill solid
    set boxwidth 1.0 relative

    set title "Histograma de factor de carga de ${file}"
    set xlabel "Indice"
    set ylabel "Factor de carga"
    
    set xtics rotate by -90 font ",5"
    set yrange [0:${max}]
    set nokey

    plot 'hist_${file}' every ::1 using 2:xtic(1) title 'Libros' with boxes
EOF

