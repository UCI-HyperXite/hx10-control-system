import "./dynamics.css";
import FrontPod from "@/assets/images/FrontPod 1.png";
import TopPod from "@/assets/images/TopPod.png";
import SidePod from "@/assets/images/SidePod 1.png";

import { useState } from "react";
import { MOCK_POD_DATA } from "@/Services/PodSocketClient";
import { PodData } from "@/Services/PodSocketClient";

export default function Dynamics() {
  const [podData, setPodData] = useState<PodData>(MOCK_POD_DATA);
  return (
    <div className="dynamics-container">
      <div className="dynamics-data">
        <h2>Roll</h2>
        <br />
        <img
          src={FrontPod}
          alt="front view of the pod"
          style={{
            transform: `rotate(${Math.round(podData.gyroscope.roll * 100) / 100}deg)`,
          }}
        />
      </div>
      <div className="dynamics-data">
        <h2>Pitch</h2>
        <br />
        <img
          src={SidePod}
          alt="front view of the pod"
          style={{
            transform: `rotate(${Math.round(podData.gyroscope.pitch * 100) / 100}deg)`,
          }}
        />
      </div>
      <div className="dynamics-data">
        <h2>Yaw</h2>
        <br />
        <img
          src={TopPod}
          alt="front view of the pod"
          style={{
            transform: `rotate(${Math.round(podData.gyroscope.yaw * 100) / 100}deg)`,
          }}
        />
      </div>
    </div>
  );
}
