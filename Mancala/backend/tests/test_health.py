from fastapi.testclient import TestClient
from unittest.mock import AsyncMock, patch
from app.main import app

client = TestClient(app)

def test_healthz():
    response = client.get("/healthz")
    assert response.status_code == 200
    assert response.json() == {"status": "ok"}

@patch("app.motor_client.motor_client.is_healthy", new_callable=AsyncMock)
def test_readyz_healthy(mock_is_healthy):
    mock_is_healthy.return_value = True
    response = client.get("/readyz")
    assert response.status_code == 200
    assert response.json() == {"status": "ready"}

@patch("app.motor_client.motor_client.is_healthy", new_callable=AsyncMock)
def test_readyz_unhealthy(mock_is_healthy):
    mock_is_healthy.return_value = False
    response = client.get("/readyz")
    assert response.status_code == 503
    assert "unreachable" in response.json()["detail"]
