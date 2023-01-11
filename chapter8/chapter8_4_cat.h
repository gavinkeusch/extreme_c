#ifndef CHAPTER_8_4_CAT_H
#define CHAPTER_8_4_CAT_H

struct cat_t;

struct cat_t* cat_new();
void cat_ctor(struct cat_t*);
void cat_dtor(struct cat_t*);

#endif