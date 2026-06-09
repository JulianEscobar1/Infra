#pragma once

#include <array>
#include <vector>
#include <string>
#include <iostream>

// -------------------------------------------------------
//  Kalah(6,4) Board Representation
//
//  Index layout:
//
//              NORTH
//     12  11  10   9   8   7
// [13]                          [6]
//      0   1   2   3   4   5
//              SOUTH
//
//  0-5   = South pits
//  6     = South kalaha (store)
//  7-12  = North pits
//  13    = North kalaha (store)
// -------------------------------------------------------

enum class Side { SOUTH = 0, NORTH = 1 };

/// Return the opposing side
Side opposite_side(Side s);

/// Convert a string ("south"/"north") to Side
Side side_from_string(const std::string& s);

/// Convert Side to string
std::string side_to_string(Side s);

class Board {
public:
    static constexpr int NUM_PITS  = 6;
    static constexpr int INIT_SEEDS = 4;
    static constexpr int TOTAL_SLOTS = 14;

    std::array<int, TOTAL_SLOTS> pits{};

    /// Construct the initial Kalah(6,4) board
    Board();

    /// Construct from an explicit 14-element state
    explicit Board(const std::array<int, TOTAL_SLOTS>& state);

    // ---- Static helpers ----

    /// Kalaha (store) index for a given side
    static constexpr int kalaha(Side s) {
        return (s == Side::SOUTH) ? 6 : 13;
    }

    /// First pit index for a side (inclusive)
    static constexpr int pit_start(Side s) {
        return (s == Side::SOUTH) ? 0 : 7;
    }

    /// One-past-last pit index for a side (exclusive)
    static constexpr int pit_end(Side s) {
        return (s == Side::SOUTH) ? 6 : 13;
    }

    /// Opposite pit index (pit 0 ↔ 12, pit 1 ↔ 11, ..., pit 5 ↔ 7)
    static constexpr int opposite_pit(int pit) {
        return 12 - pit;
    }

    /// Does the given pit belong to the given side?
    static constexpr bool owns_pit(Side s, int pit) {
        if (s == Side::SOUTH) return pit >= 0 && pit <= 5;
        else                  return pit >= 7 && pit <= 12;
    }

    // ---- Game logic ----

    /// Return valid moves (pit indices with seeds > 0) for the given side
    std::vector<int> valid_moves(Side s) const;

    /// Sow seeds from the chosen pit.
    /// Returns true if the player earns an extra turn (last seed in own kalaha).
    bool sow(Side s, int pit);

    /// Is the game over? (one side has all pits empty)
    bool is_game_over() const;

    /// Collect remaining seeds into each player's kalaha (called at game end)
    void collect_remaining();

    /// Sum of seeds in the pits (excluding kalaha) for a side
    int side_seeds(Side s) const;

    /// Score = kalaha value for a side
    int score(Side s) const;

    /// Return the winning side, or -1 for a draw.
    /// Only meaningful after collect_remaining().
    int winner() const;

    /// Pretty-print the board
    void print(std::ostream& os = std::cout) const;

    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const;
};
