import React from 'react';
import Navbar from './components/Navbar/Navbar';
import StatusIndicator from './components/StatusIndicator/StatusIndicator';
import ControlPanel from './components/ControlPanel/ControlPanel';
import './App.css';

function App() {
  return (
    <div className="App">
      <Navbar />
      <StatusIndicator status="green" />
      <main className="content">
        {"HX10 GUI"}
      </main>
      <ControlPanel />
    </div>
  );
}

export default App;