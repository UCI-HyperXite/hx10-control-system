import { useContext } from "react";
import "./statusIndicator.css";
import { PodContext } from "@/Services/PodContext";

export default function StatusIndicator() {
  //const [status, setStatus] = useState("load");

  const context = useContext(PodContext);

  if (!context) {
    return <div>Error: PodContext is not available!</div>;
  }

  const { podData } = context;
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
