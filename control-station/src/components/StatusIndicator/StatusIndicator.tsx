import React from 'react';
import './StatusIndicator.css';

type StatusType = 'green' | 'yellow' | 'red';

interface StatusIndicatorProps {
  status: StatusType;
}

function StatusIndicator({ status }: StatusIndicatorProps) {
  return (
    <div className="status-indicator-container">
      <div className="status-label">Status</div>
      <div className="status-circles">
        <div className={`status-circle green ${status === 'green' ? 'active' : ''}`}></div>
        <div className={`status-circle yellow ${status === 'yellow' ? 'active' : ''}`}></div>
        <div className={`status-circle red ${status === 'red' ? 'active' : ''}`}></div>
      </div>
    </div>
  );
}

export default StatusIndicator;