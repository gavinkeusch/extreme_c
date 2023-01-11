#ifndef CHAPTER_8_4_ANIMAL_P_H
#define CHAPTER_8_4_ANIMAL_P_H

typedef void (*sound_func_t) (void*);

typedef struct {
    char* name;
    sound_func_t sound_func;
} animal_t;

#endif