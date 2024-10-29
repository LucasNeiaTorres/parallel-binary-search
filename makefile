parametrosCompilacao=-lpthread
arquivos=bsearch.o 
all: bsearch

bsearch: ${arquivos}
	gcc -o bsearch ${arquivos} $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

run: bsearch
	./bsearch

clean:
	rm -f *.o *.gch bsearch 

purge: clean
	-rm -f bsearch