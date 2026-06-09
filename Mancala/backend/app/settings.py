import os
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    # Base URL for the C++ Mancala engine
    ENGINE_URL: str = os.getenv("ENGINE_URL", "http://localhost:8080")
    
    # Allowed CORS origins, comma-separated list
    ALLOWED_ORIGINS_STR: str = os.getenv("ALLOWED_ORIGINS", "http://localhost:3000,http://127.0.0.1:3000")

    @property
    def allowed_origins(self) -> list[str]:
        return [origin.strip() for origin in self.ALLOWED_ORIGINS_STR.split(",") if origin.strip()]

settings = Settings()
