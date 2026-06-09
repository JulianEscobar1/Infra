#include "../src/board.h"
#include <cassert>
#include <iostream>

void test_successful_capture() {
    // Set up a custom state:
    // South pit 1 has 1 seed, pit 2 is empty.
    // Opposing North pit 10 (opposite of 2) has 5 seeds.
    // The sow starts from South pit 1 (contains 1 seed).
    // The seed should land in South pit 2 (which was empty, belongs to South).
    // North pit 10 should be captured (5 seeds + 1 landing seed = 6 seeds total captured to South kalaha [6]).
    std::array<int, 14> state = {
        0, 1, 0, 0, 0, 0,   0, // South pits 0-5, Kalaha 6
        0, 0, 0, 5, 0, 0,   0  // North pits 7-12, Kalaha 13 (pit 10 has 5 seeds)
    };
    Board board(state);

    // Sow from pit 1
    bool extra = board.sow(Side::SOUTH, 1);

    assert(!extra); // No extra turn since it lands in pit 2, not kalaha
    assert(board.pits[1] == 0); // Pit 1 emptied
    assert(board.pits[2] == 0); // Pit 2 was empty, seed landed and then got captured, so it's empty now
    assert(board.pits[10] == 0); // Opposite pit 10 cleared
    assert(board.pits[6] == 6); // 5 from pit 10 + 1 landing seed = 6 in South Kalaha
}

void test_no_capture_when_opposite_empty() {
    // South pit 1 has 1 seed, pit 2 empty. Opposite North pit 10 is empty.
    std::array<int, 14> state = {
        0, 1, 0, 0, 0, 0,   0,
        0, 0, 0, 0, 0, 0,   0
    };
    Board board(state);

    board.sow(Side::SOUTH, 1);

    assert(board.pits[1] == 0);
    assert(board.pits[2] == 1); // Landed but no capture because opposite is empty
    assert(board.pits[6] == 0); // No points in Kalaha
}

void test_no_capture_on_opponent_side() {
    // South pit 0 has 10 seeds (will sow into North side).
    // We want to verify that landing in an empty North pit does NOT trigger a capture for South,
    // and also that if South plays, it doesn't capture North's seeds incorrectly.
    // Actually, capture only happens if the landing pit is owned by the player who moved.
    std::array<int, 14> state = {
        0, 0, 0, 0, 0, 1,   0, // South
        0, 0, 0, 0, 0, 0,   0  // North: pit 12 is empty, pit 0 (opposite of 12) is empty anyway
    };
    Board board(state);
    // Sow from 5. Landing will be in South kalaha (6). That's an extra turn.
    // Let's test a case where it lands in North empty pit.
    // South plays pit 5 (with 2 seeds).
    // Sow 5 -> lands in 6 (South Kalaha), then 7 (North pit).
    // Let's initialize North pit 7 as empty, South pit 5 has 2 seeds.
    // Landings: 6 (kalaha), 7 (North empty pit).
    // Since 7 is on North side, South does not capture.
    state = {
        0, 0, 0, 0, 0, 2,   0,
        0, 0, 0, 0, 0, 0,   0
    };
    Board board2(state);
    board2.sow(Side::SOUTH, 5);
    assert(board2.pits[5] == 0);
    assert(board2.pits[6] == 1); // 1 seed in Kalaha
    assert(board2.pits[7] == 1); // 1 seed in North pit 7. It shouldn't be captured because South doesn't own North's pits.
}

int main() {
    test_successful_capture();
    test_no_capture_when_opposite_empty();
    test_no_capture_on_opponent_side();
    std::cout << "test_capture passed successfully!" << std::endl;
    return 0;
}
