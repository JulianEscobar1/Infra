#pragma once

#include "board.h"
#include "minimax.h"  // SearchResult
#include <cstdint>

/// Alpha-Beta pruned search.
/// @param board        Current board state
/// @param side         Side to move (the maximizing player)
/// @param depth        Search depth (≥ 1)
/// @param alpha_weight Weight for the pit-seeds term in the heuristic
SearchResult alphabeta_search(const Board& board, Side side, int depth,
                              double alpha_weight = 0.5);

/// Internal recursive alpha-beta (exposed for use by parallel.cpp).
/// @return Evaluation from the perspective of root_side
int alphabeta_impl(const Board& board,
                   Side root_side,
                   Side current_side,
                   int  depth,
                   int  alpha,
                   int  beta,
                   double alpha_weight,
                   int64_t& nodes,
                   int64_t& prunes);
