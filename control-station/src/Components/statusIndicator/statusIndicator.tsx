import { useState } from "react";
import "./statusIndicator.css";

export default function StatusIndicator() {
  const [status, setStatus] = useState("red");
  return (
    <div className="status-indicator">
      <h3>Status</h3>
      <div
        className={`status-circle green ${status === "green" ? "active" : ""}`}
      ></div>
      <div
        className={`status-circle yellow ${
          status === "yellow" ? "active" : ""
        }`}
      ></div>
      <div
        className={`status-circle red ${status === "red" ? "active" : ""}`}
      ></div>
    </div>
  );
}
