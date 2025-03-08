import "./console.css";
import { useContext, useEffect, useRef, useState } from "react";
import PodContext from "../../../Services/PodContext";
import { MOCK_POD_DATA, PodData } from "@/Services/PodSocketClient";

export default function Console() {
  //const { podData } = useContext(PodContext);
  const [podData, setPodData] = useState<PodData>(MOCK_POD_DATA);

  useEffect(() => {
    console.log(podData);
    console.log(podData?.messages);
  }, [podData]);

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
