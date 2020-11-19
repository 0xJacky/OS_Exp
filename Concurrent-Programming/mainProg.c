#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

#include "ComputeTask.h"
#include "IOTask.h"

typedef enum OP {
    COMPUTE_TASK = 1,
    IO_TASK
} OP_t;

typedef struct task {
    struct task *next;
    OP_t taskType;
} task_t;

double getTimeElapse(struct timeval start, struct timeval end) {
    return (double) (end.tv_sec - start.tv_sec) * 1000.0 + (double) (end.tv_usec - start.tv_usec) / 1000.0;
}

int main(int argc, char *argv[]) {
    double computeTaskTimeElapse = 0.0;
    double IOTaskTimeElapse = 0.0;
    double totalTimeElapse = 0.0;

    struct timeval mainTaskStartTime, mainTaskEndTime, computeTaskStartTime, computeTaskEndTime, IOTaskStartTime, IOTaskEndTime;

    // 获取主进程执行的开始时间
    gettimeofday(&mainTaskStartTime, NULL);

    pid_t fpid;
    task_t computeTask, ioTask;
    task_t *curTask = &computeTask;

    // 进程链表
    computeTask.taskType = COMPUTE_TASK;
    computeTask.next = &ioTask;
    ioTask.taskType = IO_TASK;
    ioTask.next = NULL;

    // 父子进程区别标记
    int parentProcess = 1;
    // 子进程数量
    int childProcessNum = 0;

    while (NULL != curTask) {
        if (curTask->taskType == IO_TASK)
            // IO进程开始时间
            gettimeofday(&IOTaskStartTime, NULL);
        else
            // 计算进程开始时间
            gettimeofday(&computeTaskStartTime, NULL);
        // 创建子进程，一次调用两次返回父进程中返回所创建的子进程PID, 子进程中返回0
        fpid = fork();
        if (0 == fpid) {
            // 子进程中
            parentProcess = 0;
            break;
        } else if (-1 == fpid) {
            // 子进程创建失败
            printf("Generate child Process error!\n");
            exit(0);
        }

        printf("Generate child process with pid:%d\n", fpid);

        // 子进程数量自增长
        ++childProcessNum;
        curTask = curTask->next;

    }

    if (parentProcess == 0) {
        // IO进程执行
        if (curTask->taskType == IO_TASK) {
            executeIOTask();
            printf("This is a IO task, pid:%d parent pid:%d\n", getpid(),
                   getppid());//Print process ID and parent process ID
            // 返回状态
            exit(1);
        }
        // 计算进程执行
        if (curTask->taskType == COMPUTE_TASK) {
            executeComputeTask();
            printf("This is a compute task, pid:%d parent pid:%d\n", getpid(),
                   getppid());//Print process ID and parent process ID
            // 返回状态
            exit(2);
        }
    } else {
        int p;
        for (int i = 0; i < childProcessNum; i++) {
            wait(&p);
            // 接收退出状态区分进程的结束时间
            p = WEXITSTATUS(p);
            //printf("%d\n", p);
            if (p == 1) {
                // IO 进程结束时间
                gettimeofday(&IOTaskEndTime, NULL);
            } else {
                // 计算进程结束时间
                gettimeofday(&computeTaskEndTime, NULL);
            }
        }
        // 主进程结束时间
        gettimeofday(&mainTaskEndTime, NULL);
        //Parent Process, we calculate the time for executing computing task and the time fo executing IO task
        computeTaskTimeElapse = getTimeElapse(computeTaskStartTime, computeTaskEndTime);
        IOTaskTimeElapse = getTimeElapse(IOTaskStartTime, IOTaskEndTime);
        totalTimeElapse = getTimeElapse(mainTaskStartTime, mainTaskEndTime);
        printf("Compute Task Time Consume:%fms, IO Task Time Consume: %fms, Total Time Consume:%fms \n",
               computeTaskTimeElapse, IOTaskTimeElapse, totalTimeElapse);
    }
    return 0;
}
