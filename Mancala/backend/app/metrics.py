import time
from fastapi import Request
from prometheus_client import Counter, Histogram, generate_latest, CONTENT_TYPE_LATEST
from starlette.middleware.base import BaseHTTPMiddleware
from starlette.responses import Response

# Prometheus Metrics Definitions
HTTP_REQUESTS_TOTAL = Counter(
    "http_requests_total",
    "Total HTTP Requests",
    ["method", "endpoint", "http_status"]
)

HTTP_REQUEST_DURATION_SECONDS = Histogram(
    "http_request_duration_seconds",
    "HTTP Request Latency in seconds",
    ["method", "endpoint"]
)

ENGINE_ERRORS_TOTAL = Counter(
    "engine_errors_total",
    "Total failures communicating with the C++ engine",
    ["endpoint"]
)

class MetricsMiddleware(BaseHTTPMiddleware):
    async def dispatch(self, request: Request, call_next):
        # Exclude metrics and health points from instrumentation to avoid pollution
        path = request.url.path
        if path in ("/metrics", "/healthz", "/readyz"):
            return await call_next(request)

        method = request.method
        start_time = time.time()
        
        try:
            response = await call_next(request)
            duration = time.time() - start_time
            HTTP_REQUESTS_TOTAL.labels(
                method=method,
                endpoint=path,
                http_status=response.status_code
            ).inc()
            HTTP_REQUEST_DURATION_SECONDS.labels(
                method=method,
                endpoint=path
            ).observe(duration)
            return response
        except Exception as e:
            duration = time.time() - start_time
            HTTP_REQUESTS_TOTAL.labels(
                method=method,
                endpoint=path,
                http_status=500
            ).inc()
            HTTP_REQUEST_DURATION_SECONDS.labels(
                method=method,
                endpoint=path
            ).observe(duration)
            raise e

def get_metrics_response() -> Response:
    return Response(
        content=generate_latest(),
        media_type=CONTENT_TYPE_LATEST
    )
