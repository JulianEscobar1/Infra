#include "../src/board.h"
#include "../src/minimax.h"
#include "../src/alphabeta.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_equivalence_on_state(const Board& board, int max_depth) {
    for (int depth = 1; depth <= max_depth; ++depth) {
        SearchResult res_mini = minimax_search(board, Side::SOUTH, depth);
        SearchResult res_ab   = alphabeta_search(board, Side::SOUTH, depth);

        // Check if evaluations match
        if (res_mini.evaluation != res_ab.evaluation) {
            std::cerr << "Mismatch in evaluation at depth " << depth << "\n"
                      << "Minimax: " << res_mini.evaluation << "\n"
                      << "AlphaBeta: " << res_ab.evaluation << std::endl;
            assert(false);
        }

        // Check if chosen moves match
        if (res_mini.best_move != res_ab.best_move) {
            std::cerr << "Mismatch in best move at depth " << depth << "\n"
                      << "Minimax chose: " << res_mini.best_move << "\n"
                      << "AlphaBeta chose: " << res_ab.best_move << std::endl;
            assert(false);
        }
    }
}

int main() {
    // 1. Test starting board
    Board starting_board;
    test_equivalence_on_state(starting_board, 5);

    // 2. Test a custom mid-game board state
    std::array<int, 14> mid_state = {
        1, 0, 5, 2, 0, 8,   4,
        3, 0, 2, 7, 1, 0,   12
    };
    Board mid_board(mid_state);
    test_equivalence_on_state(mid_board, 5);

    // 3. Test another board state with fewer seeds (closer to endgame)
    std::array<int, 14> end_state = {
        0, 1, 0, 2, 0, 1,   20,
        1, 0, 0, 1, 0, 0,   22
    };
    Board end_board(end_state);
    test_equivalence_on_state(end_board, 5);

    std::cout << "test_equivalence passed successfully! Minimax and Alpha-Beta produce identical moves and evaluations." << std::endl;
    return 0;
}
