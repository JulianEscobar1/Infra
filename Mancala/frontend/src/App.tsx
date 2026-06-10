import React, { useState, useEffect } from 'react';
import { MancalaBoard } from './components/MancalaBoard';
import { computeMove, checkBackendReady, ApiError } from './services/api';
import { MoveResponse } from './types';

const INITIAL_BOARD = [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0];
const EMPTY_BOARD = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

export const App: React.FC = () => {
  const [board, setBoard] = useState<number[]>(INITIAL_BOARD);
  const [side, setSide] = useState<'south' | 'north'>('south');
  const [depth, setDepth] = useState<number>(8);
  const [threads, setThreads] = useState<number>(4);
  
  const [loading, setLoading] = useState<boolean>(false);
  const [result, setResult] = useState<MoveResponse | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [backendReady, setBackendReady] = useState<boolean | null>(null);

  // Check backend readiness on load
  useEffect(() => {
    checkHealth();
    const interval = setInterval(checkHealth, 10000); // Poll every 10s
    return () => clearInterval(interval);
  }, []);

  const checkHealth = async () => {
    const isReady = await checkBackendReady();
    setBackendReady(isReady);
  };

  const handleReset = () => {
    setBoard(INITIAL_BOARD);
    setResult(null);
    setError(null);
  };

  const handleClear = () => {
    setBoard(EMPTY_BOARD);
    setResult(null);
    setError(null);
  };

  const handleComputeMove = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError(null);
    setResult(null);

    try {
      const response = await computeMove({
        board,
        side,
        depth,
        threads,
      });
      setResult(response);
    } catch (err) {
      if (err instanceof ApiError) {
        setError(`[HTTP ${err.status}] ${err.message}: ${err.detail}`);
      } else {
        setError('Ocurrió un error inesperado al conectar.');
      }
    } finally {
      setLoading(false);
    }
  };

  return (
    <div style={{ maxWidth: '1200px', margin: '0 auto', padding: '24px', width: '100%', boxSizing: 'border-box' }}>
      {/* Header */}
      <header style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '32px', flexWrap: 'wrap', gap: '16px' }}>
        <div>
          <h1 style={{ margin: 0, fontSize: '2rem', fontWeight: 700, background: 'linear-gradient(135deg, var(--primary), var(--secondary))', WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent' }}>
            Mancala AI Engine
          </h1>
          <p style={{ margin: '4px 0 0 0', color: 'var(--text-secondary)', fontSize: '0.95rem' }}>
            Consola interactiva de cálculo algorítmico y paralelismo
          </p>
        </div>

        <div className="status-pill">
          <div className={`status-dot ${backendReady ? 'online' : 'offline'}`} />
          <span>Backend: {backendReady === null ? 'Pinging...' : backendReady ? 'Conectado' : 'Desconectado'}</span>
        </div>
      </header>

      {/* Main Grid */}
      <div style={{ display: 'grid', gridTemplateColumns: '1fr', gap: '24px', alignItems: 'start' }}>
        {/* Board Viewport */}
        <section className="glass-panel" style={{ padding: '24px', display: 'flex', flexDirection: 'column', gap: '20px' }}>
          <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', flexWrap: 'wrap', gap: '12px' }}>
            <h2 style={{ margin: 0, fontSize: '1.25rem', fontWeight: 600 }}>Estado del Tablero</h2>
            <div style={{ display: 'flex', gap: '8px' }}>
              <button onClick={handleReset} className="input-control" style={{ width: 'auto', padding: '6px 12px', cursor: 'pointer', fontSize: '0.85rem' }}>
                Reiniciar
              </button>
              <button onClick={handleClear} className="input-control" style={{ width: 'auto', padding: '6px 12px', cursor: 'pointer', fontSize: '0.85rem' }}>
                Vaciar
              </button>
            </div>
          </div>

          <MancalaBoard
            board={board}
            bestMove={result ? result.move : null}
            side={side}
            onBoardChange={setBoard}
          />

          <p style={{ margin: 0, fontSize: '0.85rem', color: 'var(--text-secondary)', textAlign: 'center' }}>
            💡 Haz clic en los hoyos para añadir semillas (+) o usa los controles inferiores para restar.
          </p>
        </section>

        {/* Configuration & Results */}
        <div style={{ display: 'grid', gridTemplateColumns: '1fr', gap: '24px' }}>
          {/* Form Config */}
          <section className="glass-panel" style={{ padding: '24px' }}>
            <h2 style={{ margin: '0 0 20px 0', fontSize: '1.25rem', fontWeight: 600 }}>Parámetros de Búsqueda</h2>
            
            <form onSubmit={handleComputeMove}>
              <div style={{ display: 'grid', gridTemplateColumns: '1fr md', gap: '16px' }}>
                
                {/* Side Selection */}
                <div className="config-group">
                  <label>Lado actual (Maximiza)</label>
                  <div className="radio-tabs">
                    <div
                      className={`radio-tab ${side === 'south' ? 'active south' : ''}`}
                      onClick={() => setSide('south')}
                    >
                      Sur
                    </div>
                    <div
                      className={`radio-tab ${side === 'north' ? 'active north' : ''}`}
                      onClick={() => setSide('north')}
                    >
                      Norte
                    </div>
                  </div>
                </div>

                {/* Depth Selection */}
                <div className="config-group">
                  <label>Profundidad de Búsqueda (Depth)</label>
                  <input
                    type="number"
                    min="1"
                    max="15"
                    className="input-control"
                    value={depth}
                    onChange={(e) => setDepth(parseInt(e.target.value) || 1)}
                    required
                  />
                </div>

                {/* Threads Selection */}
                <div className="config-group">
                  <label>Hilos OpenMP (Threads)</label>
                  <input
                    type="number"
                    min="1"
                    max="16"
                    className="input-control"
                    value={threads}
                    onChange={(e) => setThreads(parseInt(e.target.value) || 1)}
                    required
                  />
                </div>

              </div>

              <div style={{ marginTop: '20px' }}>
                <button
                  type="submit"
                  className="btn-action"
                  disabled={loading || backendReady === false}
                >
                  {loading ? 'Calculando Jugada Óptima...' : 'Calcular Jugada'}
                </button>
              </div>
            </form>
          </section>

          {/* Errors Board */}
          {error && (
            <div className="alert alert-danger">
              <h4 style={{ margin: '0 0 4px 0', fontWeight: 600 }}>Error en la Operación</h4>
              <p style={{ margin: 0 }}>{error}</p>
            </div>
          )}

          {/* Results Board */}
          {result && (
            <section className="glass-panel" style={{ padding: '24px' }}>
              <h2 style={{ margin: '0 0 20px 0', fontSize: '1.25rem', fontWeight: 600, color: 'var(--success)' }}>
                Jugada Calculada con Éxito
              </h2>
              
              <div className="results-grid">
                
                <div className="metric-card" style={{ gridColumn: 'span 2', borderLeft: '4px solid var(--success)' }}>
                  <span className="title">Mejor Movimiento Recomendado</span>
                  <span className="value" style={{ fontSize: '1.75rem', color: 'var(--success)' }}>
                    Fosa {result.move} (Semillas: {board[result.move]})
                  </span>
                </div>

                <div className="metric-card">
                  <span className="title">Evaluación de Estado</span>
                  <span className="value">{result.evaluation}</span>
                </div>

                <div className="metric-card">
                  <span className="title">Tiempo de Cómputo</span>
                  <span className="value">{result.elapsed_ms} ms</span>
                </div>

                <div className="metric-card">
                  <span className="title">Nodos Explorados</span>
                  <span className="value">{result.stats.nodes.toLocaleString()}</span>
                </div>

                <div className="metric-card">
                  <span className="title">Podas Realizadas</span>
                  <span className="value">{result.stats.prunes.toLocaleString()}</span>
                </div>

              </div>
            </section>
          )}
        </div>
      </div>
    </div>
  );
};
