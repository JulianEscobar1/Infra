#include "board.h"
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// -------------------------------------------------------
//  Side helpers
// -------------------------------------------------------

Side opposite_side(Side s) {
    return (s == Side::SOUTH) ? Side::NORTH : Side::SOUTH;
}

Side side_from_string(const std::string& s) {
    if (s == "north" || s == "NORTH" || s == "North") return Side::NORTH;
    return Side::SOUTH;
}

std::string side_to_string(Side s) {
    return (s == Side::SOUTH) ? "south" : "north";
}

// -------------------------------------------------------
//  Constructors
// -------------------------------------------------------

Board::Board() {
    pits.fill(0);
    for (int i = 0; i < NUM_PITS; ++i) {
        pits[i]     = INIT_SEEDS; // South pits 0-5
        pits[i + 7] = INIT_SEEDS; // North pits 7-12
    }
    // pits[6] = 0 (South kalaha), pits[13] = 0 (North kalaha)
}

Board::Board(const std::array<int, TOTAL_SLOTS>& state)
    : pits(state) {}

// -------------------------------------------------------
//  Valid moves
// -------------------------------------------------------

std::vector<int> Board::valid_moves(Side s) const {
    std::vector<int> moves;
    moves.reserve(NUM_PITS);
    int start = pit_start(s);
    int end   = pit_end(s);
    for (int i = start; i < end; ++i) {
        if (pits[i] > 0) {
            moves.push_back(i);
        }
    }
    return moves;
}

// -------------------------------------------------------
//  Sow
// -------------------------------------------------------

bool Board::sow(Side s, int pit) {
    int seeds = pits[pit];
    pits[pit] = 0;

    // The kalaha to skip (opponent's store)
    int skip = kalaha(opposite_side(s));
    int pos = pit;

    for (int i = 0; i < seeds; ++i) {
        pos = (pos + 1) % TOTAL_SLOTS;
        if (pos == skip) {
            pos = (pos + 1) % TOTAL_SLOTS;
        }
        pits[pos]++;
    }

    // Extra turn: last seed fell in own kalaha
    if (pos == kalaha(s)) {
        return true;
    }

    // Capture: last seed fell in an own empty pit (now == 1) and opposite has seeds
    if (owns_pit(s, pos) && pits[pos] == 1) {
        int opp = opposite_pit(pos);
        if (pits[opp] > 0) {
            pits[kalaha(s)] += pits[opp] + 1; // captured seeds + the landing seed
            pits[opp] = 0;
            pits[pos] = 0;
        }
    }

    return false;
}

// -------------------------------------------------------
//  Game-over detection
// -------------------------------------------------------

bool Board::is_game_over() const {
    bool south_empty = true;
    for (int i = 0; i < NUM_PITS; ++i) {
        if (pits[i] > 0) { south_empty = false; break; }
    }
    if (south_empty) return true;

    bool north_empty = true;
    for (int i = 7; i < 7 + NUM_PITS; ++i) {
        if (pits[i] > 0) { north_empty = false; break; }
    }
    return north_empty;
}

// -------------------------------------------------------
//  Collect remaining seeds
// -------------------------------------------------------

void Board::collect_remaining() {
    for (int i = 0; i < NUM_PITS; ++i) {
        pits[kalaha(Side::SOUTH)] += pits[i];
        pits[i] = 0;
    }
    for (int i = 7; i < 7 + NUM_PITS; ++i) {
        pits[kalaha(Side::NORTH)] += pits[i];
        pits[i] = 0;
    }
}

// -------------------------------------------------------
//  Queries
// -------------------------------------------------------

int Board::side_seeds(Side s) const {
    int sum = 0;
    int start = pit_start(s);
    int end   = pit_end(s);
    for (int i = start; i < end; ++i) {
        sum += pits[i];
    }
    return sum;
}

int Board::score(Side s) const {
    return pits[kalaha(s)];
}

int Board::winner() const {
    int s_score = pits[kalaha(Side::SOUTH)];
    int n_score = pits[kalaha(Side::NORTH)];
    if (s_score > n_score) return 0;  // South wins
    if (n_score > s_score) return 1;  // North wins
    return -1;                         // Draw
}

// -------------------------------------------------------
//  Operators
// -------------------------------------------------------

bool Board::operator==(const Board& other) const {
    return pits == other.pits;
}

bool Board::operator!=(const Board& other) const {
    return pits != other.pits;
}

// -------------------------------------------------------
//  Pretty print
// -------------------------------------------------------

void Board::print(std::ostream& os) const {
    os << "\n              NORTH\n";
    os << "    ";
    // North pits printed right-to-left (12, 11, 10, 9, 8, 7)
    for (int i = 12; i >= 7; --i) {
        os << std::setw(3) << pits[i] << " ";
    }
    os << "\n";
    os << std::setw(3) << pits[13] << "                          " << std::setw(3) << pits[6] << "\n";
    os << "    ";
    // South pits left-to-right (0, 1, 2, 3, 4, 5)
    for (int i = 0; i < 6; ++i) {
        os << std::setw(3) << pits[i] << " ";
    }
    os << "\n              SOUTH\n\n";
}
