#include "heuristic.h"
#include <cmath>

int evaluate(const Board& board, Side side, double alpha_weight) {
    Side opp = opposite_side(side);

    if (board.is_game_over()) {
        // Virtually collect remaining seeds into each kalaha
        int own_total = board.pits[Board::kalaha(side)] + board.side_seeds(side);
        int opp_total = board.pits[Board::kalaha(opp)]  + board.side_seeds(opp);
        return own_total - opp_total;
    }

    int kalaha_diff = board.pits[Board::kalaha(side)] - board.pits[Board::kalaha(opp)];
    int seeds_diff  = board.side_seeds(side) - board.side_seeds(opp);

    return kalaha_diff + static_cast<int>(std::round(alpha_weight * seeds_diff));
}
