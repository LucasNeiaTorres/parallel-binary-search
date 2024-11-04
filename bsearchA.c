#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "chrono.h"

#define MAX_THREADS 8
#define MAX_TASKS 16
#define MAX_ELEM 16000000
#define NTIMES 10

typedef struct
{
    long long *input;
    int left;
    int right;
    long long target;
    int *result;
    int id;
} ThreadData;

pthread_t threads[MAX_THREADS];
ThreadData taskQueue[MAX_TASKS];
int taskIndex = 0;
int tasksInQueue = 0;
int completedTasks = 0;

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t completeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t completeCond = PTHREAD_COND_INITIALIZER;

int numThreads;

// Implementação de busca binária (bsearch_lower_bound)
int bsearch_lower_bound(long long *input, int left, int right, long long x)
{
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

// Função de trabalho da thread
void *thread_worker(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&queueMutex);

        while (tasksInQueue == 0)
            pthread_cond_wait(&queueCond, &queueMutex);

        // Pega uma tarefa da fila
        ThreadData task = taskQueue[taskIndex];
        taskIndex = (taskIndex + 1) % MAX_TASKS;
        tasksInQueue--;

        pthread_mutex_unlock(&queueMutex);

        // Busca binária na faixa designada
        *task.result = bsearch_lower_bound(task.input, task.left, task.right, task.target);
        // printf("Resultado da thread de inicio %d e fim %d: %d\n", task.left, task.right, *task.result);

        // Marca a tarefa como concluída
        pthread_mutex_lock(&completeMutex);
        completedTasks++;
        if (completedTasks == numThreads)
        {
            pthread_cond_signal(&completeCond);
        }
        pthread_mutex_unlock(&completeMutex);
    }
}

// Inicializa o pool de threads
void init_thread_pool()
{
    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&threads[i], NULL, thread_worker, NULL);
    }
}

// Adiciona uma tarefa ao pool de threads
void add_task(long long *input, int left, int right, long long target, int *result)
{
    pthread_mutex_lock(&queueMutex);

    taskQueue[(taskIndex + tasksInQueue) % MAX_TASKS] = (ThreadData){input, left, right, target, result, -1};
    tasksInQueue++;

    pthread_cond_signal(&queueCond);
    pthread_mutex_unlock(&queueMutex);
}

// Função de busca paralela com espera pelas threads
int parallel_bsearch_lower_bound(long long *input, int n, long long x)
{
    ThreadData threadData[numThreads];
    completedTasks = 0;

    // Cria as tarefas para cada thread
    for (int i = 0; i < numThreads; i++)
    {
        threadData[i].input = input;
        threadData[i].left = i * n / numThreads;
        threadData[i].right = (i + 1) * n / numThreads;
        threadData[i].target = x;
        threadData[i].result = malloc(sizeof(int));
        *threadData[i].result = n;
        add_task(input, threadData[i].left, threadData[i].right, x, threadData[i].result);
    }

    // Aguarda até que todas as tarefas estejam concluídas
    pthread_mutex_lock(&completeMutex);
    while (completedTasks < numThreads)
    {
        pthread_cond_wait(&completeCond, &completeMutex);
    }
    pthread_mutex_unlock(&completeMutex);

    // Calcula o menor índice resultante
    int result = n;
    for (int i = 0; i < numThreads; i++)
    {
        // printf("Thread %d: %d\n", i, *threadData[i].result);
        if ((*threadData[i].result < n) && (input[*threadData[i].result] >= x))
        {
            result = *threadData[i].result < result ? *threadData[i].result : result;
        }
        free(threadData[i].result);
    }

    return result;
}

// Funções auxiliares de geração e ordenação
void geraNaleatorios(long long v[], int n)
{
    for (int i = 0; i < n; i++)
    {
        v[i] = rand() % 1000;
    }
}

int compare(const void *a, const void *b)
{
    return (*(long long *)a - *(long long *)b);
}

void printVetor(long long v[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%lld ", v[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int numElem;
    if (argc != 3) {
        printf("usage: %s <numElem> <nThreads>\n", argv[0]);
        return 0;
    } else if(atoi(argv[2]) > MAX_THREADS) {
        printf("Número de threads maior que o máximo permitido\n");
        return 0;
    } else if(atoi(argv[1]) > MAX_ELEM) {
        printf("Número de elementos maior que o máximo permitido\n");
        return 0;
    } else {
        numThreads = atoi(argv[2]);
        numElem = atoi(argv[1]);
    }

    long long *input = malloc(numElem * sizeof(long long));
    long long target = 755;
    double timeSeconds;

    srand(time(NULL));

    geraNaleatorios(input, numElem);
    qsort(input, numElem, sizeof(long long), compare);

    // printVetor(input, n);

    // Começa o cronômetro
    chronometer_t time;
    chrono_reset(&time);
    chrono_start(&time);

    init_thread_pool();

    int result = parallel_bsearch_lower_bound(input, numElem, target);
    // printf("Resultado paralelo com %lld: %d\n", target, result);

    // int resultNormal = bsearch_lower_bound(input, 0, numElem, target);
    // printf("Resultado normal com %lld: %d\n", target, resultNormal);

    // Para o cronometro
    chrono_stop(&time);

    // Printando tempo e MOPS
    chrono_reportTime(&time, "time: ");
    timeSeconds = (double) chrono_gettotal(&time) / ((double)1000 * 1000 * 1000); // NanoSeconds para Seconds
    printf("\nO algoritmo demorou: %lf ms\n", timeSeconds);
    // printf("E a vazão foi de: %lf MOPS", (numElem/timeSeconds));  // MOPS
    // double OPS = ((double)nTotalElements*NTIMES)/total_time_in_seconds;
    // printf( "Throughput: %lf OP/s\n", OPS );
    // verificar como calcula a vazao

    free(input);
    return 0;
}