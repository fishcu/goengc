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
    uint8_t bytes[GOENGC_BITFIELD_SIZE];
    /* Tracking the active area (min and max indices of set bits) */
    uint16_t active_start; /* First bit that might be set (inclusive) */
    uint16_t active_end;   /* Past the last bit that might be set (exclusive) */
} GoengcBitfield;

/**
 * Initialize a bitfield by setting all bits to 0
 * @param bitfield The bitfield to initialize
 */
static inline void goengc_bitfield_clear(GoengcBitfield* restrict bitfield) {
    assert(bitfield != NULL);
    memset(bitfield->bytes, 0, GOENGC_BITFIELD_SIZE);
    bitfield->active_start = GOENGC_DATA_SIZE_SQUARED;
    bitfield->active_end = 0;
}

/**
 * Fill a bitfield by setting all bits to 1
 * @param bitfield The bitfield to initialize
 */
static inline void goengc_bitfield_fill(GoengcBitfield* restrict bitfield) {
    assert(bitfield != NULL);
    memset(bitfield->bytes, 0xFF, GOENGC_BITFIELD_SIZE);
    bitfield->active_start = 0;
    bitfield->active_end = GOENGC_DATA_SIZE_SQUARED;
}

/**
 * Set a specific bit in the bitfield
 * @param bitfield The bitfield to modify
 * @param index The index of the bit to set (0 to GOENGC_DATA_SIZE_SQUARED-1)
 */
static inline void goengc_bitfield_set_bit(GoengcBitfield* restrict bitfield,
                                           uint16_t index) {
    assert(bitfield != NULL);
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    uint16_t byte_index = index / 8;
    uint8_t bit_pos = index % 8;
    bitfield->bytes[byte_index] |= (1 << bit_pos);

    /* Update active area */
    if (index < bitfield->active_start) {
        bitfield->active_start = index;
    }
    if (index >= bitfield->active_end) {
        bitfield->active_end = index + 1;
    }
}

/**
 * Clear a specific bit in the bitfield
 * @param bitfield The bitfield to modify
 * @param index The index of the bit to clear (0 to GOENGC_DATA_SIZE_SQUARED-1)
 */
static inline void goengc_bitfield_clear_bit(GoengcBitfield* restrict bitfield,
                                             uint16_t index) {
    assert(bitfield != NULL);
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    uint16_t byte_index = index / 8;
    uint8_t bit_pos = index % 8;
    bitfield->bytes[byte_index] &= ~(1 << bit_pos);

    /* Note: We don't update active_start/active_end for clear operations
     * since they're rarely used and we prefer a non-tight bound
     */
}

/**
 * Check if a specific bit is set in the bitfield
 * @param bitfield The bitfield to check
 * @param index The index of the bit to check
 * @return 1 if the bit is set, 0 otherwise
 */
static inline int goengc_bitfield_get_bit(
    const GoengcBitfield* restrict bitfield, uint16_t index) {
    assert(index < GOENGC_DATA_SIZE_SQUARED);
    uint16_t byte_index = index / 8;
    uint8_t bit_pos = index % 8;
    return (bitfield->bytes[byte_index] & (1 << bit_pos)) ? 1 : 0;
}

/**
 * Count the number of bits set in the bitfield
 * @param bitfield The bitfield to count
 * @return The number of bits set
 */
static inline uint16_t goengc_bitfield_count_bits(
    const GoengcBitfield* restrict bitfield) {
    assert(bitfield != NULL);

    uint16_t count = 0;

    /* Calculate byte indices for active area */
    uint16_t start_byte = bitfield->active_start / 8;
    uint16_t end_byte = (bitfield->active_end + 7) / 8;

    /* Count bits only in the active area */
    for (uint16_t i = start_byte; i < end_byte; i++) {
        count += goengc_popcount(bitfield->bytes[i]);
    }

    return count;
}

#endif /* GOENGC_BITFIELD_H */
