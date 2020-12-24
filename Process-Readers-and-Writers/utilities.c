//
//  utilities.c
//  Process-Readers-and-Writers
//
//  Created by Jacky on 2020/12/10.
//

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"


void initBookManager(BookManager_t *bookMgr) {
    sem_unlink(RMUTEX_NAME);
    sem_unlink(WMUTEX_NAME);

    bookMgr->rmutex = sem_open(RMUTEX_NAME, O_CREAT, 0600, 1);
    bookMgr->wmutex = sem_open(WMUTEX_NAME, O_CREAT, 0600, 1);
    bookMgr->readCount = 0;
    bookMgr->writePosition = 0;
    bookMgr->exit_flag = 0;
}

BookManager_t *createBookManager() {
    BookManager_t *bookMgr = (BookManager_t *)malloc(sizeof(BookManager_t));

    if (bookMgr == NULL) {
        printf("[Main] Error: unable to create bookMgr\n");
        return NULL;
    }

    initBookManager(bookMgr);

    return bookMgr;
}

void initThreadStruct(ThreadStruct_t *threadStruct, BookManager_t *bookMgr,
                      int threadIdx) {
    threadStruct->bookMgr = bookMgr;
    threadStruct->threadIdx = threadIdx;
}

ThreadStruct_t *createThreadStruct(BookManager_t *bookMgr, int threadIdx) {
    ThreadStruct_t *threadStruct =
        (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t));

    if (threadStruct == NULL) {
        printf("[Main] Error: unable to create ThreadStruct_t\n");
        return NULL;
    }

    initThreadStruct(threadStruct, bookMgr, threadIdx);

    return threadStruct;
}

void semWait(sem_t *sem) {
    int ret = sem_wait(sem);
    if (ret < 0) {
        perror("[Main] sem_wait fail!");
        exit(EXIT_FAILURE);
    }
}

void semSignal(sem_t *sem) {
    int ret = sem_post(sem);
    if (ret < 0) {
        perror("[Main] sem_post fail!\n");
        exit(EXIT_FAILURE);
    }
}

void semClose(sem_t *sem) {
    int ret = sem_close(sem);
    if (ret < 0) {
        perror("[Main] sem_close run fail\n");
        exit(EXIT_FAILURE);
    }
}
