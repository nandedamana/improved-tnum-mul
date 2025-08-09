CSRC=main.c main.h lib.c lib.h

experi: $(CSRC)
	gcc -O2 -o experi $(CSRC)

main.h: main.c

main.c: main.ngg
	ngg -oc main.c -oh main.h main.ngg
