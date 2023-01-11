#ifndef CHAPTER_7_1_ENGINE_H
#define CHAPTER_7_1_ENGINE_H

struct engine_t;

struct engine_t* engine_new();
void engine_ctor(struct engine_t*);
void engine_dtor(struct engine_t*);

void engine_turn_on(struct engine_t*);
void engine_turn_off(struct engine_t*);
double engine_get_temperature(struct engine_t*);

#endif