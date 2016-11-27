set key right
set title 'average runtimes on random data'
set ylabel 'time [ms]'
set xlabel 'array length [N]'
set format x '%.1g'
set format y '%.2g'
plot 'isort.data' u 1:8 w lp t 'insertion sort', 'msort.data' u 1:7 w lp t 'merge sort'
