#include "minimax.h"
#include "heuristic.h"
#include <limits>
#include <algorithm>

// -------------------------------------------------------
//  Internal recursive minimax
// -------------------------------------------------------

static int minimax_impl(const Board& board,
                        Side root_side,
                        Side current_side,
                        int  depth,
                        double alpha_weight,
                        int64_t& nodes) {
    nodes++;

    if (depth == 0 || board.is_game_over()) {
        return evaluate(board, root_side, alpha_weight);
    }

    auto moves = board.valid_moves(current_side);
    if (moves.empty()) {
        return evaluate(board, root_side, alpha_weight);
    }

    bool maximizing = (current_side == root_side);
    int best = maximizing ? std::numeric_limits<int>::min()
                          : std::numeric_limits<int>::max();

    for (int move : moves) {
        Board child = board;
        bool extra_turn = child.sow(current_side, move);
        Side next_side = extra_turn ? current_side : opposite_side(current_side);

        int val = minimax_impl(child, root_side, next_side,
                               depth - 1, alpha_weight, nodes);

        if (maximizing) {
            best = std::max(best, val);
        } else {
            best = std::min(best, val);
        }
    }

    return best;
}

// -------------------------------------------------------
//  Public entry point
// -------------------------------------------------------

SearchResult minimax_search(const Board& board, Side side, int depth,
                            double alpha_weight) {
    SearchResult result;
    result.nodes = 1;  // count root node

    auto moves = board.valid_moves(side);
    if (moves.empty()) {
        result.evaluation = evaluate(board, side, alpha_weight);
        return result;
    }

    int best_eval = std::numeric_limits<int>::min();

    for (int move : moves) {
        Board child = board;
        bool extra_turn = child.sow(side, move);
        Side next_side = extra_turn ? side : opposite_side(side);

        int64_t child_nodes = 0;
        int eval = minimax_impl(child, side, next_side,
                                depth - 1, alpha_weight, child_nodes);
        result.nodes += child_nodes;

        if (eval > best_eval) {
            best_eval = eval;
            result.best_move = move;
        }
    }

    result.evaluation = best_eval;
    result.prunes = 0;  // pure minimax never prunes
    return result;
}
