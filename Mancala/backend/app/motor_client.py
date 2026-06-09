import httpx
from app.settings import settings
from app.models import MoveRequest

class MotorClient:
    def __init__(self, base_url: str = settings.ENGINE_URL):
        self.base_url = base_url.rstrip("/")
        # We use a persistent async client
        self.client = httpx.AsyncClient(timeout=10.0)

    async def close(self):
        await self.client.aclose()

    async def call_move(self, request_data: MoveRequest) -> dict:
        url = f"{self.base_url}/move"
        # Serialize payload matching C++ expectations
        payload = {
            "board": request_data.board,
            "side": request_data.side,
            "depth": request_data.depth,
            "threads": request_data.threads
        }
        
        response = await self.client.post(url, json=payload)
        response.raise_for_status()
        return response.json()

    async def is_healthy(self) -> bool:
        url = f"{self.base_url}/healthz"
        try:
            response = await self.client.get(url, timeout=2.0)
            return response.status_code == 200
        except httpx.HTTPError:
            return False

# Singleton instance of client
motor_client = MotorClient()
