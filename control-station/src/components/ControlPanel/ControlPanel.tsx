import React from 'react';
import './ControlPanel.css';

function ControlPanel() {
  return (
    <footer className="status-bar">
      <button className="status-button run">Run</button>
      <button className="status-button stop">Stop</button>
      <button className="status-button halt">Halt</button>
      <button className="status-button load">Load</button>
    </footer>
  );
}

export default ControlPanel;