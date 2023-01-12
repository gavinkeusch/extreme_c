#define _GNU_SOURCE

#include <unistd.h>

#include <sys/syscall.h>

int main(int argc, char** argv) {
    char message[20] = "Hello World!\n";

    syscall(__NR_write, 1, message, 13);

    return 0;
}