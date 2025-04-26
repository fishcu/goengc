#ifndef GOENGC_POPCOUNT_H
#define GOENGC_POPCOUNT_H

#include <assert.h>
#include <stdint.h>

/**
 * Lookup table for the population count (number of set bits) of all possible
 * uint8_t values
 */
extern const uint8_t GOENGC_POPCOUNT_TABLE[256];

/**
 * Get the population count (number of set bits) for a uint8_t value
 * @param value The value to count bits in
 * @return The number of set bits (0-8)
 */
static inline uint8_t goengc_popcount(uint8_t value) {
    return GOENGC_POPCOUNT_TABLE[value];
}

#endif /* GOENGC_POPCOUNT_H */
