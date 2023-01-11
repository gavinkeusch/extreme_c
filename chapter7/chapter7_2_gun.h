#ifndef CHAPTER_7_2_GUN_H
#define CHAPTER_7_2_GUN_H

typedef int bool_t;

struct gun_t;

struct gun_t* gun_new();
void gun_ctor(struct gun_t*, int);
void gun_dtor(struct gun_t*);

bool_t gun_has_bullets(struct gun_t*);
void gun_trigger(struct gun_t*);
void gun_refill(struct gun_t*);

#endif