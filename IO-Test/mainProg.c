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
    for (int i = 0; i < N; i++) {
        printf("Test %d\n", i + 1);
        double _ws = getWriteSpeed();
        printf("Write Speed: %fMB/s\n", _ws);
        double _rs = getReadSpeed();
        printf("Read Speed: %fMB/s\n", _rs);
        writeSpeed += _ws;
        readSpeed += _rs;
    }
    writeSpeed /= N;
    readSpeed /= N;
    deleteTestFile();
    printf("Average Write Speed: %fMB/s, Average Read Speed: %fMB/s\n", writeSpeed, readSpeed);
    return 0;
}
