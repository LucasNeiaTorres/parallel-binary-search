parametrosCompilacao=-lpthread
arquivos=bsearchA.o 
all: bsearchA

bsearchA: ${arquivos}
	gcc -o bsearchA ${arquivos} $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

run: bsearchA
	./bsearchA

clean:
	rm -f *.o *.gch bsearchA 

purge: clean
	-rm -f bsearchA