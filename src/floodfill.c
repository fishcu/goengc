#include "goengc/floodfill.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "goengc/bitfield.h"
#include "goengc/color_field.h"
#include "goengc/constants.h"
#include "goengc/size.h"
#include "goengc/types.h"

/* Flood fill implementation */
void goengc_flood_fill(const GoengcColorField* restrict color_field,
                       GoengcVec2 seed, GoengcBitfield* restrict same_color,
                       GoengcBitfield* restrict visited) {
    assert(color_field != NULL);
    assert(same_color != NULL);
    assert(visited != NULL);

    /* Convert seed coordinates to index */
    uint16_t seed_index = goengc_coord_to_index(seed.x, seed.y);

    /* Get the seed color that we'll be matching */
    GoengcColor target_color =
        goengc_colorfield_get_color(color_field, seed_index);

    /* Reset the scratch bitfield and visited */
    goengc_bitfield_clear(same_color);
    goengc_bitfield_clear(visited);

    /* Pre-processing: Mark all positions with the same color as the seed */
    for (uint16_t index = 0; index < GOENGC_DATA_SIZE_SQUARED; index++) {
        if (goengc_colorfield_get_color(color_field, index) == target_color) {
            goengc_bitfield_set_bit(same_color, index);
        }
    }

    /* Initialize visited with the seed */
    goengc_bitfield_set_bit(visited, seed_index);

    /* Initialize frontier tracking */
    uint16_t current_start = seed_index;
    uint16_t current_end = seed_index + 1; /* exclusive end */

    /* Iterative flooding */
    while (current_start < current_end) {
        /* Initialize next frontier as empty range */
        uint16_t next_start = GOENGC_DATA_SIZE_SQUARED;
        uint16_t next_end = 0;

        /* Process current frontier */
        for (uint16_t index = current_start; index < current_end; index++) {
            /* Only process if this position is visited */
            if (goengc_bitfield_get_bit(visited, index)) {
                /* Check all 4 neighbors */
                for (int n = 0; n < 4; n++) {
                    /* Calculate neighbor index directly in 1D space */
                    uint16_t neighbor_index = index + GOENGC_NEIGHBOR_4[n];

                    /* If this neighbor has the same color and hasn't been
                     * visited */
                    if (goengc_bitfield_get_bit(same_color, neighbor_index) &&
                        !goengc_bitfield_get_bit(visited, neighbor_index)) {
                        /* Mark as visited */
                        goengc_bitfield_set_bit(visited, neighbor_index);

                        /* Update next frontier bounds */
                        if (neighbor_index < next_start) {
                            next_start = neighbor_index;
                        }
                        if (neighbor_index >= next_end) {
                            next_end = neighbor_index + 1; /* exclusive end */
                        }
                    }
                }
            }
        }

        /* Update current frontier to next frontier */
        current_start = next_start;
        current_end = next_end;
    }
}
