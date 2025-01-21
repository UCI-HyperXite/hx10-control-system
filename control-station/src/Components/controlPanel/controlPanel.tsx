import "./controlPanel.css";

export default function ControlPanel() {
  return (
    <div className="controlPanel">
      <button className="controlButton" id="run">
        Run
      </button>
      <button className="controlButton" id="stop">
        Stop
      </button>
      <button className="controlButton" id="halt">
        Halt
      </button>
      <button className="controlButton" id="load">
        Load
      </button>
    </div>
  );
}
