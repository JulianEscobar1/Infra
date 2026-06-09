export interface MoveRequest {
  board: number[];
  side: 'south' | 'north';
  depth: number;
  threads: number;
}

export interface SearchStats {
  nodes: number;
  prunes: number;
}

export interface MoveResponse {
  move: number;
  evaluation: number;
  elapsed_ms: number;
  stats: SearchStats;
}

export interface ApiErrorResponse {
  detail: string | { loc: string[]; msg: string; type: string }[];
}
