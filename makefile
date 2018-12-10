all: s.o
	gcc s.o
run: a.out
	./a.out
s.o: s.c
	gcc -c s.c
