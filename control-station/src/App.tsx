import React, { useState }from 'react';
import Navbar from "@/components/Navbar/Navbar";
import StatusIndicator from "@/components/StatusIndicator/StatusIndicator";
import ControlPanel from "@/components/ControlPanel/ControlPanel";
import RotationIndicator from "@/components/RotationIndicator/RotationIndicator";
import Temperature from './components/Sensors/Temperature';
import Pressure from './components/Sensors/Pressure';
import VoltageCurrent from './components/Sensors/VoltageCurrent';
import Speed from './components/Sensors/Speed';
import Distance from './components/Sensors/Distance';

import './App.css';

function App() {
  //rotation uses percentage
  const [pitch, setPitch] = useState(25);
  const [roll, setRoll] = useState(50);
  const [yaw, setYaw] = useState(75);

  const temperatures = [25, 30, 28, 27];
  const pressures = [10, 10, 10, 10];
  const voltages = [12, 24];
  const currents = [5, 10];

  const speed = 120;
  const distance = 1000;

  return (
    <div className="App">
      <Navbar />
      <StatusIndicator status="green" />
      
      <div className="sensor-container">
        <RotationIndicator pitch={pitch} roll={roll} yaw={yaw} />
        <Temperature temperatures={temperatures} unit="°C" />
        <Pressure pressures={pressures} unit="Pa" />
        <VoltageCurrent voltages={voltages} currents={currents} voltageUnit="V" currentUnit="A" />
        <Speed speed={speed} unit="km/h" />
        <Distance distance={distance} unit="m" />
      </div>
      <ControlPanel />
    </div>
  );
}

export default App;