#pragma once

#include "board.h"

/// Default weight for the pit-seeds difference term
constexpr double DEFAULT_ALPHA_WEIGHT = 0.5;

/// Evaluate the board from the perspective of the given side.
///
///   h(state) = (own_kalaha − opp_kalaha)
///            + alpha_weight × (own_pit_seeds − opp_pit_seeds)
///
/// When the game is over, remaining pit seeds are virtually collected
/// into each player's kalaha and only the kalaha difference is returned.
int evaluate(const Board& board, Side side,
             double alpha_weight = DEFAULT_ALPHA_WEIGHT);
