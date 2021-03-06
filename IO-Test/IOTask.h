//
//  IOTask.h
//  IO-Test
//
//  Created by Jacky on 2020/11/19.
//

#ifndef IOTask_h
#define IOTask_h

#define BUFFER_SIZE (1024*1024*1024)
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

double getTimeElapse(struct timeval, struct timeval);
double transSpeed(ssize_t, double);
double getWriteSpeed(void);
double getReadSpeed(void);
void deleteTestFile(void);

#endif /* IOTask_h */
