#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

#define NUM_THREADS 8

pthread_t parallelReduce_Thread[ NUM_THREADS ];
int parallelReduce_thread_id[ NUM_THREADS ];
long long parallelReduce_partialSum[ NUM_THREADS ];   

int parallelReduce_nThreads;  // numero efetivo de threads
               // obtido da linha de comando  
int parallelReduce_nTotalElements;  // numero total de elementos
               // obtido da linha de comando      

pthread_barrier_t parallelReduce_barrier;

int bsearch_lower_bound(long long *input, int n, long long x) {
    int left = 0, right = n;

    while (left < right) {
        int m = left + (right - left) / 2;

        if (input[m] < x) 
            left = m + 1;
        else 
            right = m;
    }

    return left;
}

int bsearch_lower_bound_paralel(long long *input, int n, long long x) {
    int left = 0, right = n;

    while (left < right) {
        int m = left + (right - left) / 2;

        if (input[m] < x) 
            left = m + 1;
        else 
            right = m;
    }

    return left;
}

// Printa o vetor 
void printVetor(long long v[], int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%lld ", v[i]);
    }
    printf("\n");
}

// Geração de números aleatórios
void geraNaleatorios(long long v[], int n)
{
    for(int i = 0; i < n; i++)
    {
        v[i] = rand() % 10; // Gera números aleatórios de 0 a 9
    }
}

// Necessário pra usar o qsort
int compare(const void *a, const void *b) {
    return (*(long long *)a - *(long long *)b);
}

int main() {
    int n = 5;   // numero de elementos preenchidos no vetor
    long long *input = malloc(n * sizeof(long long));

    // Randomiza a SEED
    srand(time(NULL));

    // Cria o vetor v
    geraNaleatorios(input, n);

    // Ordena o vetor v
    qsort(input, n, sizeof(long long), compare);
 
    // Printa o vetor v
    printVetor(input, n);

    printf("Resultado com 0: %d\n", bsearch_lower_bound_paralel(input, n, 0));
    printf("Resultado com 1: %d\n", bsearch_lower_bound_paralel(input, n, 1));
    printf("Resultado com 2: %d\n", bsearch_lower_bound_paralel(input, n, 2));
    printf("Resultado com 4: %d\n", bsearch_lower_bound_paralel(input, n, 4));
    printf("Resultado com 8: %d\n", bsearch_lower_bound_paralel(input, n, 8)); 
    printf("Resultado com 9: %d\n", bsearch_lower_bound_paralel(input, n, 9)); 
    free(input);
    return 0;
}