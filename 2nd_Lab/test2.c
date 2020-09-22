#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_JOIN_SUCCESS 0
#define LINES_COUNT 10


void print_ten_strings(const char* name){
        for (int i = 0; i < LINES_COUNT; i++){
                printf("%s : String %d\n", name, i);
        }
        return;
}

void* thread_func(void* args){
        print_ten_strings((char*)args);
        sleep(30);
        pthread_exit(NULL);
}

void* third_thread_func(void* thread){
        int status;

        status = pthread_join((pthread_t)thread, NULL);
        if (status != PTHREAD_JOIN_SUCCESS){
		errno = status;
                perror("Error in joining second thread to third thread");
                exit(EXIT_FAILURE);
        }
        pthread_exit(NULL);
}

int main(){
        pthread_t thread, thread1;
        int status;
        static char sec_th[] = "Second Thread";
        static char third_th[] = "Third Thread";

        status = pthread_create(&thread, NULL, thread_func, (void*)sec_th);
        if (status != PTHREAD_CREATE_SUCCESS){
		errno = status;
                perror("Error in creating second thread");
                return EXIT_FAILURE;
        }

        status = pthread_create(&thread1, NULL, third_thread_func,(void*)thread);
        if (status != PTHREAD_CREATE_SUCCESS){
		errno = status;
                perror("Error in creating third thread");
                return EXIT_FAILURE;
        }

        status = pthread_join(thread, NULL);
        if (status != PTHREAD_JOIN_SUCCESS){
		errno = status;
                perror("Error in joining second thread to main thread");
                return EXIT_FAILURE;
        }

        print_ten_strings("Main thread");
        pthread_exit(NULL);
}
