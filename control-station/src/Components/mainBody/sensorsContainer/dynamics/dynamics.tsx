import "./dynamics.css";
import FrontPod from "@/assets/images/FrontPod 1.png";
import TopPod from "@/assets/images/TopPod.png";
import SidePod from "@/assets/images/SidePod 1.png";
export default function Dynamics() {
  return (
    <div className="dynamics-container">
      <div className="dynamics-data">
        <h2>Roll</h2>
        <br />
        <img src={FrontPod} />
      </div>
      <div className="dynamics-data">
        <h2>Pitch</h2>
        <br />
        <img src={SidePod} />
      </div>
      <div className="dynamics-data">
        <h2>Yaw</h2>
        <br />
        <img src={TopPod} />
      </div>
    </div>
  );
}
