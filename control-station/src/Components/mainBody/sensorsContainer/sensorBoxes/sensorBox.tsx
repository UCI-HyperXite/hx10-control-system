import "./sensorBox.css";
import Speedometer from "./speedometer/speedometer";

interface propInterface {
  name: string;
  value: number;
}

export default function SensorBox(prop: propInterface) {
  return (
    <div className="sensorBox">
      <h2>{prop.name}</h2>
      {prop.name === "Speed" ? (
        <Speedometer value={prop.value} /> // Render Speedometer if the name is Speed
      ) : (
        <h1>{prop.value}</h1> // Otherwise, render the value as text
      )}
    </div>
  );
}
