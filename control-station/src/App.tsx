import "./App.css";
import ControlPanel from "./Components/controlPanel/controlPanel";
import MainBody from "./Components/mainBody/mainBody";
import Navbar from "./Components/navbar/navbar";
// import PodProvider from "./Services/PodDataProvider";
import PodContext from "./Services/PodContext";
import usePodData from "./Services/usePodData";

function App() {
  const { podData, podSocketClient } = usePodData();
  return (
    // <PodProvider>
    <PodContext.Provider value={{ podData, podSocketClient }}>
      <div className="app">
        <Navbar />
        <MainBody />
        <ControlPanel />
      </div>
      </PodContext.Provider>
    // </PodProvider>
  );
}

export default App;
