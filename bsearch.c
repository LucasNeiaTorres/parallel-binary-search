#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

#define MAX_THREADS 8

pthread_t parallelReduce_Thread[ MAX_THREADS ];
int parallelReduce_thread_id[ MAX_THREADS ];
long long parallelReduce_partialSum[ MAX_THREADS ];   

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

int main() {
    int n = 5;   // numero de elementos preenchidos no vetor
    long long *input = (long long *) malloc(n * sizeof(long long));
    input[0] = 2;
    input[1] = 2;
    input[2] = 4;
    input[3] = 4;
    input[4] = 7;
 
    printf("Resultado: %d\n", bsearch_lower_bound_paralel(input, n, 0)); // retorna 0
    printf("Resultado: %d\n", bsearch_lower_bound_paralel(input, n, 1)); // retorna 1
    printf("Resultado: %d\n", bsearch_lower_bound_paralel(input, n, 2)); // retorna 1
    printf("Resultado: %d\n", bsearch_lower_bound_paralel(input, n, 4)); // retorna 2
    printf("Resultado: %d\n", bsearch_lower_bound_paralel(input, n, 8)); // retorna 4
    printf("Resultado: %d\n", bsearch_lower_bound_paralel(input, n, 9)); // retorna 5
    return 0;
}