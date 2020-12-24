//
//  mainProg.c
//  Process-Readers-and-Writers
//
//  Created by Jacky on 2020/12/10.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "utilities.h"
#include "writer.h"

#define WRITER_NUMBER 10
#define READER_NUMBER 2

int main(int argc, char *argv[]) {
    int idx = 0;
    
    printf("[Main] Start\n");
    
    // 创建线程结构体的堆区
    ThreadStruct_t *writerThreadStructPtr =
        (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t) * WRITER_NUMBER);
    ThreadStruct_t *readerThreadStructPtr =
    
    // 创建 bookMgr 堆区用于在线程中共享数据
        (ThreadStruct_t *)malloc(sizeof(ThreadStruct_t) * READER_NUMBER);
    BookManager_t *bookMgr = createBookManager();
    
    if (NULL == writerThreadStructPtr || NULL == readerThreadStructPtr ||
        NULL == bookMgr) {
        perror("[Main] malloc fail!\n");
        exit(EXIT_FAILURE);
    }

    // 创建写者进程
    for (idx = 0; idx < WRITER_NUMBER; ++idx) {
        ThreadStruct_t *curWriterThreadStruct = writerThreadStructPtr + idx;
        initThreadStruct(curWriterThreadStruct, bookMgr, idx);
        pthread_create(&curWriterThreadStruct->tid, NULL, writerThread,
                       (void *)curWriterThreadStruct);
    }

    // 创建读者进程
    for (idx = 0; idx < READER_NUMBER; ++idx) {
        ThreadStruct_t *curReaderThreadStruct = readerThreadStructPtr + idx;
        initThreadStruct(curReaderThreadStruct, bookMgr, idx);
        pthread_create(&curReaderThreadStruct->tid, NULL, readerThread,
                       (void *)curReaderThreadStruct);
    }

    // 输入 q 退出
    do {
        char aChar = getchar();
        if (aChar == 'q') {
            bookMgr->exit_flag = 1;
            printf("[Main] Receive exit instruction\n");
        }
    } while (bookMgr->exit_flag == 0);

    // 等待写者线程全部退出
    for (idx = 0; idx < WRITER_NUMBER; ++idx) {
        ThreadStruct_t *curWriterThreadStruct = writerThreadStructPtr + idx;
        pthread_join(curWriterThreadStruct->tid, NULL);
    }

    // 等待读者线程全部退出
    for (idx = 0; idx < READER_NUMBER; ++idx) {
        ThreadStruct_t *curReaderThreadStruct = readerThreadStructPtr + idx;
        pthread_join(curReaderThreadStruct->tid, NULL);
    }

    printf("[Main] free ThreadStructPtrs\n");
    free(writerThreadStructPtr);
    free(readerThreadStructPtr);
    printf("[Main] close semaphores\n");
    semClose(bookMgr->rmutex);
    semClose(bookMgr->wmutex);
    printf("[Main] unlink semaphores\n");
    sem_unlink(RMUTEX_NAME);
    sem_unlink(WMUTEX_NAME);
    printf("[Main] free BookManager\n");
    free(bookMgr);
    printf("[Main] Bye\n");
    return 0;
}
