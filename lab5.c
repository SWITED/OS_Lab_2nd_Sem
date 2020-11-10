#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_CANCEL_SUCCESS 0
#define PTHREAD_JOIN_SUCCESS 0
#define WAIT_TIME 2
#define NO_HANDLER 0
#define handle_error_status(status, msg) \
    do { errno = status ; perror(msg); exit(EXIT_FAILURE); } while (0)



void *handler(void *arg){
	printf("Thread cancelled\n");
}


void *thread_func(void* arg) {
	pthread_cleanup_push(&handler, NULL);
        printf("In child thread now\n");
        int i = 0;
        while(1) {
 	    pthread_testcancel();
            printf("Still in the cycle: %d\n", i);
            i++;
        }
        pthread_exit(NULL);
}

int main() {
        pthread_t thread;
        int status;
        void *res;
        status = pthread_create(&thread, NULL, thread_func, NULL);
        if (status != PTHREAD_CREATE_SUCCESS){
           handle_error_status(status, "Error in creating second thread");
        }
        sleep(WAIT_TIME);
        printf("main(): sending cancellations request\n");
        status = pthread_cancel(thread);
        if (status != PTHREAD_CANCEL_SUCCESS) {
            handle_error_status(status, "Error in canceling thread");
        }
        pthread_exit(0);
}
