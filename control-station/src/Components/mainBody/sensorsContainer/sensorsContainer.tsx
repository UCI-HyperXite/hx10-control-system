import { useContext, useState, useMemo } from "react";
import Dynamics from "./dynamics/dynamics";
import SensorBox from "./sensorBoxes/sensorBox";
import "./sensorsContainer.css";
import PodContext from "@/Services/PodContext";
import { MOCK_POD_DATA, PodData } from "@/Services/PodSocketClient";

export default function SensorsContainer() {
  // const { podData } = useContext(PodContext);
  const [podData, setPodData] = useState<PodData>(MOCK_POD_DATA);

  const sensorData = {
    "Avg Temp": ["LIM Temp", "Coolant Temp", "Ambient Temp", "Batt Temp"],
    "Avg Pressure": ["Pneumatic Pressure", "Coolant Pressure"],
    Distance: ["Track Height", "Position"],
    Speed: ["Acceleraton"],
    "HV Voltage": [
      "HV Batt #1",
      "HV Batt #2",
      "HV Batt #3",
      "HV Batt #4",
      "HV Batt #5",
      "HV Batt #6",
      "HV Batt #7",
      "LV Batt B",
    ],
    "HV Current": [
      "HV Batt #1",
      "HV Batt #2",
      "HV Batt #3",
      "HV Batt #4",
      "HV Batt #5",
      "HV Batt #6",
      "HV Batt #7",
      "LV Batt B",
    ],
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
        <SensorBox
          name="Speed"
          value={podData.wheel_encoder.velocity}
          onClick={() => handleClick("Speed")}
        />
        <SensorBox
          name="Distance"
          value={podData.wheel_encoder.distance}
          onClick={() => handleClick("Distance")}
        />
        <SensorBox
          name="Avg Temp"
          value={
            Math.round(
              (Object.values(podData.temperature).reduce(
                (acc, val) => acc + val,
                0,
              ) /
                Object.values(podData.temperature).length) *
                1000,
            ) / 1000
          }
          onClick={() => handleClick("Avg Temp")}
        />
        <SensorBox
          name="Avg Pressure"
          value={
            Math.round(
              (Object.values(podData.pressure).reduce(
                (acc, val) => acc + val,
                0,
              ) /
                Object.values(podData.pressure).length) *
                1000,
            ) / 1000
          }
          onClick={() => handleClick("Avg Pressure")}
        />
        <SensorBox
          name="HV Voltage"
          value={
            Math.round(
              (Object.values(podData.voltage)
                .slice(0, -1)
                .reduce((acc, val) => acc + val, 0) /
                (Object.values(podData.voltage).length - 1)) *
                1000,
            ) / 1000
          }
          onClick={() => handleClick("HV Voltage")}
        />
        <SensorBox
          name="HV Current"
          value={
            Math.round(
              (Object.values(podData.current)
                .slice(0, -1)
                .reduce((acc, val) => acc + val, 0) /
                (Object.values(podData.current).length - 1)) *
                1000,
            ) / 1000
          }
          onClick={() => handleClick("HV Current")}
        />
      </div>

      {selectedSensor && sensorData[selectedSensor] && (
        <div className="overlay" onClick={handleClose}>
          <div className="overlay-content" onClick={(e) => e.stopPropagation()}>
            <h2>{selectedSensor} Details</h2>
            <div className="overlay-body">
              {sensorData[selectedSensor].map((key, index) => {
                let value: number | string = "N/A";

                switch (key) {
                  case "LIM Temp":
                    value = podData.temperature.lim_temp;
                    break;
                  case "Coolant Temp":
                    value = podData.temperature.coolant_temp;
                    break;
                  case "Ambient Temp":
                    value = podData.temperature.ambient_temp;
                    break;
                  case "Batt Temp":
                    value = podData.temperature.batt_temp;
                    break;
                  case "Pneumatic Pressure":
                    value = podData.pressure.pneumatic_press;
                    break;
                  case "Coolant Pressure":
                    value = podData.pressure.coolant_press;
                    break;
                  case "Track Height":
                    value = podData.position.track_height;
                    break;
                  case "Position":
                    value = podData.position.position;
                    break;
                  case "Acceleraton":
                    value = podData.acceleration;
                    break;
                  case "HV Batt #1":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt1)
                        : (value = podData.current.hv_batt1);
                    }
                    break;
                  case "HV Batt #2":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt2)
                        : (value = podData.current.hv_batt2);
                    }
                    break;
                  case "HV Batt #3":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt3)
                        : (value = podData.current.hv_batt3);
                    }
                    break;
                  case "HV Batt #4":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt4)
                        : (value = podData.current.hv_batt4);
                    }
                    break;
                  case "HV Batt #5":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt5)
                        : (value = podData.current.hv_batt5);
                    }
                    break;
                  case "HV Batt #6":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt6)
                        : (value = podData.current.hv_batt6);
                    }
                    break;
                  case "HV Batt #7":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.hv_batt7)
                        : (value = podData.current.hv_batt7);
                    }
                    break;
                  case "LV Batt B":
                    {
                      selectedSensor === "HV Voltage"
                        ? (value = podData.voltage.lv_battb)
                        : (value = podData.current.lv_battb);
                    }
                    break;
                  default:
                    value = "N/A";
                }

                return (
                  <div key={index} className="overlay-log-item">
                    <p>
                      {key}: &emsp; {value}
                    </p>
                  </div>
                );
              })}
            </div>
          </div>
        </div>
      )}
    </div>
  );
}
