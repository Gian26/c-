set title "Grafica de los errores para cada validacion de la Red Neuronal Artificial"
set xlabel "iteracion"
set ylabel "error"
set yrange [ -.1 : * ] noreverse nowriteback
plot "results.data" using 2 title 'training set 1' with lines, \
     "results.data" using 3 title 'training set 2' with lines, \
     "results.data" using 4 title 'training set 3' with lines, \
     "results.data" using 5 title 'training set 4' with lines
