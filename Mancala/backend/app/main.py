from fastapi import FastAPI, HTTPException, status
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager
import httpx

from app.settings import settings
from app.models import MoveRequest, MoveResponse
from app.motor_client import motor_client
from app.metrics import MetricsMiddleware, get_metrics_response, ENGINE_ERRORS_TOTAL

@asynccontextmanager
async def lifespan(app: FastAPI):
    # Setup step
    yield
    # Graceful shutdown step
    await motor_client.close()

app = FastAPI(
    title="Mancala Backend Facade",
    description="HTTP Gateway to C++ Mancala Game Engine",
    version="1.0.0",
    lifespan=lifespan
)

# Custom metrics middleware
app.add_middleware(MetricsMiddleware)

# CORS configuration
app.add_middleware(
    CORSMiddleware,
    allow_origins=settings.allowed_origins,
    allow_credentials=True,
    allow_methods=["GET", "POST", "OPTIONS"],
    allow_headers=["*"],
)

@app.get("/healthz", status_code=status.HTTP_200_OK)
async def healthz():
    """Liveness probe: verifies that the backend application is running."""
    return {"status": "ok"}

@app.get("/readyz", status_code=status.HTTP_200_OK)
async def readyz():
    """Readiness probe: verifies that the C++ motor engine is reachable."""
    is_engine_up = await motor_client.is_healthy()
    if not is_engine_up:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="C++ Mancala Engine is currently unreachable"
        )
    return {"status": "ready"}

@app.get("/metrics")
async def metrics():
    """Exposes Prometheus system and application metrics."""
    return get_metrics_response()

@app.post("/move", response_model=MoveResponse)
async def calculate_move(request: MoveRequest):
    """Facade proxy endpoint that forwards validation-cleaned move requests to the C++ engine."""
    try:
        result = await motor_client.call_move(request)
        return result
    except (httpx.ConnectError, httpx.ConnectTimeout, httpx.NetworkError) as network_exc:
        # Engine is unreachable
        ENGINE_ERRORS_TOTAL.labels(endpoint="/move").inc()
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail=f"Engine service is down: {str(network_exc)}"
        )
    except httpx.HTTPStatusError as http_exc:
        # Engine returned non-2xx response
        ENGINE_ERRORS_TOTAL.labels(endpoint="/move").inc()
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=f"Engine returned error: {http_exc.response.text}"
        )
    except Exception as exc:
        # Internal unexpected error
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail=f"Internal Server Error: {str(exc)}"
        )
