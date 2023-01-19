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

typedef uint16_t bool_t;

#define TRUE 1
#define FALSE 0

#define MUTEX_SHM_NAME "/mutex0"
#define SHM_NAME "/shm0"

int cancel_flag_shm_fd = -1;
bool_t cancel_flag_shm_owner = FALSE;
int mutex_shm_fd = -1;
pthread_mutex_t* mutex = NULL;
bool_t mutex_owner = FALSE;
bool_t* cancel_flag = NULL;

void init_shared_resource() {
    cancel_flag_shm_fd = shm_open(SHM_NAME, O_RDWR, 0600);
    if (cancel_flag_shm_fd >= 0) {
        cancel_flag_shm_owner = FALSE;
        fprintf(stdout, "The shared memory object is opened.\n");
    } else if (errno == ENOENT) {
        fprintf(stderr, "WARN: The shared memory object doesn't exist.\n");
        fprintf(stdout, "Creating the shared memory object...\n");

        cancel_flag_shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (cancel_flag_shm_fd >= 0) {
            cancel_flag_shm_owner = TRUE;
            fprintf(stdout, "The shared memory object is created.\n");
        } else {
            fprintf(stderr, "ERROR: Failed to create shared memory: %s\n", strerror(errno));
            exit(1);
        }
    } else {
        fprintf(stderr, "ERROR: Failed to create shared memory: %s\n", strerror(errno));
        exit(1);
    }

    if (cancel_flag_shm_owner) {
        if (ftruncate(cancel_flag_shm_fd, sizeof(bool_t)) < 0) {
            fprintf(stderr, "ERROR: Truncation failed: %s\n", strerror(errno));
            exit(1);
        }
        fprintf(stdout, "The memory region is truncated.\n");
    }

    void* map = mmap(0, sizeof(bool_t), PROT_WRITE, MAP_SHARED, cancel_flag_shm_fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        exit(1);
    }

    cancel_flag = (bool_t*)map;
    if (cancel_flag_shm_owner)
        *cancel_flag = FALSE;
}

void shutdown_shared_resource() {
    if (munmap(cancel_flag, sizeof(bool_t)) < 0) {
        fprintf(stderr, "ERROR: Unmapping failed: %s\n", strerror(errno));
        exit(1);
    }
    if (close(cancel_flag_shm_fd) < 0) {
        fprintf(stderr, "ERROR: Closing the shared memory failed: %s\n", strerror(errno));
        exit(1);
    }
    if (cancel_flag_shm_owner) {
        sleep(1);
        if (shm_unlink(SHM_NAME) < 0) {
            fprintf(stderr, "ERROR: Unlinking the shared memory failed: %s\n", strerror(errno));
            exit(1);
        }
    }
}

void init_control_mechanism() {
    mutex_shm_fd = shm_open(MUTEX_SHM_NAME, O_RDWR, 0600);
    if (mutex_shm_fd >= 0) {
        mutex_owner = FALSE;
        fprintf(stdout, "The mutex's shared memory object is opened.\n");
    } else if (errno == ENOENT) {
        fprintf(stderr, "WARN: Mutex's shared memory doesn't exist.\n");
        fprintf(stdout, "Creating the mutex's shared memory object...\n");

        mutex_shm_fd = shm_open(MUTEX_SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
        if (mutex_shm_fd >= 0) {
            mutex_owner = TRUE;
            fprintf(stdout, "The mutex's shared memory object is created.\n");
        } else {
            fprintf(stderr, "ERROR: Failed to create mutex's shared memory: %s\n", strerror(errno));
            exit(1);
        }
    } else {
        fprintf(stderr, "ERROR: Failed to create mutex's shared memory: %s\n", strerror(errno));
        exit(1);
    }

    if (mutex_owner) {
        if (ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0) {
            fprintf(stderr, "ERROR: Truncation of mutex failed: %s\n", strerror(errno));
            exit(1);
        }
    }

    void* map = mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        exit(1);
    }

    mutex = (pthread_mutex_t*)map;

    if (mutex_owner) {
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
}

void shutdown_control_mechanism() {
    sleep(1);
    if (mutex_owner) {
        int ret = -1;
        if ((ret = pthread_mutex_destroy(mutex))) {
            fprintf(stderr,
                    "WARN: Destruction of the mutex failed: %s\n", strerror(ret));
        }
    }
    if (munmap(mutex, sizeof(pthread_mutex_t)) < 0) {
        fprintf(stderr, "ERROR: Unmapping the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if (close(mutex_shm_fd) < 0) {
        fprintf(stderr, "ERROR: Closing the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if (mutex_owner) {
        if (shm_unlink(MUTEX_SHM_NAME) < 0) {
            fprintf(stderr, "ERROR: Unlinking the mutex failed: %s\n", strerror(errno));
            exit(1);
        }
    }
}

bool_t is_canceled() {
    pthread_mutex_lock(mutex);
    bool_t temp = *cancel_flag;
    pthread_mutex_unlock(mutex);

    return temp;
}

void cancel() {
    pthread_mutex_lock(mutex);
    *cancel_flag = TRUE;
    pthread_mutex_unlock(mutex);
}

void sigint_handler(int signo) {
    fprintf(stdout, "\nHandling INT signal: %d...\n", signo);
    cancel();
}

int main(int argc, char** argv) {
    signal(SIGINT, sigint_handler);

    init_shared_resource();
    init_control_mechanism();

    while (!is_canceled()) {
        fprintf(stdout, "Working...\n");
        sleep(1);
    }

    fprintf(stdout, "Cancel signal is received.\n");

    shutdown_shared_resource();
    shutdown_control_mechanism();

    return 0;
}