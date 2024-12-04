import React from 'react';
import './Sensors.css';

interface SpeedIndicatorProps {
  speed: number;
  unit: string;
}

function SpeedIndicator({ speed, unit }: SpeedIndicatorProps) {
  return (
    <div className="sensor-display speed-indicator">
      <h3 className="sensor-title">Speed</h3>
      <div className="sensor-value large">
        <span className="value-number">{speed}{unit}</span>
      </div>
    </div>
  );
}

export default SpeedIndicator;