O(N) = N * N
set key right
set title 'insertion sort on random data, convergence for O(N^2)'
set ylabel 'time [ms] / (N^2)'
set xlabel 'array length [N]'
set format x '%.1g'
set format y '%.2g'
set yrange [0:2e-6]
plot 'data' u 1:($7/O($1)) w lp t 'min', \
         '' u 1:($8/O($1)) w lp t 'avg', \
         '' u 1:($9/O($1)) w lp t 'max'
