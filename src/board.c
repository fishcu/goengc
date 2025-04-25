#include "goengc/board.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "goengc/color_field.h"
#include "goengc/types.h"

void goengc_board_init(GoengcBoard* restrict board, GoengcCoord board_size, int8_t komi2,
                       GoengcScoring scoring) {
    assert(board != NULL);
    assert(board_size.x > 0 && board_size.y > 0);
    assert(board_size.x <= GOENGC_DATA_SIZE);
    assert(board_size.y <= GOENGC_DATA_SIZE);

    /* Initialize board configuration */
    board->board_size = board_size;
    board->komi2 = komi2;
    board->scoring = scoring;

    goengc_board_reset(board);
}

void goengc_board_reset(GoengcBoard* restrict board) {
    assert(board != NULL);

    /* Initialize color field as empty, surround with padding of "off board" color */
    goengc_colorfield_fill(&board->color_field, GOENGC_COLOR_OFF_BOARD);
    for (uint8_t y = GOENGC_PADDING; y < board->board_size.y + GOENGC_PADDING; y++) {
        for (uint8_t x = GOENGC_PADDING; x < board->board_size.x + GOENGC_PADDING; x++) {
            goengc_colorfield_set_color(&board->color_field, goengc_coord_to_index(x, y),
                                        GOENGC_COLOR_EMPTY);
        }
    }
    
    board->num_captures = 0;
}

void goengc_board_setup_move(GoengcBoard* restrict board, GoengcMove move) {
    assert(board != NULL);
    assert(move.color == GOENGC_COLOR_EMPTY || 
           move.color == GOENGC_COLOR_BLACK || 
           move.color == GOENGC_COLOR_WHITE);
    
    if (!move.is_pass) {
        uint8_t index = goengc_coord_to_index(move.coord.x, move.coord.y);
        goengc_colorfield_set_color(&board->color_field, index, move.color);
    }
}

/* Stub implementation - to be filled in later */
GoengcMoveLegality goengc_board_get_move_legality(GoengcBoard* restrict board, GoengcMove move) {
    assert(board != NULL);
    /* Implementation to be added later */
    return GOENGC_MOVE_LEGAL; /* Default return to avoid compiler warnings */
}

int goengc_board_is_legal(GoengcBoard* restrict board, GoengcMove move) {
    assert(board != NULL);
    return goengc_board_get_move_legality(board, move) == GOENGC_MOVE_LEGAL;
}

/* Stub implementation - to be filled in later */
void goengc_board_play(GoengcBoard* restrict board, GoengcMove move) {
    assert(board != NULL);
    /* Implementation to be added later */
}
