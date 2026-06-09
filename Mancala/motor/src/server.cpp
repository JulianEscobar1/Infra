#include "board.h"
#include "parallel.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <atomic>
#include <string>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

// Thread-safe metrics counters
static std::atomic<int64_t> g_total_requests{0};
static std::atomic<int64_t> g_total_nodes{0};
static std::atomic<int64_t> g_total_prunes{0};
static std::atomic<int64_t> g_total_elapsed_ms{0};

int run_server(int port, const std::string& host) {
    httplib::Server svr;

    // POST /move
    svr.Post("/move", [](const httplib::Request& req, httplib::Response& res) {
        g_total_requests++;
        try {
            auto body = json::parse(req.body);

            // Validation
            if (!body.contains("board") || !body.contains("side") || 
                !body.contains("depth") || !body.contains("threads")) {
                res.status = 400;
                res.set_content("{\"error\":\"Missing fields in request\"}", "application/json");
                return;
            }

            std::array<int, Board::TOTAL_SLOTS> board_state;
            auto board_arr = body["board"];
            if (board_arr.size() != Board::TOTAL_SLOTS) {
                res.status = 400;
                res.set_content("{\"error\":\"Board size must be 14\"}", "application/json");
                return;
            }

            for (size_t i = 0; i < Board::TOTAL_SLOTS; ++i) {
                board_state[i] = board_arr[i].get<int>();
            }

            std::string side_str = body["side"].get<std::string>();
            Side side = side_from_string(side_str);
            int depth = body["depth"].get<int>();
            int threads = body["threads"].get<int>();

            Board board(board_state);

            // Measure search time
            auto start_time = std::chrono::high_resolution_clock::now();
            
            // Execute parallel alpha-beta search
            SearchResult result = parallel_alphabeta_search(board, side, depth, threads);
            
            auto end_time = std::chrono::high_resolution_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

            // Update global metrics
            g_total_nodes += result.nodes;
            g_total_prunes += result.prunes;
            g_total_elapsed_ms += elapsed_ms;

            json response_json = {
                {"move", result.best_move},
                {"evaluation", result.evaluation},
                {"elapsed_ms", elapsed_ms},
                {"stats", {
                    {"nodes", result.nodes},
                    {"prunes", result.prunes}
                }}
            };

            res.status = 200;
            res.set_content(response_json.dump(), "application/json");

        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(std::string("{\"error\":\"Internal Server Error: ") + e.what() + "\"}", "application/json");
        }
    });

    // GET /healthz
    svr.Get("/healthz", [](const httplib::Request&, httplib::Response& res) {
        res.status = 200;
        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    // GET /readyz
    svr.Get("/readyz", [](const httplib::Request&, httplib::Response& res) {
        // Since we are stateless, if healthz is fine, readyz is fine.
        res.status = 200;
        res.set_content("{\"ready\":true}", "application/json");
    });

    // GET /metrics
    svr.Get("/metrics", [](const httplib::Request&, httplib::Response& res) {
        std::stringstream ss;
        ss << "# HELP mancala_requests_total Total number of moves calculated\n"
           << "# TYPE mancala_requests_total counter\n"
           << "mancala_requests_total " << g_total_requests.load() << "\n"
           << "# HELP mancala_nodes_total Total number of minimax nodes visited\n"
           << "# TYPE mancala_nodes_total counter\n"
           << "mancala_nodes_total " << g_total_nodes.load() << "\n"
           << "# HELP mancala_prunes_total Total number of prunes made\n"
           << "# TYPE mancala_prunes_total counter\n"
           << "mancala_prunes_total " << g_total_prunes.load() << "\n"
           << "# HELP mancala_elapsed_ms_total Total time spent searching in milliseconds\n"
           << "# TYPE mancala_elapsed_ms_total counter\n"
           << "mancala_elapsed_ms_total " << g_total_elapsed_ms.load() << "\n";
        
        res.status = 200;
        res.set_content(ss.str(), "text/plain");
    });

    std::cout << "Starting C++ Mancala Engine HTTP Server on " << host << ":" << port << std::endl;
    if (!svr.listen(host.c_str(), port)) {
        std::cerr << "Failed to start server on port " << port << std::endl;
        return 1;
    }
    return 0;
}
