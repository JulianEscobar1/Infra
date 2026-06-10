import React from 'react';

// Kalah(6,4) starts with 48 seeds total (6 pits × 4 seeds × 2 sides)
const MAX_TOTAL_SEEDS = 48;

// South player owns pits 0-5 and kalaha 6
// North player owns pits 7-12 and kalaha 13
const SOUTH_INDICES = [0, 1, 2, 3, 4, 5, 6];
const NORTH_INDICES = [7, 8, 9, 10, 11, 12, 13];

interface MancalaBoardProps {
  board: number[];
  bestMove: number | null;
  side: 'south' | 'north';
  onBoardChange: (newBoard: number[]) => void;
}

export const MancalaBoard: React.FC<MancalaBoardProps> = ({
  board,
  bestMove,
  onBoardChange,
}) => {
  const getTotalSeeds = (b: number[]) => b.reduce((sum, v) => sum + v, 0);
  const getSideTotal  = (indices: number[], b: number[]) =>
    indices.reduce((sum, i) => sum + b[i], 0);

  const updatePit = (index: number, delta: number) => {
    const newBoard = [...board];
    const newCount = Math.max(0, newBoard[index] + delta);

    // Block addition only when the whole board already has 48 seeds
    if (delta > 0 && getTotalSeeds(newBoard) >= MAX_TOTAL_SEEDS) {
      return;
    }

    newBoard[index] = newCount;
    onBoardChange(newBoard);
  };

  const renderSeedsPreview = (count: number) => {
    const dotsCount = Math.min(12, count);
    return (
      <div className="seeds-preview">
        {Array.from({ length: dotsCount }).map((_, i) => (
          <div key={i} className="seed-dot" />
        ))}
      </div>
    );
  };

  // Seed totals for display
  const totalSeeds = getTotalSeeds(board);
  const southTotal = getSideTotal(SOUTH_INDICES, board);
  const northTotal = getSideTotal(NORTH_INDICES, board);
  const boardFull  = totalSeeds >= MAX_TOTAL_SEEDS;

  const northPitIndices = [12, 11, 10, 9, 8, 7];
  const southPitIndices = [0, 1, 2, 3, 4, 5];

  return (
    <div className="mancala-board-container">
      {/* Seed counters */}
      <div className="seeds-counters">
        <span className="seeds-counter north-counter">
          Norte: <strong>{northTotal}</strong>
        </span>
        <span className={`seeds-counter ${boardFull ? 'counter-full' : 'counter-ok'}`}>
          Total: <strong>{totalSeeds}</strong> / {MAX_TOTAL_SEEDS}
        </span>
        <span className="seeds-counter south-counter">
          Sur: <strong>{southTotal}</strong>
        </span>
      </div>

      <div className="glass-panel mancala-board">
        {/* North Store (Kalaha 13) */}
        <div className="store-container">
          <div
            className="store north"
            onClick={() => updatePit(13, 1)}
          >
            <span style={{ fontSize: '1.5rem', fontWeight: 700 }}>{board[13]}</span>
            <span className="pit-index-label">Kalaha N (13)</span>
            {renderSeedsPreview(board[13])}
          </div>
          <div className="pit-controls" style={{ marginTop: '8px' }}>
            <button className="pit-btn" onClick={() => updatePit(13, -1)}>-</button>
            <span className="pit-index-label">K13</span>
            <button
              className="pit-btn"
              onClick={() => updatePit(13, 1)}
              disabled={boardFull}
            >+</button>
          </div>
        </div>

        {/* Central Pits */}
        <div className="pits-grid-container">
          {/* North Pits Row */}
          <div className="pits-row">
            {northPitIndices.map((idx) => {
              const isBest = bestMove === idx;
              return (
                <div key={idx} className="pit-wrapper">
                  <div
                    className={`pit north ${isBest ? 'best-move' : ''}`}
                    onClick={() => updatePit(idx, 1)}
                  >
                    {board[idx]}
                    {renderSeedsPreview(board[idx])}
                  </div>
                  <div className="pit-controls">
                    <button className="pit-btn" onClick={() => updatePit(idx, -1)}>-</button>
                    <span className="pit-index-label">P{idx}</span>
                    <button
                      className="pit-btn"
                      onClick={() => updatePit(idx, 1)}
                      disabled={boardFull}
                    >+</button>
                  </div>
                </div>
              );
            })}
          </div>

          {/* South Pits Row */}
          <div className="pits-row">
            {southPitIndices.map((idx) => {
              const isBest = bestMove === idx;
              return (
                <div key={idx} className="pit-wrapper">
                  <div
                    className={`pit south ${isBest ? 'best-move' : ''}`}
                    onClick={() => updatePit(idx, 1)}
                  >
                    {board[idx]}
                    {renderSeedsPreview(board[idx])}
                  </div>
                  <div className="pit-controls">
                    <button className="pit-btn" onClick={() => updatePit(idx, -1)}>-</button>
                    <span className="pit-index-label">P{idx}</span>
                    <button
                      className="pit-btn"
                      onClick={() => updatePit(idx, 1)}
                      disabled={boardFull}
                    >+</button>
                  </div>
                </div>
              );
            })}
          </div>
        </div>

        {/* South Store (Kalaha 6) */}
        <div className="store-container">
          <div
            className="store south"
            onClick={() => updatePit(6, 1)}
          >
            <span style={{ fontSize: '1.5rem', fontWeight: 700 }}>{board[6]}</span>
            <span className="pit-index-label">Kalaha S (6)</span>
            {renderSeedsPreview(board[6])}
          </div>
          <div className="pit-controls" style={{ marginTop: '8px' }}>
            <button className="pit-btn" onClick={() => updatePit(6, -1)}>-</button>
            <span className="pit-index-label">K6</span>
            <button
              className="pit-btn"
              onClick={() => updatePit(6, 1)}
              disabled={boardFull}
            >+</button>
          </div>
        </div>
      </div>
    </div>
  );
};
