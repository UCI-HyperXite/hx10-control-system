import "./controlPanel.css";
import { useContext, useEffect, useState } from "react";
import PodContext from "@/Services/PodContext";

export default function ControlPanel() {
  const { podSocketClient } = useContext(PodContext);

  useEffect(() => {
    const handleKeyPress = (event: KeyboardEvent) => {
      if (event.code === "Space") {
        event.preventDefault(); // Prevents scrolling when spacebar is pressed
        const stopButton = document.querySelector("#stop") as HTMLElement;
        if (stopButton) {
          stopButton.click();
          console.log("stop pressed");
        }
      }
    };

    window.addEventListener("keydown", handleKeyPress);
    return () => {
      window.removeEventListener("keydown", handleKeyPress);
    };
  }, []);

  return (
    <div className="controlPanel">
      <button
        className="controlButton"
        id="run"
        onClick={() => podSocketClient.sendRun()}
      >
        Run
      </button>
      <button
        className="controlButton"
        id="stop"
        onClick={() => podSocketClient.sendStop()}
      >
        Stop
      </button>
      <button
        className="controlButton"
        id="halt"
        onClick={() => podSocketClient.sendHalt()}
      >
        Halt
      </button>
      <button
        className="controlButton"
        id="load"
        onClick={() => podSocketClient.sendLoad()}
      >
        Load
      </button>
    </div>
  );
}
