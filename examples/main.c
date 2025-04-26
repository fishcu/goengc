#include <stdio.h>

#include "goengc/board.h"
#include "goengc/bitfield.h"
#include "goengc/color_field.h"
#include "goengc/floodfill.h"
#include "goengc/types.h"

/* Helper function to print the entire board */
void print_board(const GoengcColorField* field) {
    printf("Board state (full board):\n");
    for (int y = 0; y < GOENGC_DATA_SIZE; y++) {
        for (int x = 0; x < GOENGC_DATA_SIZE; x++) {
            uint16_t index = goengc_coord_to_index(x, y);
            GoengcColor color = goengc_colorfield_get_color(field, index);
            
            switch(color) {
                case GOENGC_COLOR_BLACK: printf("B "); break;
                case GOENGC_COLOR_WHITE: printf("W "); break;
                case GOENGC_COLOR_EMPTY: printf(". "); break;
                case GOENGC_COLOR_OFF_BOARD: printf("# "); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Helper function to print a small region of the board (for debugging) */
void print_board_region(const GoengcColorField* field, int x_start, int y_start, 
                        int width, int height) {
    printf("Board state (region):\n");
    for (int y = y_start; y < y_start + height; y++) {
        for (int x = x_start; x < x_start + width; x++) {
            uint16_t index = goengc_coord_to_index(x, y);
            GoengcColor color = goengc_colorfield_get_color(field, index);
            
            switch(color) {
                case GOENGC_COLOR_BLACK: printf("B "); break;
                case GOENGC_COLOR_WHITE: printf("W "); break;
                case GOENGC_COLOR_EMPTY: printf(". "); break;
                case GOENGC_COLOR_OFF_BOARD: printf("# "); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Helper function to print the entire flood fill result */
void print_visited(const GoengcBitfield* visited) {
    printf("Flood fill result (full board):\n");
    for (int y = 0; y < GOENGC_DATA_SIZE; y++) {
        for (int x = 0; x < GOENGC_DATA_SIZE; x++) {
            uint16_t index = goengc_coord_to_index(x, y);
            if (goengc_bitfield_get_bit(visited, index)) {
                printf("X "); /* visited */
            } else {
                printf(". "); /* not visited */
            }
        }
        printf("\n");
    }
    printf("\n");
}

/* Helper function to print visited points from a flood fill in a region (for debugging) */
void print_visited_region(const GoengcBitfield* visited, int x_start, int y_start, 
                         int width, int height) {
    printf("Flood fill result (region):\n");
    for (int y = y_start; y < y_start + height; y++) {
        for (int x = x_start; x < x_start + width; x++) {
            uint16_t index = goengc_coord_to_index(x, y);
            if (goengc_bitfield_get_bit(visited, index)) {
                printf("X "); /* visited */
            } else {
                printf(". "); /* not visited */
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    /* Initialize a board */
    GoengcBoard board;
    printf("Size of board structure: %zu bytes\n", sizeof(GoengcBoard));
    GoengcVec2 board_size = goengc_vec2_create(19, 19);
    goengc_board_init(&board, board_size, 15,
                      GOENGC_SCORING_TERRITORY); /* komi = 7.5 */

    /* Reset the board to clear it */
    goengc_board_reset(&board);

    /* Set up a more complex pattern that forms an enclosed area */
    const int p = GOENGC_PAD; /* Padding offset */
    const int cx = 5 + p;     /* Center X for the pattern */
    const int cy = 5 + p;     /* Center Y for the pattern */
    
    /* Create a black "wall" pattern with some empty spaces inside */
    GoengcVec2 black_stones[] = {
        /* Top row */
        goengc_vec2_create(cx - 3, cy - 3),
        goengc_vec2_create(cx - 2, cy - 3),
        goengc_vec2_create(cx - 1, cy - 3),
        goengc_vec2_create(cx, cy - 3),
        goengc_vec2_create(cx + 1, cy - 3),
        goengc_vec2_create(cx + 2, cy - 3),
        
        /* Left side */
        goengc_vec2_create(cx - 3, cy - 2),
        goengc_vec2_create(cx - 3, cy - 1),
        goengc_vec2_create(cx - 3, cy),
        goengc_vec2_create(cx - 3, cy + 1),
        goengc_vec2_create(cx - 3, cy + 2),
        
        /* Right side */
        goengc_vec2_create(cx + 2, cy - 2),
        goengc_vec2_create(cx + 2, cy - 1),
        goengc_vec2_create(cx + 2, cy),
        goengc_vec2_create(cx + 2, cy + 1),
        goengc_vec2_create(cx + 2, cy + 2),
        
        /* Bottom row */
        goengc_vec2_create(cx - 3, cy + 2),
        goengc_vec2_create(cx - 2, cy + 2),
        goengc_vec2_create(cx - 1, cy + 2),
        goengc_vec2_create(cx, cy + 2),
        goengc_vec2_create(cx + 1, cy + 2),
        goengc_vec2_create(cx + 2, cy + 2),
    };
    
    /* Create some white stones inside the enclosed area */
    GoengcVec2 white_stones[] = {
        goengc_vec2_create(cx - 1, cy),
        goengc_vec2_create(cx, cy),
        goengc_vec2_create(cx, cy - 1),
    };
    
    /* Place the black stones */
    int num_black_stones = sizeof(black_stones) / sizeof(black_stones[0]);
    for (int i = 0; i < num_black_stones; i++) {
        goengc_board_setup_move(&board, goengc_move_create(GOENGC_COLOR_BLACK, 0, black_stones[i]));
    }
    
    /* Place the white stones */
    int num_white_stones = sizeof(white_stones) / sizeof(white_stones[0]);
    for (int i = 0; i < num_white_stones; i++) {
        goengc_board_setup_move(&board, goengc_move_create(GOENGC_COLOR_WHITE, 0, white_stones[i]));
    }
    
    /* Print the entire board */
    print_board(&board.color_field);
    
    /* For reference, also print just the region of interest */
    print_board_region(&board.color_field, cx - 5, cy - 5, 11, 11);
    
    /* Choose a starting point for flood fill (empty location inside the enclosed area) */
    GoengcVec2 seed = goengc_vec2_create(cx - 2, cy - 1);
    
    /* Run the flood fill using the board's scratch bitfields */
    goengc_flood_fill(&board.color_field, seed, &board.scratch1, &board.scratch2);
    
    /* Print the visited points (full board) */
    print_visited(&board.scratch2);
    
    /* Count the visited points */
    uint16_t count = goengc_bitfield_count_bits(&board.scratch2);
    printf("Number of connected empty points: %d\n\n", count);
    
    /* Try another flood fill from outside the enclosed area */
    GoengcVec2 outside_seed = goengc_vec2_create(cx - 5, cy - 5);
    
    /* Reset and run the flood fill from outside */
    goengc_bitfield_clear(&board.scratch1);
    goengc_bitfield_clear(&board.scratch2);
    goengc_flood_fill(&board.color_field, outside_seed, &board.scratch1, &board.scratch2);
    
    /* Print the visited points (full board) */
    print_visited(&board.scratch2);
    
    /* Count the visited points */
    count = goengc_bitfield_count_bits(&board.scratch2);
    printf("Number of connected empty points from outside: %d\n", count);

    return 0;
}
