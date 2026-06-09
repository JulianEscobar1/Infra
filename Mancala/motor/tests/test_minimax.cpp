#include "../src/board.h"
#include "../src/minimax.h"
#include <cassert>
#include <iostream>

void test_minimax_basic() {
    Board board;
    SearchResult result = minimax_search(board, Side::SOUTH, 2);

    // Make sure move is within South range [0, 5]
    assert(result.best_move >= 0 && result.best_move <= 5);
    // Make sure nodes are counted
    assert(result.nodes > 1);
    // Minimax doesn't prune
    assert(result.prunes == 0);
}

void test_minimax_depth_scaling() {
    Board board;
    SearchResult result_d1 = minimax_search(board, Side::SOUTH, 1);
    SearchResult result_d2 = minimax_search(board, Side::SOUTH, 2);
    SearchResult result_d3 = minimax_search(board, Side::SOUTH, 3);

    // Nodes visited should increase with depth
    assert(result_d3.nodes > result_d2.nodes);
    assert(result_d2.nodes > result_d1.nodes);
}

int main() {
    test_minimax_basic();
    test_minimax_depth_scaling();
    std::cout << "test_minimax passed successfully!" << std::endl;
    return 0;
}
