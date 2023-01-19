#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#define SH_SIZE 4

int shared_fd = -1;
int mutex_shm_fd = -1;

int32_t* counter = NULL;
pthread_mutex_t* mutex = NULL;

void init_control_mechanism() {
    mutex_shm_fd = shm_open("/mutex0", O_CREAT | O_RDWR, 0600);
    if (mutex_shm_fd < 0) {
        fprintf(stderr, "ERROR: Failed to create shared memory: %s\n", strerror(errno));
        exit(1);
    }

    if (ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0) {
        fprintf(stderr, "ERROR: Truncation of mutex failed: %s\n", strerror(errno));
        exit(1);
    }

    void* map = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        exit(1);
    }

    mutex = (pthread_mutex_t*)map;
    int ret = -1;
    pthread_mutexattr_t attr;

    if ((ret = pthread_mutexattr_init(&attr))) {
        fprintf(stderr, "ERROR: Failed to init mutex attrs: %s\n", strerror(errno));
        exit(1);
    }
    if ((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))) {
        fprintf(stderr, "ERROR: Failed to set the mutex attr: %s\n", strerror(errno));
        exit(1);
    }
    if ((ret = pthread_mutex_init(mutex, &attr))) {
        fprintf(stderr, "ERROR: Initializing the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if ((ret = pthread_mutexattr_destroy(&attr))) {
        fprintf(stderr, "ERROR: Failed to destroy mutex attrs: %s\n", strerror(errno));
        exit(1);
    }
}

void shutdown_control_mechanism() {
    int ret = -1;

    if ((ret = pthread_mutex_destroy(mutex))) {
        fprintf(stderr, "ERROR: Failed to destroy mutex: %s\n", strerror(errno));
        exit(1);
    }
    if (munmap(mutex, sizeof(pthread_mutex_t)) < 0) {
        fprintf(stderr, "ERROR: Unmapping the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if (close(mutex_shm_fd) < 0) {
        fprintf(stderr, "ERROR: Closing the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if (shm_unlink("/mutex0") < 0) {
        fprintf(stderr, "ERROR: Unlinking the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
}

void init_shared_resource() {
    shared_fd = shm_open("/shm0", O_CREAT | O_RDWR, 0600);
    if (shared_fd < 0) {
        fprintf(stderr, "ERROR: Failed to create shared memory: %s\n", strerror(errno));
        exit(1);
    }

    fprintf(stdout, "Shared memory is created with fd: %d\n", shared_fd);
}

void shutdown_shared_resource() {
    if (shm_unlink("/shm0") < 0) {
        fprintf(stderr, "ERROR: Unlinking shared memory failed: %s\n", strerror(errno));
        exit(1);
    }
}

void inc_counter() {
    usleep(1);
    pthread_mutex_lock(mutex);
    int32_t temp = *counter;
    usleep(1);
    temp++;
    usleep(1);
    *counter = temp;
    pthread_mutex_unlock(mutex);
    usleep(1);
}

int main(int argc, char** argv) {
    init_shared_resource();
    init_control_mechanism();

    if (ftruncate(shared_fd, SH_SIZE * sizeof(char)) < 0) {
        fprintf(stderr, "ERROR: Truncation failed: %s\n", strerror(errno));
        return 1;
    }
    fprintf(stdout, "The memory region was truncated.\n");

    void* map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        return 1;
    }

    counter = (int32_t*)map;
    *counter = 0;

    pid_t pid = fork();
    if (pid) {
        inc_counter();
        fprintf(stdout, "The parent process sees the counter as %d.\n", *counter);

        int status = -1;
        wait(&status);

        fprintf(stdout, "The child process finished with the state %d.\n", status);
    } else {
        inc_counter();
        fprintf(stdout, "The child process sees the counter as %d.\n", *counter);
    }

    if (munmap(counter, SH_SIZE) < 0) {
        fprintf(stderr, "ERROR: Unmapping failed: %s\n",
                strerror(errno));
        return 1;
    }
    if (close(shared_fd) < 0) {
        fprintf(stderr, "ERROR: Closing shared memory fd filed: %s\n",
                strerror(errno));
        return 1;
    }

    // only parent process needs to shut down the shared resource
    // and the employed control mechanism
    if (pid) {
        shutdown_shared_resource();
        shutdown_control_mechanism();
    }
    return 0;
}

