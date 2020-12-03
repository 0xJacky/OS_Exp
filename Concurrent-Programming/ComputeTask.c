#include "ComputeTask.h"
#include <stdio.h>
#include <stdint.h>

#define LOOP_SIZE (1024*1024*1024)

void executeComputeTask() {
    size_t idx = 0;
    size_t loopSize = LOOP_SIZE;
    size_t sum = 0;
    for (idx = 0; idx < loopSize; ++idx) {
        sum += idx;
    }
}

