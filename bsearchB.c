#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 8
#define MAX_TASKS 16
#define TAM_Q 100000

// Implementação de busca binária (bsearch_lower_bound)
int single_bsearch_lower_bound(long long *input, int n, long long x) {
    int left = 0, right = n;
    while (left < right) {
        int m = left + (right - left) / 2;
        if (input[m] < x) left = m + 1;
        else right = m;
    }
    return left;
}

// bsearch_lower_bound com varias pesquisas
void bsearch_lower_bound(long long *input, int n, long long *Q, long long nQ, long long *Pos) {
    for (int i = 0; i < nQ; i++) {
        Pos[i] = single_bsearch_lower_bound(input, n, Q[i]);
    }
}

// Funções auxiliares de geração e ordenação
void geraNaleatorios(long long v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % 1000;
    }
}

int compare(const void *a, const void *b) {
    return (*(long long *)a - *(long long *)b);
}

void printVetor(long long v[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%lld ", v[i]);
    }
    printf("\n");
}

int main() {
    int n = 16;
    long long *input = malloc(n * sizeof(long long));

    srand(time(NULL));

    geraNaleatorios(input, n);
    qsort(input, n, sizeof(long long), compare);

    printVetor(input, n);

    // init_thread_pool();

    long long *Q = malloc(TAM_Q * sizeof(long long));  
    long long *Pos = malloc(TAM_Q * sizeof(long long));  
    long long nQ = 3;

    geraNaleatorios(Q, nQ);
    printVetor(Q, nQ);

    bsearch_lower_bound(input, n, Q, nQ, Pos); 
    printf("Resultado normal:\n");
    printVetor(Pos, nQ);

    // int resultNormal = bsearch_lower_bound(input, n, target);
    // printf("Resultado normal com %lld: %d\n", target, resultNormal);

    free(input);
    return 0;
}