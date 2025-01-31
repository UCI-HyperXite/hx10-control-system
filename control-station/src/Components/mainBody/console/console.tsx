import "./console.css";

export default function Console() {
  return (
    <div className="console">
      <h1>Console</h1>
      <div className="console-body">
        <div className="log-item">
          <p>31-01-25 &nbsp; 08:46:53 PST &emsp; Some log</p>
        </div>
        <div className="log-item">
          <p>31-01-25 &nbsp; 08:47:00 PST &emsp; Another log</p>
        </div>
        <div className="log-item">
          <p>31-01-25 &nbsp; 08:47:10 PST &emsp; More logs coming...</p>
        </div>{" "}
      </div>
    </div>
  );
}
