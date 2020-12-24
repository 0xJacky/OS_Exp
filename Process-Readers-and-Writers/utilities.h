//
//  utilities.h
//  Process-Readers-and-Writers
//
//  Created by Jacky on 2020/12/10.
//

#ifndef utilities_h
#define utilities_h

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define N 100
#define BUFFER_SIZE 100

#define WAIT_TIME_IN_SECOND 1
#define RMUTEX_NAME "/rmutex"
#define WMUTEX_NAME "/wmutex"

typedef struct {
    // 互斥型号量
    sem_t *rmutex;
    sem_t *wmutex;

    // 书本内容
    char buffer[BUFFER_SIZE];
    int writePosition;

    // 读者数量
    int readCount;

    // 退出标记
    int exit_flag;

} BookManager_t;

typedef struct {
    BookManager_t *bookMgr;
    pthread_t tid;
    int threadIdx;
} ThreadStruct_t;

BookManager_t *createBookManager(void);

void initThreadStruct(ThreadStruct_t *threadStruct, BookManager_t *bookMgr,
                      int threadIdx);
ThreadStruct_t *createThreadStruct(BookManager_t *bookMgr, int threadIdx);

void semWait(sem_t *sem);
void semSignal(sem_t *sem);
void semClose(sem_t *sem);
#endif
