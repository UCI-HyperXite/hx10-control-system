import "./console.css";
import { useContext } from "react";
import { PodContext } from "@/Services/PodContext";

export default function Console() {
  const context = useContext(PodContext);

  if (!context) {
    return <div>Error: PodContext is not available!</div>;
  }

  const { podData } = context;

  return (
    <div className="console">
      <h1>Console</h1>
      <div className="console-body">
        <div className="log-item">
          {podData?.messages ? (
            podData.messages.map((msg, index) => (
              <div key={index} className="log-item">
                <p>
                  {new Date(msg.timestamp).toLocaleTimeString("en-US", {
                    hour12: false,
                  })}{" "}
                  &nbsp;
                  {msg.message.toUpperCase()}
                </p>
              </div>
            ))
          ) : (
            <div className="log-item">
              <p>No logs yet...</p>
            </div>
          )}
        </div>
      </div>
    </div>
  );
}
