import React from 'react';

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
  const updatePit = (index: number, delta: number) => {
    const newBoard = [...board];
    const newCount = Math.max(0, newBoard[index] + delta);
    newBoard[index] = newCount;
    onBoardChange(newBoard);
  };

  const renderSeedsPreview = (count: number) => {
    // Generate up to 12 dot previews to avoid overflow clutter
    const dotsCount = Math.min(12, count);
    return (
      <div className="seeds-preview">
        {Array.from({ length: dotsCount }).map((_, i) => (
          <div key={i} className="seed-dot" />
        ))}
      </div>
    );
  };

  // Indices for North pits in visual order (12 down to 7)
  const northPitIndices = [12, 11, 10, 9, 8, 7];
  // Indices for South pits in visual order (0 up to 5)
  const southPitIndices = [0, 1, 2, 3, 4, 5];

  return (
    <div className="mancala-board-container">
      <div className="glass-panel mancala-board">
        {/* North Store (Kalaha 13) */}
        <div className="store-container">
          <div className="store north">
            <span style={{ fontSize: '1.5rem', fontWeight: 700 }}>{board[13]}</span>
            <span className="pit-index-label">Kalaha N (13)</span>
            {renderSeedsPreview(board[13])}
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
                    <button className="pit-btn" onClick={() => updatePit(idx, 1)}>+</button>
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
                    <button className="pit-btn" onClick={() => updatePit(idx, 1)}>+</button>
                  </div>
                </div>
              );
            })}
          </div>
        </div>

        {/* South Store (Kalaha 6) */}
        <div className="store-container">
          <div className="store south">
            <span style={{ fontSize: '1.5rem', fontWeight: 700 }}>{board[6]}</span>
            <span className="pit-index-label">Kalaha S (6)</span>
            {renderSeedsPreview(board[6])}
          </div>
        </div>
      </div>
    </div>
  );
};
