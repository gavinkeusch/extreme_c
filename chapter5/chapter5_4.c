#include <stdlib.h>

int main(int argc, char** argv) {
    char* ptr = (char*)malloc(16 * sizeof(char));
    free(ptr);
    return 0;
}