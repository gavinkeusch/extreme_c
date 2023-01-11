#ifndef CHAPTER_6_3_H
#define CHAPTER_6_3_H

#include <unistd.h>

struct list_t;

struct list_t* list_malloc();

void list_init(struct list_t*);
void list_destroy(struct list_t*);

int list_add(struct list_t*, int);
int list_get(struct list_t*, int, int*);
void list_clear(struct list_t*);
size_t list_size(struct list_t*);
void list_print(struct list_t*);

#endif