#ifndef GOENGC_TYPES_H
#define GOENGC_TYPES_H

#include "size.h"
#include <assert.h>
#include <stdint.h>

/* Enum for stone colors using bit field representation
 * Bit meanings:
 * - 1st bit: empty(0) / occupied(1)
 * - 2nd bit: black(0) / white(1)
 */
typedef enum {
    GOENGC_COLOR_OFF_BOARD = 0, /* 00 - off board */
    GOENGC_COLOR_EMPTY = 1,     /* 01 - empty space */
    GOENGC_COLOR_BLACK = 2,     /* 10 - black stone */
    GOENGC_COLOR_WHITE = 3      /* 11 - white stone */
} GoengcColor;

/* Get the opposite color (black↔white) */
static inline GoengcColor goengc_color_opposite(GoengcColor color) {
    assert(color == GOENGC_COLOR_BLACK || color == GOENGC_COLOR_WHITE);
    return color == GOENGC_COLOR_BLACK ? GOENGC_COLOR_WHITE : GOENGC_COLOR_BLACK;
}

/* 2D coordinate structure */
typedef struct {
    /* Coordinate {0, 0} is A19 (top-left corner) in common coordinate display form. */
    uint8_t x;  /* Denotes column (left to right) */
    uint8_t y;  /* Denotes row (top to bottom) */
} GoengcCoord;

/* Create a coordinate */
static inline GoengcCoord goengc_coord_create(uint8_t x, uint8_t y) {
    /* Using C99+ designated initializers */
    return (GoengcCoord){ .x = x, .y = y };
}

/* Check if two coordinates are equal */
static inline int goengc_coord_equals(GoengcCoord a, GoengcCoord b) {
    return a.x == b.x && a.y == b.y;
}

/**
 * Convert between 1D index and 2D coordinates
 * @param x The x coordinate
 * @param y The y coordinate
 * @return The 1D index corresponding to these coordinates
 */
static inline uint8_t goengc_coord_to_index(uint8_t x, uint8_t y) {
    assert(x < GOENGC_DATA_SIZE);
    assert(y < GOENGC_DATA_SIZE);
    return y * GOENGC_DATA_SIZE + x;
}

/**
 * Convert between 1D index and 2D coordinates
 * @param index The 1D index
 * @return The 2D coordinates corresponding to this index
 */
static inline GoengcCoord goengc_index_to_coord(uint8_t index) {
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    return (GoengcCoord){ .x = index % GOENGC_DATA_SIZE, .y = index / GOENGC_DATA_SIZE };
}

/* Move structure */
typedef struct {
    GoengcColor color;
    int is_pass;         /* 1 if this is a pass, 0 otherwise */
    GoengcCoord coord;   /* Only valid if is_pass is 0 */
} GoengcMove;

/* Create a new move */
static inline GoengcMove goengc_move_create(GoengcColor color, int is_pass, GoengcCoord coord) {
    /* Using C99+ designated initializers */
    return (GoengcMove){ 
        .color = color, 
        .is_pass = is_pass, 
        .coord = coord 
    };
}

/* Check if two moves are equal */
static inline int goengc_move_equals(GoengcMove a, GoengcMove b) {
    /* Both moves need to be a pass of the same color (coordinates don't matter),
       or both need to be not a pass and have the same coordinates. */
    return a.color == b.color && 
           (a.is_pass ? b.is_pass : (!b.is_pass && goengc_coord_equals(a.coord, b.coord)));
}

/* Move legality codes */
typedef enum {
    GOENGC_MOVE_LEGAL = 0,
    GOENGC_MOVE_NON_EMPTY = 1,
    GOENGC_MOVE_SUICIDAL = 2,
    GOENGC_MOVE_KO = 3  /* Superko or ko */
} GoengcMoveLegality;

#endif /* GOENGC_TYPES_H */
