import { useState } from "react";
import "./statusIndicator.css";
import { MOCK_POD_DATA, PodData } from "@/Services/PodSocketClient";

export default function StatusIndicator() {
  //const [status, setStatus] = useState("load");
  const [podData, setPodData] = useState<PodData>(MOCK_POD_DATA);

  return (
    <div className="status-indicator">
      <h3>Status</h3>
      <div className="status-text">
        <span className={`status-color ${podData.state.toLowerCase()}`}>
          {podData.state}
        </span>
      </div>
    </div>
  );
}
