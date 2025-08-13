CSRC=main.c main.h lib.c lib.h

experi-omp: $(CSRC)
	gcc -O2 -o experi-omp -fopenmp $(CSRC) -lm

main.h: main.c

main.c: main.ngg
	ngg -oc main.c -oh main.h main.ngg
