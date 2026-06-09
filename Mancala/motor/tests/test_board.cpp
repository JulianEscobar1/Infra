#include "../src/board.h"
#include <cassert>
#include <iostream>

void test_initialization() {
    Board board;
    // Check initial seed count
    for (int i = 0; i < 6; ++i) {
        assert(board.pits[i] == 4);
        assert(board.pits[i + 7] == 4);
    }
    assert(board.pits[6] == 0); // South kalaha
    assert(board.pits[13] == 0); // North kalaha

    assert(board.side_seeds(Side::SOUTH) == 24);
    assert(board.side_seeds(Side::NORTH) == 24);
    assert(!board.is_game_over());
}

void test_pit_helpers() {
    assert(Board::opposite_pit(0) == 12);
    assert(Board::opposite_pit(5) == 7);
    assert(Board::opposite_pit(7) == 5);
    assert(Board::opposite_pit(12) == 0);

    assert(Board::owns_pit(Side::SOUTH, 0));
    assert(Board::owns_pit(Side::SOUTH, 5));
    assert(!Board::owns_pit(Side::SOUTH, 6));
    assert(!Board::owns_pit(Side::SOUTH, 7));

    assert(Board::owns_pit(Side::NORTH, 7));
    assert(Board::owns_pit(Side::NORTH, 12));
    assert(!Board::owns_pit(Side::NORTH, 13));
    assert(!Board::owns_pit(Side::NORTH, 0));
}

void test_valid_moves() {
    Board board;
    auto s_moves = board.valid_moves(Side::SOUTH);
    assert(s_moves.size() == 6);
    for (int i = 0; i < 6; ++i) {
        assert(s_moves[i] == i);
    }

    // Set one pit to empty
    board.pits[2] = 0;
    s_moves = board.valid_moves(Side::SOUTH);
    assert(s_moves.size() == 5);
    assert(s_moves[2] == 3); // Pit 2 skipped
}

int main() {
    test_initialization();
    test_pit_helpers();
    test_valid_moves();
    std::cout << "test_board passed successfully!" << std::endl;
    return 0;
}
