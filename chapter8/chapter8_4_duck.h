#ifndef CHAPTER_8_4_DUCK_H
#define CHAPTER_8_4_DUCK_H

struct duck_t;

struct duck_t* duck_new();
void duck_ctor(struct duck_t*);
void duck_dtor(struct duck_t*);

#endif