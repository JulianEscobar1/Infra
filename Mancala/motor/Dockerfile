# ==========================================
# Stage 1: Build Environment
# ==========================================
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies needed for compiling C++ and fetching third-party packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

# Copy the source tree into the docker build context
COPY . .

# Configure and compile using CMake
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --config Release -j$(nproc)

# Run tests to guarantee build correctness before shipping
RUN cd build && ctest --output-on-failure

# ==========================================
# Stage 2: Minimal Runtime Environment
# ==========================================
FROM ubuntu:22.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install curl for health checks, and libgomp1 for OpenMP support
RUN apt-get update && apt-get install -y \
    curl \
    libgomp1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the built executables from the builder stage
COPY --from=builder /build/build/mancala_engine .
COPY --from=builder /build/build/benchmark_main .

# Expose HTTP port
EXPOSE 8080

# Environment variables
ENV PORT=8080
ENV HOST=0.0.0.0

# Set up health check
HEALTHCHECK --interval=30s --timeout=5s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:8080/healthz || exit 1

# Launch the engine HTTP server
ENTRYPOINT ["./mancala_engine"]
