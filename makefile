parametrosCompilacao=-lpthread
arquivos=bsearchA.o 
all: bsearchA bsearchB

bsearchA: bsearchA.o
	gcc -o bsearchA bsearchA.o $(parametrosCompilacao)

bsearchB: bsearchB.o
	gcc -o bsearchB bsearchB.o $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch bsearchA bsearchB 

purge: clean
	-rm -f bsearchA bsearchB