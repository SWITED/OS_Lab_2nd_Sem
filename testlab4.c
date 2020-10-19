#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_CANCEL_SUCCESS 0
#define PTHREAD_JOIN_SUCCESS 0
#define WAIT_TIME 2
#define handle_error_status(status, msg) \
    do { errno = status ; perror(msg); exit(EXIT_FAILURE); } while (0)



void *thread_func(void* arg) {
        printf("In child thread now\n");
        int i = 0;
        while(1) {
	    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
 	    pthread_testcancel();
	    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
            printf("Still in the cycle: %d\n", i);
            i++;
            usleep(100000);
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
        if (status != PTHREAD_CANCEL_SUCCESS){
            handle_error_status(status, "Error in canceling thread");
        }
        
        status = pthread_join(thread, &res);
        if (status != PTHREAD_JOIN_SUCCESS){
            handle_error_status(status, "Error in joining canceled thread");
        }
        
        if (res == PTHREAD_CANCELED){
            printf("main(): thread was canceled\n");
        } else{
            printf("main(): thread wasn't canceled (shouldn't happen!)\n");
        }
        exit(EXIT_SUCCESS);
}
