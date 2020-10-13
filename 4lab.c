#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_CANCEL_SUCCESS 0
#define WAIT_TIME 2


void *thread_func(void* arg) { 
        printf("In child thread now\n");
        int i = 0;
        while(1) {
            pthread_testcancel();
            printf("Still in the cycle: %d\n", i);
            i++;
            usleep(100000);
        }
        printf("Child was not canceled\n");
        pthread_exit(NULL);
        return NULL;
}

int main() {
        pthread_t thread;
    int status;
        status = pthread_create(&thread, NULL, thread_func, NULL);
        if (status != PTHREAD_CREATE_SUCCESS) {
            errno = status;
            perror("Error in creating second thread");
            return EXIT_FAILURE;
        }
        sleep(WAIT_TIME);

        status = pthread_cancel(thread);
        if (status != PTHREAD_CANCEL_SUCCESS) {
            errno = status;
            perror("Error in cancel");
            return EXIT_FAILURE;
        }
        printf("Back\n");
        pthread_exit(NULL);
}
