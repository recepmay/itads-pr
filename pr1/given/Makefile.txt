all: insertion-sort-benchmark merge-sort-benchmark

insertion-sort-benchmark: insertion-sort-benchmark.c Makefile
	gcc -Wall -o insertion-sort-benchmark insertion-sort-benchmark.c

merge-sort-benchmark: merge-sort-benchmark.c Makefile
	gcc -Wall -o merge-sort-benchmark merge-sort-benchmark.c
