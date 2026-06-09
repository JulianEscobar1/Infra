#pragma once

#include "board.h"
#include <cstdint>

/// Result returned by all search algorithms
struct SearchResult {
    int      best_move  = -1;   ///< Pit index of the best move (-1 if none)
    int      evaluation = 0;    ///< Heuristic evaluation of the best move
    int64_t  nodes      = 0;    ///< Total nodes visited
    int64_t  prunes     = 0;    ///< Total alpha-beta cutoffs (0 for pure minimax)
};

/// Pure Minimax search (no pruning).
/// @param board        Current board state
/// @param side         Side to move (the maximizing player)
/// @param depth        Search depth (≥ 1)
/// @param alpha_weight Weight for the pit-seeds term in the heuristic
SearchResult minimax_search(const Board& board, Side side, int depth,
                            double alpha_weight = 0.5);
