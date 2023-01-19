#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "chapter18_4_shared_mem.h"

typedef struct {
    struct shared_mem_t* shm;
    pthread_mutex_t* ptr;
} shared_mutex_t;

shared_mutex_t* shared_mutex_new() {
    shared_mutex_t* obj = (shared_mutex_t*)malloc(sizeof(shared_mutex_t));
    obj->shm = shared_mem_new();

    return obj;
}

void shared_mutex_delete(shared_mutex_t* obj) {
    shared_mem_delete(obj->shm);
    free(obj);
}

void shared_mutex_ctor(shared_mutex_t* obj, const char* name) {
    shared_mem_ctor(obj->shm, name, sizeof(pthread_mutex_t));
    obj->ptr = (pthread_mutex_t*)shared_mem_getptr(obj->shm);

    if (shared_mem_isowner(obj->shm)) {
        pthread_mutexattr_t mutex_attr;
        int ret = -1;
        if ((ret = pthread_mutexattr_init(&mutex_attr))) {
            fprintf(stderr, "ERROR(%s): Initializing mutex attrs failed: %s\n", name, strerror(ret));
            exit(1);
        }
        if ((ret = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED))) {
            fprintf(stderr, "ERROR(%s): Failed to set as process-shared: %s\n", name, strerror(ret));
            exit(1);
        }
        if ((ret = pthread_mutex_init(obj->ptr, &mutex_attr))) {
            fprintf(stderr, "ERROR(%s): Initializing the mutex failed: %s\n", name, strerror(ret));
            exit(1);
        }
        if ((ret = pthread_mutexattr_destroy(&mutex_attr))) {
            fprintf(stderr, "ERROR(%s): Destruction of mutex attrs failed: %s\n", name, strerror(ret));
            exit(1);
        }
    }
}

void shared_mutex_dtor(shared_mutex_t* obj) {
    if (shared_mem_isowner(obj->shm)) {
        int ret = -1;
        if ((ret = pthread_mutex_destroy(obj->ptr))) {
            fprintf(stderr, "WARN: Destruction of the mutex failed: %s\n", strerror(ret));
        }
    }

    shared_mem_dtor(obj->shm);
}

pthread_mutex_t* shared_mutex_getptr(shared_mutex_t* obj) {
    return obj->ptr;
}

void shared_mutex_lock(shared_mutex_t* obj) {
    int ret = -1;
    if ((ret = pthread_mutex_lock(obj->ptr))) {
        fprintf(stderr, "ERROR: Locking the mutex failed: %s\n",
                strerror(ret));
        exit(1);
    }
}
void shared_mutex_unlock(shared_mutex_t* obj) {
    int ret = -1;
    if ((ret = pthread_mutex_unlock(obj->ptr))) {
        fprintf(stderr, "ERROR: Unlocking the mutex failed: %s\n", strerror(ret));
        exit(1);
    }
}