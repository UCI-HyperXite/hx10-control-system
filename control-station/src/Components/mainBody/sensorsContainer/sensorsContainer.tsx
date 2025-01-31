import Dynamics from "./dynamics/dynamics";
import SensorBox from "./sensorBoxes/sensorBox";
import "./sensorsContainer.css";
export default function SensorsContainer() {
  return (
    <div className="sensors-container">
      <Dynamics />
      <div className="sensorBox-container">
        <SensorBox name="Speed" value={50} />
        <SensorBox name="Distance" value={100} />
        <SensorBox name="Avg Temp" value={100} />
        <SensorBox name="Avg Pressure" value={100} />
        <SensorBox name="HV Voltage" value={100} />
        <SensorBox name="HV Current" value={100} />
      </div>
    </div>
  );
}
