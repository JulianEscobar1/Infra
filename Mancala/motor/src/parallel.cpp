#include "parallel.h"
#include "alphabeta.h"
#include "heuristic.h"
#include <omp.h>
#include <limits>
#include <algorithm>
#include <vector>

SearchResult parallel_alphabeta_search(const Board& board, Side side, int depth, int threads,
                                       double alpha_weight) {
    SearchResult result;
    result.nodes = 1; // Count root node

    auto moves = board.valid_moves(side);
    if (moves.empty()) {
        result.evaluation = evaluate(board, side, alpha_weight);
        return result;
    }

    int num_moves = static_cast<int>(moves.size());
    std::vector<int> evaluations(num_moves, std::numeric_limits<int>::min());
    std::vector<int64_t> thread_nodes(num_moves, 0);
    std::vector<int64_t> thread_prunes(num_moves, 0);

    // Run OpenMP parallel region over all available moves at the root
    #pragma omp parallel for num_threads(threads) schedule(dynamic)
    for (int i = 0; i < num_moves; ++i) {
        int move = moves[i];
        Board child = board;
        bool extra_turn = child.sow(side, move);
        Side next_side = extra_turn ? side : opposite_side(side);

        // In Root Parallelism, each child sub-tree search is independent
        int alpha = std::numeric_limits<int>::min();
        int beta  = std::numeric_limits<int>::max();

        int64_t nodes_visited = 0;
        int64_t prunes_made = 0;

        evaluations[i] = alphabeta_impl(child, side, next_side,
                                        depth - 1, alpha, beta,
                                        alpha_weight, nodes_visited, prunes_made);
        thread_nodes[i] = nodes_visited;
        thread_prunes[i] = prunes_made;
    }

    // Reduce results sequentially
    int best_eval = std::numeric_limits<int>::min();
    for (int i = 0; i < num_moves; ++i) {
        result.nodes += thread_nodes[i];
        result.prunes += thread_prunes[i];
        if (evaluations[i] > best_eval) {
            best_eval = evaluations[i];
            result.best_move = moves[i];
        }
    }
    result.evaluation = best_eval;

    return result;
}
