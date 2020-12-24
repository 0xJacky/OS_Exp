//
//  reader.c
//  Process-Readers-and-Writers
//
//  Created by Jacky on 2020/12/10.
//

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "utilities.h"
#include "writer.h"


void readBook(BookManager_t *bookMgr, int threadIdx) {
    sem_t *wmutex = bookMgr->wmutex;
    sem_t *rmutex = bookMgr->rmutex;
    
    // 防止多核心 CPU 多个线程在操作 readCount 时出现冲突
    // 锁定、自减读者信号量
    semWait(rmutex);
    // 没有读者时 锁定等待写者
    if (bookMgr->readCount == 0) semWait(wmutex); // 锁定、自减 写者信号量
    // 读者数量自增
    bookMgr->readCount++;
    // 解锁、自增读者信号量
    semSignal(rmutex);
    
    // 读
    printf("[Reader] The content of buffer is %s\n", bookMgr->buffer);

    // 防止多核心 CPU 多个线程在操作 readCount 时出现冲突
    // 锁定、自减读者信号量
    semWait(rmutex);
    bookMgr->readCount--;
    // 最后一个读者退出时，解锁、自增写者信号量
    if (bookMgr->readCount == 0) semSignal(wmutex); // 解锁、自增 写者信号量
    // 解锁、自增
    semSignal(rmutex);
}

void *readerThread(void *arg) {
    ThreadStruct_t *curThreadStruct = (ThreadStruct_t *)arg;
    BookManager_t *bookMgr = curThreadStruct->bookMgr;

    while (curThreadStruct->bookMgr->exit_flag == 0) {
        readBook(bookMgr, curThreadStruct->threadIdx);
        // wait WAIT_TIME_IN_SECOND seconds
        sleep(WAIT_TIME_IN_SECOND);
    }

    return arg;
}
