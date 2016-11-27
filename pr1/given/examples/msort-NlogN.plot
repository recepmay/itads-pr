O(N) = N * log(N)
set key left
set title 'merge sort on random data, convergence for O(N log N)'
set ylabel 'time [ms] / (N log N)'
set xlabel 'array length [N]'
set format x '%.1g'
set format y '%.2g'
plot 'data' u 1:($7/O($1)) w lp t 'min', \
         '' u 1:($8/O($1)) w lp t 'avg', \
         '' u 1:($9/O($1)) w lp t 'max'
