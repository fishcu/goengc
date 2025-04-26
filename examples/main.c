#include <stdio.h>

#include "goengc/board.h"
#include "goengc/color_field.h"
#include "goengc/types.h"

int main(void) {
    /* Initialize a board */
    GoengcBoard board;
    GoengcVec2 board_size = goengc_vec2_create(19, 19);
    goengc_board_init(&board, board_size, 15,
                      GOENGC_SCORING_TERRITORY); /* komi = 7.5 */

    /* Reset the board to clear it */
    goengc_board_reset(&board);

    /* Set up a handicap stone pattern */
    GoengcVec2 c1 = goengc_vec2_create(3 + GOENGC_PAD, 3 + GOENGC_PAD);
    GoengcVec2 c2 = goengc_vec2_create(9 + GOENGC_PAD, 9 + GOENGC_PAD);
    GoengcVec2 c3 =
        goengc_vec2_create(15 + GOENGC_PAD, 15 + GOENGC_PAD);

    goengc_board_setup_move(&board,
                            goengc_move_create(GOENGC_COLOR_BLACK, 0, c1));
    goengc_board_setup_move(&board,
                            goengc_move_create(GOENGC_COLOR_BLACK, 0, c2));
    goengc_board_setup_move(&board,
                            goengc_move_create(GOENGC_COLOR_BLACK, 0, c3));

    /* Display the komi value */
    printf("Komi value: %.1f\n", board.komi2 / 2.0f);

    /* We've successfully created a board with handicap stones */
    printf("Go board initialized successfully with handicap stones\n");

    return 0;
}
