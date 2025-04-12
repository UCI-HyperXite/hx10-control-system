import "./controlPanel.css";
import { useContext, useEffect } from "react";
import { PodContext } from "@/Services/PodContext";

export default function ControlPanel() {

  const {podSocketClient} = useContext(PodContext);

  useEffect(() => {
    const handleKeyPress = (event: KeyboardEvent) => {
      if (event.code === "Space") {
        event.preventDefault(); // Prevents scrolling when spacebar is pressed
        const haltButton = document.querySelector("#halt") as HTMLElement;
        if (haltButton) {
          haltButton.click();
          console.log("halt pressed");
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
