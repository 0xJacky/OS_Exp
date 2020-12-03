//
//  IOTask.c
//  IO-Test
//
//  Created by Jacky on 2020/11/19.
//

#include "IOTask.h"

char filename[] = "IOTest"; //即将创建和打开的文件名

double getTimeElapse(struct timeval start, struct timeval end) {
    return (double) (end.tv_sec - start.tv_sec) * 1000.0 + (double) (end.tv_usec - start.tv_usec) / 1000.0;
}

double transSpeed(ssize_t size, double t) {
    return ((((double)size/1024.0)/1024.0) / (t/1000.0));
}

double getWriteSpeed() {
    void * bufForReadWrite = malloc(BUFFER_SIZE);
    double timeElapse = 0.0;
    struct timeval writeStartTime, writeEndTime;
    int fd; //用于存放open函数返回的文件描述符
    
    fd = open(filename, O_RDWR | O_CREAT, 0644);//创建并打开文件，可读可写，创建权限为644
    if(fd < 0){
        //创建文件失败时返回
        perror("Unable to create file");
        return -1;
    }
    gettimeofday(&writeStartTime, NULL);
    ssize_t sizeWrited = write(fd, bufForReadWrite, BUFFER_SIZE);
    fsync(fd);
    gettimeofday(&writeEndTime, NULL);
    printf("Write:%ld bytes to %s\n", sizeWrited, filename);
    close(fd); //写入完毕后关闭文件
    free(bufForReadWrite);
    timeElapse = getTimeElapse(writeStartTime, writeEndTime);
    
    return transSpeed(sizeWrited, timeElapse);
}

double getReadSpeed() {
    void *bufForReadWrite = malloc(BUFFER_SIZE);
    double timeElapse = 0.0;
    int fd; //用于存放open函数返回的文件描述符
    struct timeval readStartTime, readEndTime;
    fd = open(filename, O_RDWR); //打开文件后的权限为可读可写
    if(fd < 0){
        //打开文件失败时返回
        perror("Unable to open file");
        return -1;
    }
    gettimeofday(&readStartTime, NULL);
    ssize_t sizeReaded = read(fd, bufForReadWrite, BUFFER_SIZE);
    fsync(fd);
    gettimeofday(&readEndTime, NULL);
    close(fd); //读完毕后关闭文件
    free(bufForReadWrite);
    printf("Read:%ld bytes from %s\n", sizeReaded, filename);
    
    timeElapse = getTimeElapse(readStartTime, readEndTime);
    
    return transSpeed(sizeReaded, timeElapse);
}

void deleteTestFile() {
    // 删除测试文件
    if (access(filename, 0) == 0 && unlink(filename) != 0) {
        perror("unlink");
    }
}
