#pragma once

#include "board.h"
#include "minimax.h"  // SearchResult

/// Root-Parallel Alpha-Beta pruned search using OpenMP.
/// @param board        Current board state
/// @param side         Side to move (the maximizing player)
/// @param depth        Search depth (≥ 1)
/// @param threads      Number of OpenMP threads to use
/// @param alpha_weight Weight for the pit-seeds term in the heuristic
SearchResult parallel_alphabeta_search(const Board& board, Side side, int depth, int threads,
                                       double alpha_weight = 0.5);
