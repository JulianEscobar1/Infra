from fastapi.testclient import TestClient
from unittest.mock import AsyncMock, patch
import httpx
from app.main import app

client = TestClient(app)

@patch("app.motor_client.motor_client.call_move", new_callable=AsyncMock)
def test_move_happy_path(mock_call_move):
    mock_call_move.return_value = {
        "move": 3,
        "evaluation": 7,
        "elapsed_ms": 120,
        "stats": {
            "nodes": 150000,
            "prunes": 25000
        }
    }

    payload = {
        "board": [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],
        "side": "south",
        "depth": 8,
        "threads": 4
    }

    response = client.post("/move", json=payload)
    assert response.status_code == 200
    data = response.json()
    assert data["move"] == 3
    assert data["evaluation"] == 7
    assert data["elapsed_ms"] == 120
    assert data["stats"]["nodes"] == 150000
    assert data["stats"]["prunes"] == 25000

@patch("app.motor_client.motor_client.call_move", new_callable=AsyncMock)
def test_move_engine_down(mock_call_move):
    # Simulate a network/connection error to the engine
    mock_call_move.side_effect = httpx.ConnectError("Connection refused")

    payload = {
        "board": [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],
        "side": "north",
        "depth": 6,
        "threads": 2
    }

    response = client.post("/move", json=payload)
    assert response.status_code == 503
    assert "Engine service is down" in response.json()["detail"]
