#ifndef GOENGC_FLOODFILL_H
#define GOENGC_FLOODFILL_H

#include <assert.h>
#include <stdint.h>

#include "bitfield.h"
#include "color_field.h"
#include "types.h"

/**
 * Perform a 4-connected flood fill from a seed position
 * @param color_field The color field to operate on
 * @param seed The starting coordinate for the flood fill
 * @param same_color Scratch bitfield to track positions with the same color
 * @param visited Bitfield to store visited positions (output)
 */
void goengc_flood_fill(const GoengcColorField* restrict color_field,
                       GoengcVec2 seed, GoengcBitfield* restrict same_color,
                       GoengcBitfield* restrict visited);

#endif /* GOENGC_FLOODFILL_H */
