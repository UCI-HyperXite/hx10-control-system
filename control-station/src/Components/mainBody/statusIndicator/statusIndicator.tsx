import { useContext, useState } from "react";
import "./statusIndicator.css";
import { PodContext } from "@/Services/PodContext";

export default function StatusIndicator() {
  //const [status, setStatus] = useState("load");
  const { podData } = useContext(PodContext);

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
