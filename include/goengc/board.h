#ifndef GOENGC_BOARD_H
#define GOENGC_BOARD_H

#include <stdint.h>

#include "bitfield.h"
#include "color_field.h"
#include "size.h"
#include "types.h"

/* Scoring system enumeration */
typedef enum {
    GOENGC_SCORING_TERRITORY = 0,
    GOENGC_SCORING_AREA = 1
} GoengcScoring;

/* Board structure */
typedef struct {
    /* Board configuration */
    GoengcVec2 board_size; /* Typically 19x19, 13x13, or 9x9 */
    int8_t komi2; /* Compensation points for white * 2 (e.g., 7.5 -> 15) */
    GoengcScoring scoring; /* Scoring system */

    /* Board state */
    GoengcColorField color_field; /* Colors at each position */
    int8_t
        num_captures; /* Number of captures by Black minus captures by White */

    /* Utility bitfields for flood fill and counting operations */
    GoengcBitfield scratch1;
    GoengcBitfield scratch2;
} GoengcBoard;

/**
 * Initialize a Go board with the given parameters
 * @param board The board to initialize
 * @param board_size Size of the board (typically 19x19)
 * @param komi2 Compensation points for white * 2 (e.g., 7.5 -> 15)
 * @param scoring Scoring system to use
 */
void goengc_board_init(GoengcBoard* restrict board, GoengcVec2 board_size,
                       int8_t komi2, GoengcScoring scoring);

/**
 * Reset the board to its initial state
 * @param board The board to reset
 */
void goengc_board_reset(GoengcBoard* restrict board);

/**
 * Set up a stone on the board (for handicap and setup moves)
 * Does not handle legality checks or captures.
 * Does not influence ko.
 * Allows for "Empty" moves, meaning it erases stones from the board.
 *
 * @param board The board to modify
 * @param move The move to set up
 */
void goengc_board_setup_move(GoengcBoard* restrict board, GoengcMove move);

/**
 * Check if a move is legal
 * @param board The board to check
 * @param move The move to check
 * @return The legality status of the move
 */
GoengcMoveLegality goengc_board_get_move_legality(GoengcBoard* restrict board,
                                                  GoengcMove move);

/**
 * Convenience function to check if a move is legal
 * @param board The board to check
 * @param move The move to check
 * @return 1 if legal, 0 if illegal
 */
int goengc_board_is_legal(GoengcBoard* restrict board, GoengcMove move);

/**
 * Play a move on the board
 * @param board The board to modify
 * @param move The move to play
 */
void goengc_board_play(GoengcBoard* restrict board, GoengcMove move);

#endif /* GOENGC_BOARD_H */
