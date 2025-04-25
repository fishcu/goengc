#ifndef GOENGC_BITFIELD_H
#define GOENGC_BITFIELD_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "popcount.h"
#include "size.h"

#define GOENGC_BITFIELD_SIZE (GOENGC_DATA_SIZE_SQUARED + 7) / 8

/**
 * A bitfield with GOENGC_DATA_SIZE_SQUARED bits.
 * Used to represent various board states.
 */
typedef struct {
    /* Using uint8_t (8 bits per byte) for storage */
    uint8_t bits[GOENGC_BITFIELD_SIZE];
} GoengcBitfield;

/**
 * Initialize a bitfield by setting all bits to 0
 * @param bitfield The bitfield to initialize
 */
static inline void goengc_bitfield_clear(GoengcBitfield* restrict bitfield) {
    assert(bitfield != NULL);
    memset(bitfield->bits, 0, GOENGC_BITFIELD_SIZE);
}

/**
 * Fill a bitfield by setting all bits to 1
 * @param bitfield The bitfield to initialize
 */
static inline void goengc_bitfield_fill(GoengcBitfield* restrict bitfield) {
    assert(bitfield != NULL);
    memset(bitfield->bits, 0xFF, GOENGC_BITFIELD_SIZE);
}

/**
 * Set a specific bit in the bitfield
 * @param bitfield The bitfield to modify
 * @param index The index of the bit to set (0 to GOENGC_DATA_SIZE_SQUARED-1)
 */
static inline void goengc_bitfield_set_bit(GoengcBitfield* restrict bitfield, uint8_t index) {
    assert(bitfield != NULL);
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    uint8_t byte_index = index / 8;
    uint8_t bit_pos = index % 8;
    bitfield->bits[byte_index] |= (1 << bit_pos);
}

/**
 * Clear a specific bit in the bitfield
 * @param bitfield The bitfield to modify
 * @param index The index of the bit to clear (0 to GOENGC_DATA_SIZE_SQUARED-1)
 */
static inline void goengc_bitfield_clear_bit(GoengcBitfield* restrict bitfield, uint8_t index) {
    assert(bitfield != NULL);
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    uint8_t byte_index = index / 8;
    uint8_t bit_pos = index % 8;
    bitfield->bits[byte_index] &= ~(1 << bit_pos);
}

/**
 * Check if a specific bit is set in the bitfield
 * @param bitfield The bitfield to check
 * @param index The index of the bit to check
 * @return 1 if the bit is set, 0 otherwise
 */
static inline int goengc_bitfield_get_bit(const GoengcBitfield* restrict bitfield, uint8_t index) {
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    uint8_t byte_index = index / 8;
    uint8_t bit_pos = index % 8;
    return (bitfield->bits[byte_index] & (1 << bit_pos)) ? 1 : 0;
}

/**
 * Count the number of bits set in the bitfield
 * @param bitfield The bitfield to count
 * @return The number of bits set
 */
static inline uint16_t goengc_bitfield_count_bits(const GoengcBitfield* restrict bitfield) {
    assert(bitfield != NULL);
    uint16_t count = 0;
    for (uint8_t i = 0; i < GOENGC_BITFIELD_SIZE; i++) {
        count += goengc_popcount(bitfield->bits[i]);
    }
    return count;
}

#endif /* GOENGC_BITFIELD_H */
