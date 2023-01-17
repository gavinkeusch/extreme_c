#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

void* thread_body(void* arg) {
    char* str = (char*)arg;
    printf("%s\n", str);
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    // ex15_2_1
//    int result1 = pthread_create(&thread1, NULL, thread_body, "Apple");
//    int result2 = pthread_create(&thread2, NULL, thread_body, "Orange");
//    int result3 = pthread_create(&thread3, NULL, thread_body, "Lemon");

    // ex15_2_2
    char str1[8], str2[8], str3[8];
    strcpy(str1, "Apple");
    strcpy(str2, "Orange");
    strcpy(str3, "Lemon");

    int result1 = pthread_create(&thread1, NULL, thread_body, str1);
    int result2 = pthread_create(&thread2, NULL, thread_body, str2);
    int result3 = pthread_create(&thread3, NULL, thread_body, str3);

    if (result1 || result2 || result3) {
        printf("The threads could not be created.\n");
        exit(1);
    }

    // ex15_2_1
//    result1 = pthread_join(thread1, NULL);
//    result2 = pthread_join(thread2, NULL);
//    result3 = pthread_join(thread3, NULL);

    result1 = pthread_detach(thread1);
    result2 = pthread_detach(thread2);
    result3 = pthread_detach(thread3);

    if (result1 || result2 || result3) {
        // ex15_2_1
//        printf("The threads could not be joined\n".);
        printf("The threads could not be detached.\n");
        exit(2);
    }

    // now the strings become deallocated
    pthread_exit(NULL);

    return 0;
}