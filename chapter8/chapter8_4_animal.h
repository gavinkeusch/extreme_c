#ifndef CHAPTER_8_4_ANIMAL_H
#define CHAPTER_8_4_ANIMAL_H

struct animal_t;

struct animal_t* animal_new();
void animal_ctor(struct animal_t*);
void animal_dtor(struct animal_t*);

void animal_get_name(struct animal_t*, char*);
void animal_sound(struct animal_t*);

#endif