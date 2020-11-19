//
//  mainProg.c
//  IO-Test
//
//  Created by Jacky on 2020/11/19.
//

#define N 10
#include <stdio.h>

#include "IOTask.h"

int main(int argc, char *argv[]){
    double writeSpeed = 0;
    double readSpeed = 0;
    for (int i = 0; i< N; i++) {
        writeSpeed += getWriteSpeed();
        readSpeed += getReadSpeed();
    }
    writeSpeed /= N;
    readSpeed /= N;
    deleteTestFile();
    printf("Write Speed: %fMB/s, Read Speed: %fMB/s\n", writeSpeed, readSpeed);
    return 0;
}
