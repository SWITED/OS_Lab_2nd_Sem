#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define PTHREAD_CREATE_SUCCESS 0
#define PTHREAD_JOIN_SUCCESS 0
#define SEM_DESTROY_SUCCESS 0
#define SEM_INIT_SUCCESS 0
#define SEM_WAIT_SUCCESS 0
#define SEM_POST_SUCCESS 0
#define SEM_1_VALUE 1
#define SEM_2_VALUE 0
#define MUST_BE_ZERO 0
#define LINES_CNT 10

#define handle_error_status(status, msg) \
    do { errno = status ; perror(msg); exit(EXIT_FAILURE); } while (0)


sem_t sem1,sem2;

void my_sem_init() {
    int status = sem_init(&sem1, MUST_BE_ZERO, SEM_1_VALUE);
    if (status != SEM_INIT_SUCCESS) {
        handle_error_status(status, "Error in initialising sem1.");
    }
    status = sem_init(&sem2, MUST_BE_ZERO, SEM_2_VALUE);
    if (status != SEM_INIT_SUCCESS) {
        handle_error_status(status, "Error in initialising sem2.");
    }
}

void my_sem_destroy() {
    int status = sem_destroy(&sem1);
    if (status != SEM_DESTROY_SUCCESS) {
        handle_error_status(status, "Error in destroying sem1.");
    }
    status = sem_destroy(&sem2);
    if (status != SEM_DESTROY_SUCCESS) {
        handle_error_status(status, "Error in destroying sem2.");
    }
}


void* thread_func() {
    for (int i = 0; i < LINES_CNT; i++) {
        int status = sem_wait(&sem2);
        if (status != SEM_WAIT_SUCCESS) {
            handle_error_status(status,"It\'s impossible to lock sem2");
        }

        printf("child  %d string\n", i+1);

        status = sem_post(&sem1);
        if (status != SEM_POST_SUCCESS) {
            handle_error_status(status,"It\'s impossible to unlock sem1");
        }
    }

    return NULL;
}

int main() {
    pthread_t thread;
    my_sem_init();

    int status = pthread_create(&thread, NULL, thread_func, NULL);
    if (status != PTHREAD_CREATE_SUCCESS) {
        handle_error_status(status, "Error in creating second thread");
    }

    for (int i = 0; i < LINES_CNT; i++) {
        status = sem_wait(&sem1);
        if (status != SEM_WAIT_SUCCESS) {
            handle_error_status(status,"It\'s impossible to lock sem1");
        }

        printf("parent %d string\n", i+1);

        status = sem_post(&sem2);
        if (status != SEM_POST_SUCCESS) {
            handle_error_status(status,"It\'s impossible to unlock sem2");
        }
    }

    status = pthread_join(thread, NULL);
    if (status != PTHREAD_JOIN_SUCCESS) {
        handle_error_status(status, "Error in joining second thread to main thread");
    }

    my_sem_destroy();
    pthread_exit(NULL);
}
