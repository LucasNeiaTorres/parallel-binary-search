parametrosCompilacao=-lpthread
arquivos=bsearchA.o 
all: bsearchA bsearchB chrono

bsearchA: bsearchA.o chrono.o
	gcc -o bsearchA bsearchA.o chrono.o $(parametrosCompilacao)

bsearchB: bsearchB.o chrono.o
	gcc -o bsearchB bsearchB.o chrono.o $(parametrosCompilacao)

chrono.o: chrono.h chrono.c
	gcc -c chrono.c $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch bsearchA bsearchB chrono

purge: clean
	-rm -f bsearchA bsearchB chrono