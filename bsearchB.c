#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "chrono.h"

#define MAX_THREADS 8
#define MAX_TASKS 16
#define TAM_Q 100000
#define MAX_ELEM 1600000
#define NTIMES 10

typedef struct {
    long long *input;
    int left;
    int right;
    long long target;
    int *result;
    int id;
} ThreadData;

long long inputG[10 * MAX_ELEM];
pthread_t threads[MAX_THREADS];
ThreadData taskQueue[MAX_TASKS];
int taskIndex = 0;
int tasksInQueue = 0;
int completedTasks = 0;
int numThreads;

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t completeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t completeCond = PTHREAD_COND_INITIALIZER;

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
void bsearch_lower_bound(long long *input, int n, long long *Q, long long nQ, long long int *Pos) {
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

void concatenaVetor(long long *inputG[], long long G[], int numElem){
    memcpy(inputG, G, sizeof(long long));
    for (int i = 0; i < 10; i++) {
        memcpy(inputG + (numElem*i), G, numElem*sizeof(long long));
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

// Função de trabalho da thread
void *thread_worker(void *arg) {
    while (1) {
        pthread_mutex_lock(&queueMutex);

        while (tasksInQueue == 0) pthread_cond_wait(&queueCond, &queueMutex);

        // Pega uma tarefa da fila
        ThreadData task = taskQueue[taskIndex];
        taskIndex = (taskIndex + 1) % MAX_TASKS;
        tasksInQueue--;

        pthread_mutex_unlock(&queueMutex);

        // Busca binária na faixa designada
        *task.result = single_bsearch_lower_bound(task.input, task.right, task.target);
        // printf("Resultado da thread de inicio %d e fim %d: %d\n", task.left, task.right, *task.result);

        // Marca a tarefa como concluída
        pthread_mutex_lock(&completeMutex);
        completedTasks++;
        if (completedTasks == numThreads) {
            pthread_cond_signal(&completeCond);
        }
        pthread_mutex_unlock(&completeMutex);
    }
}

// Inicializa o pool de threads
void init_thread_pool() {
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, thread_worker, NULL);
    }
}

// Adiciona uma tarefa ao pool de threads
void add_task(long long *input, int left, int right, long long target, int *result) {
    pthread_mutex_lock(&queueMutex);

    taskQueue[(taskIndex + tasksInQueue) % MAX_TASKS] = (ThreadData){input, left, right, target, result, -1};
    tasksInQueue++;

    pthread_cond_signal(&queueCond);
    pthread_mutex_unlock(&queueMutex);
}

// Função de busca paralela com espera pelas threads
void parallel_bsearch_lower_bound(long long *input, int n, long long *Q, long long nQ, long long int *Pos) {
    ThreadData threadData[nQ];
    completedTasks = 0;

    for(int i = 0; i < nQ; i++){
        threadData[i].input = input;
        threadData[i].left = 0;
        threadData[i].right = n;
        threadData[i].target = Q[i];
        threadData[i].result = malloc(sizeof(int));
        *threadData[i].result = n;
        add_task(input, threadData[i].left, threadData[i].right, Q[i], threadData[i].result);
    }

    // Aguarda até que todas as tarefas estejam concluídas
    pthread_mutex_lock(&completeMutex);
    while (completedTasks < numThreads) {
        pthread_cond_wait(&completeCond, &completeMutex);
    }
    pthread_mutex_unlock(&completeMutex);

    for(int i = 0; i < nQ; i++){
        Pos[i] = *threadData[i].result;
        free(threadData[i].result);
    }
}

int main() {
    long long numElem = 100; // Supondo que numElem é 100
    long long nQ = 100000;

    long long *input = malloc(numElem * sizeof(long long));
    if (input == NULL) {
        fprintf(stderr, "Erro ao alocar memória para input\n");
        return 1;
    }

    long long *Q = malloc(nQ * sizeof(long long));
    if (Q == NULL) {
        fprintf(stderr, "Erro ao alocar memória para Q\n");
        free(input);
        return 1;
    }

    long long *Pos = malloc(TAM_Q * sizeof(long long));
    if (Pos == NULL) {
        fprintf(stderr, "Erro ao alocar memória para Pos\n");
        free(input);
        free(Q);
        return 1;
    }

    long long *inputG = malloc(NTIMES * numElem * sizeof(long long));
    if (inputG == NULL) {
        fprintf(stderr, "Erro ao alocar memória para inputG\n");
        free(input);
        free(Q);
        free(Pos);
        return 1;
    }

    long long *QG = malloc(NTIMES * nQ * sizeof(long long));
    if (QG == NULL) {
        fprintf(stderr, "Erro ao alocar memória para QG\n");
        free(input);
        free(Q);
        free(Pos);
        free(inputG);
        return 1;
    }

    geraNaleatorios(Q, nQ);
    // printVetor(Q, nQ);

    for (int j = 0; j < NTIMES; j++) {
        memcpy(inputG + (j * numElem), input, numElem * sizeof(long long));
        memcpy(QG + (j * nQ), Q, nQ * sizeof(long long));
    }

    init_thread_pool();

    double timeSeconds = 0.0;
    chronometer_t time;
    chrono_reset(&time);
    chrono_start(&time);
    for (int i = 0; i < NTIMES; i++) {
        parallel_bsearch_lower_bound(&inputG[i * numElem], numElem, &QG[i * nQ], nQ, Pos);
    }
    chrono_stop(&time);
    // chrono_reportTime(&time, "time: ");
    timeSeconds += (double)chrono_gettotal(&time) / ((double)1000 * 1000 * 1000); // NanoSeconds para Seconds
    double tempoMedio = timeSeconds / NTIMES;
    printf("\ntotal_time_in_seconds: %lf s\n", tempoMedio);

    free(input);
    free(Q);
    free(Pos);
    free(inputG);
    free(QG);

    return 0;
}