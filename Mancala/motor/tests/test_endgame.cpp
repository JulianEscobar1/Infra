#include "../src/board.h"
#include <cassert>
#include <iostream>

void test_endgame_trigger() {
    // South pits are empty. Game should be over.
    std::array<int, 14> state = {
        0, 0, 0, 0, 0, 0,   10,
        1, 1, 1, 1, 1, 1,   5
    };
    Board board(state);
    assert(board.is_game_over());
}

void test_endgame_not_trigger() {
    // Both sides have seeds
    std::array<int, 14> state = {
        0, 0, 0, 0, 0, 1,   10,
        0, 0, 0, 0, 0, 1,   5
    };
    Board board(state);
    assert(!board.is_game_over());
}

void test_collect_remaining_and_winner() {
    // South pits empty. North pits have some seeds.
    std::array<int, 14> state = {
        0, 0, 0, 0, 0, 0,   10, // South pits 0, Kalaha 10
        1, 2, 0, 0, 0, 0,   20  // North pits (7 has 1, 8 has 2), Kalaha 20
    };
    Board board(state);
    assert(board.is_game_over());

    board.collect_remaining();

    // South pits should remain empty
    for (int i = 0; i < 6; ++i) {
        assert(board.pits[i] == 0);
    }
    // North pits should be swept to North Kalaha (13)
    for (int i = 7; i < 13; ++i) {
        assert(board.pits[i] == 0);
    }

    assert(board.pits[6] == 10);  // South kalaha unchanged
    assert(board.pits[13] == 23); // North kalaha got 1 + 2 = 3 additional seeds (20 + 3 = 23)

    assert(board.winner() == 1); // North wins (13 has 23 > 6 has 10)
}

void test_draw() {
    std::array<int, 14> state = {
        0, 0, 0, 0, 0, 0,   24,
        0, 0, 0, 0, 0, 0,   24
    };
    Board board(state);
    assert(board.is_game_over());
    board.collect_remaining();
    assert(board.winner() == -1); // Draw
}

int main() {
    test_endgame_trigger();
    test_endgame_not_trigger();
    test_collect_remaining_and_winner();
    test_draw();
    std::cout << "test_endgame passed successfully!" << std::endl;
    return 0;
}
