#include "../src/board.h"
#include <cassert>
#include <iostream>

void test_extra_turn_earned() {
    // Starting from a pit that is exactly the distance to the Kalaha.
    // South pit 2 is 4 steps away from South Kalaha (6).
    // If South pit 2 has 4 seeds, last seed lands in Kalaha 6.
    std::array<int, 14> state = {
        0, 0, 4, 0, 0, 0,   0,
        0, 0, 0, 0, 0, 0,   0
    };
    Board board(state);
    bool extra = board.sow(Side::SOUTH, 2);

    assert(extra);
    assert(board.pits[2] == 0);
    assert(board.pits[3] == 1);
    assert(board.pits[4] == 1);
    assert(board.pits[5] == 1);
    assert(board.pits[6] == 1); // Landed in Kalaha
}

void test_no_extra_turn() {
    // South pit 2 has 3 seeds. Last seed lands in South pit 5.
    std::array<int, 14> state = {
        0, 0, 3, 0, 0, 0,   0,
        0, 0, 0, 0, 0, 0,   0
    };
    Board board(state);
    bool extra = board.sow(Side::SOUTH, 2);

    assert(!extra);
    assert(board.pits[6] == 0);
}

void test_skips_opponent_kalaha() {
    // South pit 5 has 8 seeds.
    // It should sow to: 6 (South Kalaha), 7, 8, 9, 10, 11, 12 (North pits).
    // It should SKIP 13 (North Kalaha) and land in 0 (South pit).
    std::array<int, 14> state = {
        0, 0, 0, 0, 0, 8,   0,
        0, 0, 0, 0, 0, 0,   0
    };
    Board board(state);
    bool extra = board.sow(Side::SOUTH, 5);

    assert(!extra); // Lands in 0, not 6
    assert(board.pits[6] == 1);  // South Kalaha got a seed
    assert(board.pits[13] == 0); // Opponent Kalaha was skipped!
    assert(board.pits[0] == 1);  // Landed in South pit 0
}

int main() {
    test_extra_turn_earned();
    test_no_extra_turn();
    test_skips_opponent_kalaha();
    std::cout << "test_extra_turn passed successfully!" << std::endl;
    return 0;
}
