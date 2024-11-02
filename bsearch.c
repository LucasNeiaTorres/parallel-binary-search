#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

#define MAX_THREADS 8

typedef struct
{
    long long *input;
    int left;
    int right;
    long long target;
    int result; // Índice de resposta para a thread
} ThreadData;

pthread_t threads[MAX_THREADS];
int threads_id[MAX_THREADS];

int bsearch_lower_bound(long long *input, int n, long long x)
{
    int left = 0, right = n;

    while (left < right)
    {
        int m = left + (right - left) / 2;

        if (input[m] < x)
            left = m + 1;
        else
            right = m;
    }

    return left;
}

void *bsearch_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int left = data->left;
    int right = data->right;
    long long target = data->target;
    long long *input = data->input;

    while (left < right)
    {
        int m = left + (right - left) / 2;

        if (input[m] < target)
            left = m + 1;
        else
            right = m;
    }
    data->result = left;
    // printf("Resultado da thread de inicio %d e fim %d: %d\n", data->left, data->right, data->result);
    pthread_exit(NULL);
}

int parallel_bsearch_lower_bound(long long *input, int n, long long x)
{
    int i;
    ThreadData threadData[MAX_THREADS];
    // Cria as threads para pesquisar em sub-faixas do array
    for (i = 0; i < MAX_THREADS; i++)
    {
        threadData[i].input = input;
        threadData[i].left = i * n / MAX_THREADS;
        threadData[i].right = (i + 1) * n / MAX_THREADS;
        threadData[i].target = x;
        threadData[i].result = -1;
        pthread_create(&threads[i], NULL, bsearch_thread, &threadData[i]);
    }

    // Aguarda o término das threads
    for (i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Encontra o menor índice retornado pelas threads
    int result = n;
    for (i = 0; i < MAX_THREADS; i++)
    {
        // printf("Target: %lld - Valor de result: %lld - Próximo: %lld\n", x, input[threadData[i].result], input[threadData[i].result + 1]);
        if ((input[threadData[i].result] >= x) && threadData[i].result < result)
        {
            result = threadData[i].result;
        }
    }

    return result;
}

// Printa o vetor l
void printVetor(long long v[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%lld ", v[i]);
    }
    printf("\n");
}

// Geração de números aleatórios
void geraNaleatorios(long long v[], int n)
{
    for (int i = 0; i < n; i++)
    {
        v[i] = rand() % 1000; // Gera números aleatórios de 0 a 1000
    }
}

// Necessário pra usar o qsort
int compare(const void *a, const void *b)
{
    return (*(long long *)a - *(long long *)b);
}

int main()
{
    int n = 16; // numero de elementos preenchidos no vetor
    long long *input = malloc(n * sizeof(long long));

    // Randomiza a SEED
    srand(time(NULL));

    // Cria o vetor v
    geraNaleatorios(input, n);

    // Ordena o vetor v
    qsort(input, n, sizeof(long long), compare);

    // Printa o vetor v
    printVetor(input, n);

    int result = bsearch_lower_bound(input, n, 565);
    printf("Resultado normal com 5: %d\n", result);

    result = parallel_bsearch_lower_bound(input, n, 565);
    printf("Resultado paralelo com 5: %d\n", result);

    // printf("Resultado com 0: %d\n", parallel_bsearch_lower_bound(input, n, 0));
    // printf("Resultado com 1: %d\n", parallel_bsearch_lower_bound(input, n, 1));
    // printf("Resultado com 2: %d\n", parallel_bsearch_lower_bound(input, n, 2));
    // printf("Resultado com 4: %d\n", parallel_bsearch_lower_bound(input, n, 4));
    // printf("Resultado com 8: %d\n", parallel_bsearch_lower_bound(input, n, 8));
    // printf("Resultado com 9: %d\n", parallel_bsearch_lower_bound(input, n, 9));
    free(input);
    return 0;
}