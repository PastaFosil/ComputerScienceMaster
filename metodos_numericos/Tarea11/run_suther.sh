#!/bin/bash

gcc sutherland.c -o s -lm

lambda=("0" "1e-5" "1e-7")
function=(1 2 3)
files=()
xmin=0.273
xmax=1.5
T=1.2
ex="a"

for f in "${function[@]}"; do
    for l in "${lambda[@]}"; do
        outfile="vis_f_${f}_lambda_$l.txt"
        files+=($outfile)
        > $outfile
        ./s $l $f $xmin $xmax >> $outfile
        > ${ex}_inter_${outfile}
        ./s $l $f $T $T >> ${ex}_inter_$outfile
    done
done

gnuplot -persist <<-EOF
    set terminal pngcairo enhanced size 800,600
    set output 'curves_suther_pol_${ex}.png'

    set title "Ajuste con funcion polinomica"
    set xlabel "T"
    set ylabel "f(x)"
    set grid
    set key bottom right

    set xrange[0.273:1.5]

    plot "vis_data_${ex}.txt" using 1:2 with points ps 2 title "Observaciones", \
         "${files[0]}" using 1:2 with lines lw 4 lc "yellow" title "{/Symbol l}=0.0", \
         "${ex}_inter_${files[0]}" using 1:2 with points pt 3 lc "yellow" title "f(1.2) con {/Symbol l}=0.0", \
         "${files[1]}" using 1:2 with lines lw 2 lc "red" title "{/Symbol l}=1e-5", \
         "${ex}_inter_${files[1]}" using 1:2 with points pt 3 lc "red" title "f(1.2) con {/Symbol l}=1e-5", \
         "${files[2]}" using 1:2 with lines lw 1 lc "black" title "{/Symbol l}=1e-7", \
         "${ex}_inter_${files[2]}" using 1:2 with points pt 3 lc "black" title "f(1.2) con {/Symbol l}=1e-7"
EOF

gnuplot -persist <<-EOF
    set terminal pngcairo enhanced size 800,600
    set output 'curves_suther_cos_${ex}.png'

    set title "Ajuste con funcion cos(k{/Symbol p}x/6)"
    set xlabel "T"
    set ylabel "f(x)"
    set grid
    set key bottom right

    set xrange[0.273:1.5]

    plot "vis_data_${ex}.txt" using 1:2 with points ps 2 title "Observaciones", \
         "${files[3]}" using 1:2 with lines lc "yellow" title "{/Symbol l}=0.0", \
         "${ex}_inter_${files[3]}" using 1:2 with points pt 3 lc "yellow" title "f(1.2) con {/Symbol l}=0.0", \
         "${files[4]}" using 1:2 with lines lc "red" title "{/Symbol l}=1e-5", \
         "${ex}_inter_${files[4]}" using 1:2 with points pt 3 lc "red" title "f(1.2) con {/Symbol l}=1e-5", \
         "${files[5]}" using 1:2 with lines lc "black" title "{/Symbol l}=1e-7", \
         "${ex}_inter_${files[5]}" using 1:2 with points pt 3 lc "black" title "f(1.2) con {/Symbol l}=1e-7"
EOF

gnuplot -persist <<-EOF
    set terminal pngcairo enhanced size 800,600
    set output 'curves_suther_exp_${ex}.png'

    set title "Ajuste con funcion e^{-r^2}"
    set xlabel "T"
    set ylabel "f(x)"
    set grid
    set key bottom right

    set xrange[0.273:1.5]

    plot "vis_data_${ex}.txt" using 1:2 with points ps 2 title "Observaciones", \
         "${files[6]}" using 1:2 with lines lc "yellow" title "{/Symbol l}=0.0", \
         "${ex}_inter_${files[6]}" using 1:2 with points pt 3 lc "yellow" title "f(1.2) con {/Symbol l}=0.0", \
         "${files[7]}" using 1:2 with lines lc "red" title "{/Symbol l}=1e-5", \
         "${ex}_inter_${files[7]}" using 1:2 with points pt 3 lc "red" title "f(1.2) con {/Symbol l}=1e-5", \
         "${files[8]}" using 1:2 with lines lc "black" title "{/Symbol l}=1e-7", \
         "${ex}_inter_${files[8]}" using 1:2 with points pt 3 lc "black" title "f(1.2) con {/Symbol l}=1e-7"
EOF