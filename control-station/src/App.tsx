import "./App.css";
import ControlPanel from "./Components/controlPanel/controlPanel";
import MainBody from "./Components/mainBody/mainBody";
import Navbar from "./Components/navbar/navbar";
import { PodProvider } from "./Services/PodDataProvider";

function App() {
  return (
    <PodProvider>
      <div className="app">
        <Navbar />
        <MainBody />
        <ControlPanel />
      </div>
    </PodProvider>
  );
}

export default App;
