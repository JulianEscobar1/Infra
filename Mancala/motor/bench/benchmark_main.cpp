#include "../src/board.h"
#include "../src/parallel.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

void print_usage() {
    std::cout << "Usage: benchmark_main --depth <depth> --threads <threads>\n"
              << "Arguments:\n"
              << "  --depth <d>    Search depth (default: 8)\n"
              << "  --threads <t>  Number of parallel threads (default: 4)\n";
}

int main(int argc, char* argv[]) {
    int depth = 8;
    int threads = 4;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--depth" && i + 1 < argc) {
            depth = std::stoi(argv[++i]);
        } else if (arg == "--threads" && i + 1 < argc) {
            threads = std::stoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            print_usage();
            return 0;
        }
    }

    // Set up a standard mid-game board or starting board
    // Starting board for Kalah(6,4):
    // 4 4 4 4 4 4 
    // 0         0
    // 4 4 4 4 4 4
    Board board; 

    // Run baseline with 1 thread
    auto start_1 = std::chrono::high_resolution_clock::now();
    SearchResult res_1 = parallel_alphabeta_search(board, Side::SOUTH, depth, 1);
    auto end_1 = std::chrono::high_resolution_clock::now();
    double time_ms_1 = std::chrono::duration<double, std::milli>(end_1 - start_1).count();

    // Run with target threads
    auto start_n = std::chrono::high_resolution_clock::now();
    SearchResult res_n = parallel_alphabeta_search(board, Side::SOUTH, depth, threads);
    auto end_n = std::chrono::high_resolution_clock::now();
    double time_ms_n = std::chrono::duration<double, std::milli>(end_n - start_n).count();

    double speedup = time_ms_1 / (time_ms_n > 0.0 ? time_ms_n : 1.0);
    double efficiency = speedup / threads;

    // Output header and CSV row
    std::cout << "depth,threads,time_ms,nodes,prunes,speedup,efficiency\n";
    std::cout << depth << ","
              << threads << ","
              << std::fixed << std::setprecision(2) << time_ms_n << ","
              << res_n.nodes << ","
              << res_n.prunes << ","
              << speedup << ","
              << efficiency << "\n";

    return 0;
}
