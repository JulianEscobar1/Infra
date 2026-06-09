import { MoveRequest, MoveResponse } from '../types';

// Detect default backend URL depending on production/development context
// In dev (port 3000), Vite proxies /api to port 8000
// In production, Nginx proxies /api to backend service, or we read from an env/config
const API_BASE = '/api';

export class ApiError extends Error {
  status: number;
  detail: string;

  constructor(status: number, message: string, detail: string) {
    super(message);
    this.name = 'ApiError';
    this.status = status;
    this.detail = detail;
  }
}

export async function computeMove(request: MoveRequest): Promise<MoveResponse> {
  const url = `${API_BASE}/move`;
  
  try {
    const response = await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(request),
    });

    if (!response.ok) {
      let errorDetail = 'Ocurrió un error inesperado';
      try {
        const errorJson = await response.json();
        if (typeof errorJson.detail === 'string') {
          errorDetail = errorJson.detail;
        } else if (Array.isArray(errorJson.detail)) {
          errorDetail = errorJson.detail
            .map((err: any) => `${err.loc.join('.')}: ${err.msg}`)
            .join(' | ');
        }
      } catch {
        errorDetail = await response.text() || response.statusText;
      }
      
      throw new ApiError(response.status, `Error ${response.status}`, errorDetail);
    }

    return await response.json() as MoveResponse;
  } catch (error) {
    if (error instanceof ApiError) {
      throw error;
    }
    // Network errors
    throw new ApiError(503, 'Error de red', 'No se pudo contactar con el backend. Verifica tu conexión.');
  }
}

export async function checkBackendReady(): Promise<boolean> {
  const url = `${API_BASE}/readyz`;
  try {
    const response = await fetch(url);
    return response.ok;
  } catch {
    return false;
  }
}
