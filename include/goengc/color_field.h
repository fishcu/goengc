#ifndef GOENGC_COLORFIELD_H
#define GOENGC_COLORFIELD_H

#include "size.h"
#include "types.h"
#include "bitfield.h"
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

/**
 * A color field with GOENGC_DATA_SIZE_SQUARED positions.
 * Each position's color is represented using 2 bits split across 2 bitfields:
 * - Bit 1 (occupied_bits): 0 = empty/off-board, 1 = occupied (black/white)
 * - Bit 2 (color_bits): 0 = black/off-board, 1 = white/empty
 */
typedef struct {
    GoengcBitfield occupied_bits;  /* 1 = stone present (black/white), 0 = empty/off-board */
    GoengcBitfield color_bits;     /* 1 = white/empty, 0 = black/off-board */
} GoengcColorField;

/**
 * Initialize a color field by setting all positions to a specified color
 * @param field The color field to initialize
 * @param color The color to fill with
 */
static inline void goengc_colorfield_fill(GoengcColorField* restrict field, GoengcColor color) {
    assert(field != NULL);
    
    /* Extract the individual bits from the color enum */
    uint8_t is_occupied = (color >> 1) & 0x01;  /* Bit 1 (MSB) */
    uint8_t is_white = color & 0x01;            /* Bit 0 (LSB) */
    
    /* Set the bitfields accordingly */
    if (is_occupied) {
        goengc_bitfield_fill(&field->occupied_bits);
    } else {
        goengc_bitfield_clear(&field->occupied_bits);
    }
    
    if (is_white) {
        goengc_bitfield_fill(&field->color_bits);
    } else {
        goengc_bitfield_clear(&field->color_bits);
    }
}

/**
 * Get the color at a specific position
 * @param field The color field to query
 * @param index The index of the position (0 to GOENGC_DATA_SIZE_SQUARED-1)
 * @return The color at the specified position
 */
static inline GoengcColor goengc_colorfield_get_color(const GoengcColorField* restrict field, uint8_t index) {
    assert(field != NULL);
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    
    /* Get the individual bits */
    uint8_t is_occupied = goengc_bitfield_get_bit(&field->occupied_bits, index);
    uint8_t is_white = goengc_bitfield_get_bit(&field->color_bits, index);
    
    /* Combine the bits to form the color */
    return (GoengcColor)((is_occupied << 1) | is_white);
}

/**
 * Set the color at a specific position
 * @param field The color field to modify
 * @param index The index of the position (0 to GOENGC_DATA_SIZE_SQUARED-1)
 * @param color The color to set
 */
static inline void goengc_colorfield_set_color(GoengcColorField* restrict field, uint8_t index, GoengcColor color) {
    assert(field != NULL);
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    
    /* Extract the individual bits from the color enum */
    uint8_t is_occupied = (color >> 1) & 0x01;  /* Bit 1 (MSB) */
    uint8_t is_white = color & 0x01;            /* Bit 0 (LSB) */
    
    /* Set the bitfields accordingly */
    if (is_occupied) {
        goengc_bitfield_set_bit(&field->occupied_bits, index);
    } else {
        goengc_bitfield_clear_bit(&field->occupied_bits, index);
    }
    
    if (is_white) {
        goengc_bitfield_set_bit(&field->color_bits, index);
    } else {
        goengc_bitfield_clear_bit(&field->color_bits, index);
    }
}

#endif /* GOENGC_COLORFIELD_H */ 