import Dynamics from "./dynamics/dynamics";
import SensorBox from "./sensorBoxes/sensorBox";
import "./sensorsContainer.css";

import { useState } from "react";

export default function SensorsContainer() {
  const sensorData = {
    "Avg Temp": ["LIM Temp", "Coolant Temp", "Ambient Temp", "Batt Temp"],
    "Avg Pressure": ["Pneumatic Pressure", "Coolant Pressure"],
    "Distance": ["Track Height", "Position"],
    "Speed": ["Acceleraton"], 
    "HV Voltage": [], 
    "HV Current": [],
  };

  type SensorType = keyof typeof sensorData; 
  const [selectedSensor, setSelectedSensor] = useState<SensorType | null>(null);

  const handleClick = (name: SensorType) => {
    setSelectedSensor(name);
  };

  const handleClose = () => {
    setSelectedSensor(null); 
  };

  return (
    <div className="sensors-container">
      <Dynamics />
      <div className="sensorBox-container">
        <SensorBox name="Speed" value={50} onClick={() => handleClick("Speed")}/>
        <SensorBox name="Distance" value={100} onClick={() => handleClick("Distance")}/>
        <SensorBox name="Avg Temp" value={100} onClick={() => handleClick("Avg Temp")}/>
        <SensorBox name="Avg Pressure" value={100} onClick={() => handleClick("Avg Pressure")}/>
        <SensorBox name="HV Voltage" value={100} onClick={() => handleClick("HV Voltage")}/>
        <SensorBox name="HV Current" value={100} onClick={() => handleClick("HV Current")}/>
      </div>

      {selectedSensor &&  sensorData[selectedSensor] &&(
        <div className="overlay" onClick={handleClose}>
          <div className="overlay-content" onClick={(e) => e.stopPropagation()}>
            <h2>{selectedSensor} Details</h2>
            <div className="sensor-body">
              {sensorData[selectedSensor].map((key, index) => (
                <div key={index} className="sensor-log-item">
                  <p>{key}: &emsp;  {"50"}</p>
                </div>
              ))}
            </div>
          </div>
        </div>
      )}

    </div>
  );
}
