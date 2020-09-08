#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PTHREAD_CREATE_SUCCESS 0
#define LINES_COUNT 10

void print_ten_strings(const char* name){
        for (int i = 0; i < LINES_COUNT; i++){
                printf("%s : String %d\n", name, i);
        }
}


void* thread_func(void* args){
        print_ten_strings((char*)args);
        pthread_exit(NULL);
}


int main(){
        pthread_t thread;
        int status;
        static char sec_th[] = "Second Thread";
        status = pthread_create(&thread, NULL, thread_func, (void*)sec_th);
        if (status != PTHREAD_CREATE_SUCCESS){
                perror("Error in creating thread\n");
                return EXIT_FAILURE;
        }
        print_ten_strings("Main thread");
        pthread_exit(NULL);
}

