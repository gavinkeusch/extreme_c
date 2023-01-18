#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SH_SIZE 4

int shared_fd = -1;
int32_t* counter = NULL;

void init_shared_resource() {
    shared_fd = open("data.bin", O_CREAT | O_RDWR, 0600);
    if (shared_fd < 0) {
        fprintf(stderr, "ERROR: Failed to create file: %s\n", strerror(errno));
        exit(1);
    }

    fprintf(stdout, "File is created and opened with fd: %d\n", shared_fd);
}

void shutdown_shared_resource() {
    if (remove("data.bin") < 0) {
        fprintf(stderr, "ERROR: Removing the file failed: %s\n", strerror(errno));
        exit(1);
    }
}

void inc_counter() {
    usleep(1);
    int32_t temp = *counter;
    usleep(1);
    temp++;
    usleep(1);
    *counter = temp;
    usleep(1);
}

int main(int argc, char** argv) {
    init_shared_resource();

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

    // only parent process needs to shutdown the shared resource
    if (pid) {
        shutdown_shared_resource();
    }
    return 0;
}

