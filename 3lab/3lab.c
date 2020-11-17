#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define THREAD_CNT 4
#define THREAD_CREATE_SUCCESS 0
#define BUFFER_SIZE 4

void *thread_func(void *strings) {
    char ** lines = (char **) strings;

    for (int i = 0; i < BUFFER_SIZE; i++){
        if(lines[i] != NULL) {
            printf("%s\n", lines[i]);
        }
    }
    return NULL;
}


int main(){
    pthread_t threads[THREAD_CNT];
    int status;
    static char* strings[THREAD_CNT][BUFFER_SIZE] = {
        {"first1", "first2"},
        {"second1", "second2", "second3","second4"},
        {"third1", "third2", "third3"},
        {"forth1"}
    };

    for (int i = 0; i < THREAD_CNT; i++) {
        status = pthread_create(&threads[i], NULL, thread_func, strings[i]);
        if (status != THREAD_CREATE_SUCCESS) {
            fprintf(stderr, "Error in creating thread %s\n", strerror(status));
            return EXIT_FAILURE;
        }
    }

    pthread_exit(NULL);
}
