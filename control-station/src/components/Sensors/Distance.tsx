import React from 'react';
import './Sensors.css';

interface DistanceIndicatorProps {
  distance: number;
  unit: string;
}

function DistanceIndicator({ distance, unit }: DistanceIndicatorProps) {
  return (
    <div className="sensor-display distance-indicator">
      <h3 className="sensor-title">Distance</h3>
      <div className="sensor-value large">
        <span className="value-number">{distance}{unit}</span>
      </div>
    </div>
  );
}

export default DistanceIndicator;