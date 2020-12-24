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
#include "mainProg.h"

#define  FRAME_SIZE (64*1024)
#define  STRING_SIZE 100

int bss_var;
int global_data=1;

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

int main(int argc, char **argv)
{
    printf("Process ID:%d\n",getpid());

    printf("Below are address of types of process's mem\n");

    char fileName[STRING_SIZE];
    TextLocation(main);

    StackLocation(2,3);

    DataSegmentLocation();
    
    printf("Please check /proc/PID/maps after Free heap to check the change,press Enter to continue!\n");
    getchar();
    print_format();

    printf("File Map Location!\n");
        //printf("\tPlease input the file name used for file map:");
    //scanf("%s",fileName);
    file_map();
    print_format();
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
    getchar();
    strcpy(start_addr,modify_info);
    munmap(start_addr,sb.st_size);
    close(fd);
}


void recursion_fun(int n)
{
    int stack_frame[FRAME_SIZE];
    printf("\tLevel %d: n location %p\n",n,&n);
    getchar();
    if(n<3)
        recursion_fun(n+1);
    printf("\tLevel %d: n location %p\n",n,&n);
    getchar();
    return;
}
