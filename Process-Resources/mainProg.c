#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "Consumer.h"
#include "Producer.h"
#include "ProducerConsumerUtilities.h"

#define PRODUCER_NUMBER 1
#define CONSUMER_NUMBER 3

int main(int argc, char *argv[]) {
    int idx = 0;

    ThreadStruct_t *producerThreadStructPtr =
        (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t) * PRODUCER_NUMBER);
    ThreadStruct_t *consumerThreadStructPtr =
        (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t) * CONSUMER_NUMBER);
    ProducerConsumerManager_t *producerConsumerMgr =
        createProducerConsumerManager();
    if (NULL == producerThreadStructPtr || NULL == consumerThreadStructPtr ||
        NULL == producerConsumerMgr) {
        perror("malloc fail!\n");
        exit(EXIT_FAILURE);
    }

    for (idx = 0; idx < PRODUCER_NUMBER; ++idx) {
        ThreadStruct_t *curProducerThreadStruct = producerThreadStructPtr + idx;
        initThreadStruct(curProducerThreadStruct, producerConsumerMgr, idx);
        pthread_create(&curProducerThreadStruct->tid, NULL, producerThread,
                       (void *)curProducerThreadStruct);
    }

    for (idx = 0; idx < CONSUMER_NUMBER; ++idx) {
        ThreadStruct_t *curConsumerThreadStruct = consumerThreadStructPtr + idx;
        initThreadStruct(curConsumerThreadStruct, producerConsumerMgr, idx);
        pthread_create(&curConsumerThreadStruct->tid, NULL, consumerThread,
                       (void *)curConsumerThreadStruct);
    }

    do {
        char aChar = getchar();
        if (aChar == 'q') {
            producerConsumerMgr->exit_flag = 1;
        }
    } while (producerConsumerMgr->exit_flag == 0);

    //Wait producer threads exit
    for (idx = 0; idx < PRODUCER_NUMBER; ++idx) {
        ThreadStruct_t *curProducerThreadStruct = producerThreadStructPtr + idx;
        pthread_join(curProducerThreadStruct->tid, NULL);
    }

    //Wait consumer threads exit
    for (idx = 0; idx < CONSUMER_NUMBER; ++idx) {
        ThreadStruct_t *curConsumerThreadStruct = consumerThreadStructPtr + idx;
        pthread_join(curConsumerThreadStruct->tid, NULL);
    }

    free(producerThreadStructPtr);
    free(consumerThreadStructPtr);
    semDestroy(producerConsumerMgr->mutex);
    semDestroy(producerConsumerMgr->full);
    semDestroy(producerConsumerMgr->empty);
    free(producerConsumerMgr);
    return 1;
}
