#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "shm_com.h"
int main(){
    int shmid;
    srand((unsigned int)getpid());
    shmid = shmget((key_t)1234,sizeof(struct shared_use_st),0666| IPC_CREAT);
    if(shmid == -1){
        fprintf(stderr, "shmget failed.\n");
            exit(EXIT_FAILURE);
    }
    void *shared_memory = (void*)0;
    shared_memory = shmat(shmid,(void *)0,0);
    if(shared_memory==(void *)-1){
        fprintf(stderr, "shmat failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n",(long)shared_memory);
    struct shared_use_st *shared_stuff;
    shared_stuff = (struct shared_use_st*)shared_memory;
    //以上是映射共享内存，与consumer一样
    int running = 1;
    char buffer[BUFSIZ];
    while(running){
        while(shared_stuff->written==1){//消费者还没读完，等待
            sleep(1);
            printf("waiting for client...\n" );
        }     
        printf("enter some text:\n" );
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared_stuff->text, buffer, TEXT_SZ);//写入数据
        shared_stuff->written=1;
        if(strncpy(buffer, "end",3)==0){
            running=0;
        }
    }
    if(shmdt(shared_memory)==-1){
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
