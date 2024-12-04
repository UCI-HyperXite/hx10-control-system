import React from 'react';
import './Sensors.css';

interface TemperatureSensorProps {
  temperatures: number[];
  unit: string;
}

function TemperatureSensor({ temperatures, unit }: TemperatureSensorProps) {
  return (
    <div className="sensor-display temperature-sensor">
      <h3 className="sensor-title">Temperature Sensor</h3>
      <div className="sensor-values">
        {temperatures.map((temp, index) => (
          <div key={index} className="sensor-value">
            <span className="value-number">{temp}{unit}</span>
          </div>
        ))}
      </div>
    </div>
  );
}

export default TemperatureSensor;