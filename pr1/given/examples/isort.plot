set key left
set title 'insertion sort on random data'
set ylabel 'time [ms]'
set xlabel 'array length [N]'
set format x '%.1g'
set format y '%.2g'
plot 'data' u 1:7 w lp t 'min', '' u 1:8 w lp t 'avg', '' u 1:9 w lp t 'max'
