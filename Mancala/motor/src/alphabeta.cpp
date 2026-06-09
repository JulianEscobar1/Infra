#include "alphabeta.h"
#include "heuristic.h"
#include <limits>
#include <algorithm>

// -------------------------------------------------------
//  Internal recursive alpha-beta
// -------------------------------------------------------

int alphabeta_impl(const Board& board,
                   Side root_side,
                   Side current_side,
                   int  depth,
                   int  alpha,
                   int  beta,
                   double alpha_weight,
                   int64_t& nodes,
                   int64_t& prunes) {
    nodes++;

    if (depth == 0 || board.is_game_over()) {
        return evaluate(board, root_side, alpha_weight);
    }

    auto moves = board.valid_moves(current_side);
    if (moves.empty()) {
        return evaluate(board, root_side, alpha_weight);
    }

    bool maximizing = (current_side == root_side);

    if (maximizing) {
        int value = std::numeric_limits<int>::min();
        for (int move : moves) {
            Board child = board;
            bool extra_turn = child.sow(current_side, move);
            Side next_side = extra_turn ? current_side
                                        : opposite_side(current_side);

            value = std::max(value,
                alphabeta_impl(child, root_side, next_side,
                               depth - 1, alpha, beta,
                               alpha_weight, nodes, prunes));

            alpha = std::max(alpha, value);
            if (alpha >= beta) {
                prunes++;
                break;   // β cutoff
            }
        }
        return value;
    } else {
        int value = std::numeric_limits<int>::max();
        for (int move : moves) {
            Board child = board;
            bool extra_turn = child.sow(current_side, move);
            Side next_side = extra_turn ? current_side
                                        : opposite_side(current_side);

            value = std::min(value,
                alphabeta_impl(child, root_side, next_side,
                               depth - 1, alpha, beta,
                               alpha_weight, nodes, prunes));

            beta = std::min(beta, value);
            if (alpha >= beta) {
                prunes++;
                break;   // α cutoff
            }
        }
        return value;
    }
}

// -------------------------------------------------------
//  Public entry point
// -------------------------------------------------------

SearchResult alphabeta_search(const Board& board, Side side, int depth,
                              double alpha_weight) {
    SearchResult result;
    result.nodes = 1;  // count root node

    auto moves = board.valid_moves(side);
    if (moves.empty()) {
        result.evaluation = evaluate(board, side, alpha_weight);
        return result;
    }

    int best_eval = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta  = std::numeric_limits<int>::max();

    for (int move : moves) {
        Board child = board;
        bool extra_turn = child.sow(side, move);
        Side next_side = extra_turn ? side : opposite_side(side);

        int64_t child_nodes = 0, child_prunes = 0;
        int eval = alphabeta_impl(child, side, next_side,
                                  depth - 1, alpha, beta,
                                  alpha_weight, child_nodes, child_prunes);
        result.nodes  += child_nodes;
        result.prunes += child_prunes;

        if (eval > best_eval) {
            best_eval = eval;
            result.best_move = move;
        }
        alpha = std::max(alpha, eval);
    }

    result.evaluation = best_eval;
    return result;
}
