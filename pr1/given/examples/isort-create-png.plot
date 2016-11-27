# This plot script does the same as isort.plot but it creates a PNG
# image instead of displaying the figure on the screen. This is
# achieved by adding the two lines:
#
#   set terminal png
#   set output 'isort.png'
#
# before the plot command. You can also create PDF files and other
# formats using a similar approach, as described in the Gnuplot
# documentation.

set key left
set title 'insertion sort on random data'
set ylabel 'time [ms]'
set xlabel 'array length [N]'
set format x '%.1g'
set format y '%.2g'
set terminal png
set output 'isort.png'
plot 'data' u 1:7 w lp t 'min', '' u 1:8 w lp t 'avg', '' u 1:9 w lp t 'max'
