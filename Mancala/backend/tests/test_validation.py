from fastapi.testclient import TestClient
from app.main import app

client = TestClient(app)

def test_validation_board_length():
    payload = {
        "board": [4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],  # 13 elements
        "side": "south",
        "depth": 8,
        "threads": 4
    }
    response = client.post("/move", json=payload)
    assert response.status_code == 422
    assert "board" in response.text

def test_validation_depth_zero():
    payload = {
        "board": [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],
        "side": "south",
        "depth": 0,  # <= 0
        "threads": 4
    }
    response = client.post("/move", json=payload)
    assert response.status_code == 422
    assert "depth" in response.text

def test_validation_threads_negative():
    payload = {
        "board": [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],
        "side": "south",
        "depth": 8,
        "threads": -1  # <= 0
    }
    response = client.post("/move", json=payload)
    assert response.status_code == 422
    assert "threads" in response.text

def test_validation_invalid_side():
    payload = {
        "board": [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],
        "side": "east",  # invalid side
        "depth": 8,
        "threads": 4
    }
    response = client.post("/move", json=payload)
    assert response.status_code == 422
    assert "side" in response.text

def test_validation_side_casing_normalization():
    # Capitalized input sides should normalize to lower case, matchingLiteral check
    # Let's verify that a valid uppercase side is accepted (FastAPI should validate & normalize)
    # Actually, we mock client.call_move to verify normalization.
    # In test_move.py we verify correct processing. Here we just ensure 200 is returned for normalized casing.
    from unittest.mock import AsyncMock, patch
    with patch("app.motor_client.motor_client.call_move", new_callable=AsyncMock) as mock_call_move:
        mock_call_move.return_value = {
            "move": 3,
            "evaluation": 7,
            "elapsed_ms": 12,
            "stats": {"nodes": 10, "prunes": 0}
        }
        payload = {
            "board": [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0],
            "side": "SOUTH",
            "depth": 8,
            "threads": 4
        }
        response = client.post("/move", json=payload)
        assert response.status_code == 200
        # Check that normalized side "south" was passed to the client
        mock_call_move.assert_called_once()
        called_args = mock_call_move.call_args[0][0]
        assert called_args.side == "south"
