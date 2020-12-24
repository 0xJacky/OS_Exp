#ifndef PRODUCERCONSUMER_UTILITIES_HEADER
#define PRODUCERCONSUMER_UTILITIES_HEADER

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define BUFFER_SIZE 10

#define WAIT_TIME_IN_SECOND 1

typedef struct {
    sem_t
        *mutex;  //Used for accessing buffer in exclusive way, initial value is 1
    sem_t
        *full;  //Used to indicate the number of boxes in the buffer that filled with product
    sem_t *empty;  //Used to indicate the number of empty boxes in the buffer

    int producerOffset;
    int consumerOffset;

    int remainProducts;
    int buffer[BUFFER_SIZE];
    int bufferSize;

    int exit_flag;
} ProducerConsumerManager_t;

typedef struct {
    ProducerConsumerManager_t *producerConsumerMgr;
    pthread_t tid;
    int threadIdx;
} ThreadStruct_t;

ProducerConsumerManager_t *createProducerConsumerManager();

void initThreadStruct(ThreadStruct_t *threadStruct,
                      ProducerConsumerManager_t *producerConsumerMgr,
                      int threadIdx);
ThreadStruct_t *createThreadStruct(
    ProducerConsumerManager_t *producerConsumerMgr, int threadIdx);

void semWait(sem_t *sem);
void semSignal(sem_t *sem);
void semDestroy(sem_t *sem);
#endif
