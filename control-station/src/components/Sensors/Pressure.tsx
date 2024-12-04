import React from 'react';
import './Sensors.css';

interface PressureSensorProps {
  pressures: number[];
  unit: string;
}

function PressureSensor({ pressures, unit }: PressureSensorProps) {
  return (
    <div className="sensor-display pressure-sensor">
      <h3 className="sensor-title">Pressure Sensor</h3>
      <div className="sensor-values">
        {pressures.map((pressure, index) => (
          <div key={index} className="sensor-value">
            <span className="value-number">{pressure}{unit}</span>
          </div>
        ))}
      </div>
    </div>
  );
}

export default PressureSensor;