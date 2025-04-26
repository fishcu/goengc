#include "goengc/constants.h"

/**
 * Definition of 4-connected neighbor deltas
 * These are used for traversal algorithms to navigate to adjacent points
 */
const int16_t GOENGC_NEIGHBOR_4[4] = {
    -GOENGC_DATA_SIZE, /* North: one row up */
    -1,                /* West: one column left */
    GOENGC_DATA_SIZE,  /* South: one row down */
    1                  /* East: one column right */
};
