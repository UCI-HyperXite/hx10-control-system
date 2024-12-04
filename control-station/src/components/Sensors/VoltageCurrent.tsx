import React from 'react';
import './Sensors.css';

interface VoltageCurrentSensorProps {
  voltages: number[];
  currents: number[];
  voltageUnit: string;
  currentUnit: string;
}

function VoltageCurrentSensor({ voltages, currents, voltageUnit, currentUnit }: VoltageCurrentSensorProps) {
  return (
    <div className="sensor-display voltage-current-sensor">
      <h3 className="sensor-title">Voltage/Current Sensor</h3>
      <div className="sensor-values">
        {voltages.map((voltage, index) => (
          <div key={`voltage-${index}`} className="sensor-value">
            <span className="value-number">{voltage}{voltageUnit}</span>
          </div>
        ))}
        {currents.map((current, index) => (
          <div key={`current-${index}`} className="sensor-value">
            <span className="value-number">{current}{currentUnit}</span>
          </div>
        ))}
      </div>
    </div>
  );
}

export default VoltageCurrentSensor;