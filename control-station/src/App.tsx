import "./App.css";
import ControlPanel from "./Components/controlPanel/controlPanel";
import MainBody from "./Components/mainBody/mainBody";
import Navbar from "./Components/navbar/navbar";

function App() {
  return (
    <div className="app">
      <Navbar />
      <MainBody />
      <ControlPanel />
    </div>
  );
}

export default App;
