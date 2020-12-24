//
//  writer.c
//  Process-Readers-and-Writers
//
//  Created by Jacky on 2020/12/10.
//

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "utilities.h"
#include "writer.h"

void writeBook(BookManager_t *bookMgr, int threadIdx) {
    sem_t *wmutex = bookMgr->wmutex;

    // 锁定、自增 写者信号量
    semWait(wmutex);

    bookMgr->buffer[bookMgr->writePosition] = 'A' + bookMgr->writePosition;
    printf("[Writer] Add content %c to buffer\n",
           bookMgr->buffer[bookMgr->writePosition]);
    bookMgr->buffer[bookMgr->writePosition + 1] = '\0';

    bookMgr->writePosition++;
    if (bookMgr->writePosition == 26) {
        printf("[Writer] Add content to buffer from begining\n");
        bookMgr->writePosition = 0;
    }
    // 解锁、自减 写者信号量
    semSignal(wmutex);
}

void *writerThread(void *arg) {
    ThreadStruct_t *curThreadStruct = (ThreadStruct_t *)arg;
    BookManager_t *bookMgr = curThreadStruct->bookMgr;

    while (curThreadStruct->bookMgr->exit_flag == 0) {
        writeBook(bookMgr, curThreadStruct->threadIdx);
        // wait WAIT_TIME_IN_SECOND seconds
        sleep(WAIT_TIME_IN_SECOND);
    }

    return arg;
}
