#include <stdlib.h>

#include "chapter7_1_engine.h"

typedef struct {
    struct engine_t* engine;
} car_t;

car_t* car_new() {
    return (car_t*)malloc(sizeof(car_t));
}

void car_ctor(car_t* car) {
    car->engine = engine_new();
    engine_ctor(car->engine);
}

void car_dtor(car_t* car) {
    engine_dtor(car->engine);
    free(car->engine);
}

void car_start(car_t* car) {
    engine_turn_on(car->engine);
}

void car_stop(car_t* car) {
    engine_turn_off(car->engine);
}

double car_get_engine_temperature(car_t* car) {
    return engine_get_temperature(car->engine);
}