#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

sem_t* semaphore;

void* thread_body_1(void* arg) {
    int* shared_var_ptr = (int*)arg;

    sem_wait(semaphore);
    (*shared_var_ptr)++;
    printf("%d\n", *shared_var_ptr);
    sem_post(semaphore);

    return NULL;
}

void* thread_body_2(void* arg) {
    int* shared_var_ptr = (int*)arg;

    sem_wait(semaphore);
    *shared_var_ptr += 2;
    printf("%d\n", *shared_var_ptr);
    sem_post(semaphore);

    return NULL;
}

int main(int argc, char** argv) {
    int shared_var = 0;

    pthread_t thread1;
    pthread_t thread2;

#ifdef __APPLE__
    semaphore = sem_open("sem0", O_CREAT | O_EXCL, 0644, 1);
#else
    sem_t local_semaphore;
    semaphore = &local_semaphore;
    sem_init(semaphore, 0, 1);
#endif

    int result1 = pthread_create(&thread1, NULL, thread_body_1, &shared_var);
    int result2 = pthread_create(&thread2, NULL, thread_body_2, &shared_var);

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

#ifdef __APPLE__
    sem_close(semaphore);
#else
    sem_destroy(semaphore);
#endif

    return 0;
}