#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

pthread_barrier_t barrier;

void* thread_body_1(void* arg) {
   printf("A\n");

   // wait for the other thread to join
    pthread_barrier_wait(&barrier);

    return NULL;
}

void* thread_body_2(void* arg) {
   // wait for the other thread to join
    pthread_barrier_wait(&barrier);
    printf("B\n");

    return NULL;
}

int main(int argc, char** argv) {
    pthread_barrier_init(&barrier, NULL, 2);

    pthread_t thread1;
    pthread_t thread2;

    int result1 = pthread_create(&thread1, NULL, thread_body_1, NULL);
    int result2 = pthread_create(&thread2, NULL, thread_body_2, NULL);

    if (result1 || result2) {
        printf("The threads could not be created.\n");
        exit(1);
    }

    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);

    if (result1 || result2) {
        printf("The threads could not be joined.\n");
        exit(2);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}