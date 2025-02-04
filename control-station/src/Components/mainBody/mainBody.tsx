import "./mainBody.css";
import StatusIndicator from "./statusIndicator/statusIndicator";
import SensorsContainer from "./sensorsContainer/sensorsContainer";
import Console from "./console/console";

export default function MainBody() {
  return (
    <div className="mainBody">
      <StatusIndicator />
      <SensorsContainer />
      <Console />
    </div>
  );
}
