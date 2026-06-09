# ==========================================
# Dockerfile: Mancala Backend HTTP Facade
# ==========================================
FROM python:3.12-slim

# Prevent Python from writing .pyc files and buffering stdout/stderr
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# Copy dependency requirements
COPY requirements.txt .

# Install dependencies without package manager cache
RUN pip install --no-cache-dir -r requirements.txt

# Copy application code and test suite
COPY app/ app/
COPY tests/ tests/

# Expose default HTTP Port
EXPOSE 8000

# Environment variables with sensible defaults
ENV PORT=8000
ENV HOST=0.0.0.0
ENV ENGINE_URL=http://localhost:8080
ENV ALLOWED_ORIGINS=http://localhost:3000,http://127.0.0.1:3000

# Health check using native Python to check the liveness endpoint
HEALTHCHECK --interval=30s --timeout=5s --start-period=5s --retries=3 \
  CMD python -c "import urllib.request; urllib.request.urlopen('http://localhost:8000/healthz')" || exit 1

# Launch the FastAPI application using Uvicorn
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000"]
