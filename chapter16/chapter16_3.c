#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <pthread.h>

void* thread_body_1(void* arg) {
    int local_var = 0;
    printf("Thread 1 > Stack Address: %p\n", (void*)&local_var);
    return 0;
}

void* thread_body_2(void* arg) {
    int local_var = 0;
    printf("Thread 2 > Stack Address: %p\n", (void*)&local_var);
    return 0;
}

int main(int argc, char** argv) {
    size_t buffer_len = PTHREAD_STACK_MIN + 100;
    // the buffer allocated from the heap to be used as the thread's stack region
    char* buffer = (char*)malloc(buffer_len * sizeof(char));

    pthread_t thread1;
    pthread_t thread2;

    int result1 = pthread_create(&thread1, NULL, thread_body_1, NULL);

    // create a new thread with a custom stack region
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // set the stack address and size
    if (pthread_attr_setstack(&attr, buffer, buffer_len)) {
        printf("Failed while setting the stack attributes.\n");
        exit(1);
    }

    int result2 = pthread_create(&thread2, &attr, thread_body_2, NULL);

    if (result1 || result2) {
        printf("The threads could not be created.\n");
        exit(1);
    }

    printf("Main Thread > Heap Address: %p\n", (void*)buffer);
    printf("Main Thread > Stack Address: %p\n", (void*)&buffer_len);

    // wait for the threads to finish
    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);

    if (result1 || result2) {
        printf("The threads could not be joined.\n");
        exit(2);
    }

    free(buffer);

    return 0;
}