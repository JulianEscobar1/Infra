#include "../src/board.h"
#include "../src/alphabeta.h"
#include "../src/minimax.h"
#include <cassert>
#include <iostream>

void test_alphabeta_basic() {
    Board board;
    SearchResult result = alphabeta_search(board, Side::SOUTH, 2);

    assert(result.best_move >= 0 && result.best_move <= 5);
    assert(result.nodes > 1);
}

void test_alphabeta_pruning_performance() {
    Board board;
    int depth = 5;

    SearchResult res_minimax = minimax_search(board, Side::SOUTH, depth);
    SearchResult res_ab = alphabeta_search(board, Side::SOUTH, depth);

    // Alpha-beta must evaluate fewer nodes than pure minimax due to pruning
    assert(res_ab.nodes < res_minimax.nodes);
    // Prunes count must be non-zero
    assert(res_ab.prunes > 0);
    // Evaluations must be the same
    assert(res_ab.evaluation == res_minimax.evaluation);
}

int main() {
    test_alphabeta_basic();
    test_alphabeta_pruning_performance();
    std::cout << "test_alphabeta passed successfully!" << std::endl;
    return 0;
}
