CSRC=main.c main.h lib.c lib.h nggvector.c nggvector.h
CFLAGS=-Wall -Wextra -O2

experi: $(CSRC)
	gcc $(CFLAGS) -o experi -fopenmp $(CSRC) -lm

main.h: main.c

main.c: main.ngg
	ngg -oc main.c -oh main.h main.ngg

nggvector.h: nggvector.c

nggvector.c: nggvector.ngg
	ngg -oc nggvector.c -oh nggvector.h nggvector.ngg

clean:
	rm -f experi
