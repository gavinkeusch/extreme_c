#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define TRUE 1
#define FALSE 0

typedef unsigned int bool_t;

// a struct for keeping all variables related to the shared state
typedef struct {
    bool_t done;
    pthread_mutex_t mtx;
    pthread_cond_t cv;
} shared_state_t;

// init the members of the shared state
void shared_state_init(shared_state_t* shared_state) {
    shared_state->done = FALSE;
    pthread_mutex_init(&shared_state->mtx, NULL);
    pthread_cond_init(&shared_state->cv, NULL);
}

// destroy the members of the shared state
void shared_state_destroy(shared_state_t* shared_state) {
    pthread_mutex_destroy(&shared_state->mtx);
    pthread_cond_destroy(&shared_state->cv);
}

void* thread_body_1(void* arg) {
    shared_state_t* ss = (shared_state_t*)arg;
    pthread_mutex_lock(&ss->mtx);
    printf("A\n");
    ss->done = TRUE;

    // signal thread waiting on conditional variable
    pthread_cond_signal(&ss->cv);
    pthread_mutex_unlock(&ss->mtx);
    return NULL;
}

void* thread_body_2(void* arg) {
    shared_state_t* ss = (shared_state_t*)arg;
    pthread_mutex_lock(&ss->mtx);

    while (!ss->done) {
        // wait on conditional variable
        pthread_cond_wait(&ss->cv, &ss->mtx);
    }

    printf("B\n");
    pthread_mutex_unlock(&ss->mtx);
    return NULL;
}

int main(int argc, char** argv) {
    shared_state_t shared_state;
    shared_state_init(&shared_state);

    pthread_t thread1;
    pthread_t thread2;

    int result1 = pthread_create(&thread1, NULL, thread_body_1, &shared_state);
    int result2 = pthread_create(&thread2, NULL, thread_body_2, &shared_state);

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

    // destroy the shared state and release the mutex and conditional variable
    shared_state_destroy(&shared_state);

    return 0;
}