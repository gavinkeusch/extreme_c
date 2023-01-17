#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void* thread_body(void* arg) {
    printf("Hello from first thread!\n");
    return NULL;
}

int main(int argc, char** argv) {
    // thread handler
    pthread_t thread;

    // create a new thread
    int result = pthread_create(&thread, NULL, thread_body, NULL);
    if (result) {
        printf("Thread could not be created. Error number: %d\n", result);
        exit(1);
    }

    // wait for created thread to finish
//    result = pthread_join(thread, NULL);
//    if (result) {
//        printf("The thread could not be joined. Error number: %d\n", result);
//        exit(2);
//    }

    // detach the thread
    result = pthread_detach(thread);
    if (result) {
        printf("Thread could not be detached. Error number: %d\n", result);
        exit(2);
    }

    // exit the main thread;
    pthread_exit(NULL);

    return 0;
}