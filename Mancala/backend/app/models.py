from pydantic import BaseModel, Field, field_validator
from typing import List, Literal

class MoveRequest(BaseModel):
    board: List[int]
    side: Literal["south", "north", "SOUTH", "NORTH"]
    depth: int = Field(gt=0, description="Search depth must be greater than 0")
    threads: int = Field(gt=0, description="Threads count must be greater than 0")

    @field_validator("board")
    @classmethod
    def validate_board(cls, v: List[int]) -> List[int]:
        if len(v) != 14:
            raise ValueError("Board must contain exactly 14 integers")
        return v

    @field_validator("side")
    @classmethod
    def validate_side(cls, v: str) -> str:
        normalized = v.lower()
        if normalized not in ("south", "north"):
            raise ValueError("Side must be 'south' or 'north'")
        return normalized

class Stats(BaseModel):
    nodes: int
    prunes: int

class MoveResponse(BaseModel):
    move: int
    evaluation: int
    elapsed_ms: int
    stats: Stats
