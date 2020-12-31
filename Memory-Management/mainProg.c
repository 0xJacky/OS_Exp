//
//  mainProg.c
//  Memory-Management
//
//  Created by Jacky on 2020/12/3.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "mainProg.h"

#define  FRAME_SIZE (64*1024)
#define  STRING_SIZE 100

int bss_var;
int global_data=1;

void OSCopyFile(char *source, char *dest)
{
    int childExitStatus;
        pid_t pid;
        int status;
        if (!source || !dest) {
            /* handle as you wish */
        }

        pid = fork();

        if (pid == 0) { /* child */
            execl("/bin/cp", "/bin/cp", "-f", source, dest, (char *)0);
        }
        else if (pid < 0) {
            /* error - couldn't start process - you decide how to handle */
        }
        else {
            /* parent - wait for child - this has all error handling, you
             * could just call wait() as long as you are only expecting to
             * have one child process at a time.
             */
            pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
            if (ws == -1)
            { /* error - handle as you wish */
            }

            if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
            {
                status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
                /* handle non-zero as you wish */
            }
            else if (WIFSIGNALED(childExitStatus)) /* killed */
            {
            }
            else if (WIFSTOPPED(childExitStatus)) /* stopped */
            {
            }
        }
}

void StackLocation(int stack_var0, int stack_var1)
{
    print_format();
    
    printf("Stack Location\n");
    printf("\tThe address of Stack_var0 : %p\n",&stack_var0);
    printf("\tThe address of Stack_var1:%p\n",&stack_var1);
    print_format();
    
}

void DataSegmentLocation()
{
    print_format();
    printf("BSS Location:\n");
    printf("\tAddress of bss_var:%p\n",&bss_var);
    printf("Data Location:\n");
    printf("\tAddress of global data(Data Segment):%p\n",&global_data);
    static int static_data=4;
    printf("\tNew end of static data(Data Segment):%p\n",&static_data);
    print_format();
    
    
    
}

void TextLocation(int (*ptr)(int ,char**))
{
    print_format();
    printf("Code Location:\n");
    printf("\tAddress of main    (Code Segment):%p \n",ptr);
    print_format();
    
}

void HeapLocation()
{
    printf("Heap Location:\n");
    char *heap_var2=(char*)malloc(1024*1024);
    printf("\tAddress of heap_var2 is %p\n",heap_var2);
    printf("Please check /proc/PID/maps file to check the change,press Enter to continue!\n");
    getchar();
    free(heap_var2);
    
}

char path[100];
int pid;

int main(int argc, char **argv)
{
    pid = getpid();
    printf("Process ID:%d\n",getpid());
    
    printf("Below are address of types of process's mem\n");
    
    char fileName[STRING_SIZE];
    TextLocation(main);
    
    StackLocation(2,3);
    
    DataSegmentLocation();
    sprintf(path, "/proc/%d/maps", getpid());
    OSCopyFile( path, "maps/original" );
    sprintf(path, "/proc/%d/smaps", getpid());
    OSCopyFile( path, "smaps/original" );
    printf("Please check /proc/PID/maps after Free heap to check the change,press Enter to continue!\n");
    getchar();
    print_format();
    
    printf("File Map Location!\n");
    //printf("\tPlease input the file name used for file map:");
    //scanf("%s",fileName);
    file_map();
    print_format();
    
    OSCopyFile( path, "maps/FileMap1" );
    OSCopyFile( path, "smaps/FileMap1" );
    getchar(); //used to check maps， smaps
    
    printf("Recursion Call Location:\n");
    recursion_fun(1);
    
}



void print_format()
{
    printf("====================================================\n");
}

void file_map()
{
    int fd;
    void *start_addr;
    struct stat sb;
    char modify_info[STRING_SIZE]="Modify map-file content!";
    char fileName[]="map-file.txt";
    fd=open(fileName,O_RDWR|O_CREAT,0644);
    write(fd,modify_info,STRING_SIZE);
    
    
    fstat(fd,&sb);
    
    start_addr = mmap(NULL,sb.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(start_addr==MAP_FAILED)
    {
        printf("mmap file failed\n");
        return;
    }
    
    printf("\tMapped area stared by addr:%p:\n\tThe original info of file are as below:",start_addr);
    fputs((char*)start_addr,stdout);
    printf("\tPlease check /proc/PID/maps file to check the map \n");
    sprintf(path, "/proc/%d/maps", pid);
    OSCopyFile( path, "maps/FileMap" );
    sprintf(path, "/proc/%d/smaps", pid);
    OSCopyFile( path, "smaps/FileMap" );
    getchar();
    strcpy(start_addr,modify_info);
    munmap(start_addr,sb.st_size);
    close(fd);
}

void recursion_copy(int n, char * p) {
    sprintf(path, "/proc/%d/maps", getpid());
    char fileName[100];
    sprintf(fileName, "maps/%s%d", p, n);
    OSCopyFile( path, fileName );
    
    sprintf(path, "/proc/%d/smaps", getpid());
    sprintf(fileName, "smaps/%s%d", p, n);
    OSCopyFile( path, fileName );
}


void recursion_fun(int n)
{
    int stack_frame[FRAME_SIZE];
    recursion_copy(n, "Recursion");
    printf("\tLevel %d: n location %p\n",n,&n);
    getchar();
    if(n<3)
        recursion_fun(n+1);
    printf("\tLevel %d: n location %p\n",n,&n);
    recursion_copy(n, "R");
    getchar();
    return;
}
