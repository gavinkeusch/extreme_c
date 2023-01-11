#ifndef CHAPTER_7_2_PLAYER_H
#define CHAPTER_7_2_PLAYER_H

struct player_t;
struct gun_t;

struct player_t* player_new();
void player_ctor(struct player_t*, const char*);
void player_dtor(struct player_t*);

void player_pickup_gun(struct player_t*, struct gun_t*);
void player_shoot(struct player_t*);
void player_drop_gun(struct player_t*);

#endif