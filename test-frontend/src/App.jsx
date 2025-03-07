import React, { useEffect, useState } from "react";

export default function App() {
  const [counter, setCounter] = useState(0);

  useEffect(() => {
    const ws = new WebSocket("ws://localhost:8080");

    ws.onopen = () => {
      console.log("Connected to WebSocket server");
      ws.send("increment"); // Send an initial message
    };

    ws.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);
        setCounter(data.counter);
      } catch (error) {
        console.error("Error parsing WebSocket message:", error);
      }
    };

    ws.onerror = (error) => console.error("WebSocket Error:", error);
    ws.onclose = () => console.log("WebSocket Disconnected");

    return () => ws.close(); // Cleanup on unmount
  }, []);

  const incrementCounter = () => {
    const ws = new WebSocket("ws://localhost:8080");
    ws.onopen = () => ws.send("increment");
  };

  return (
    <div className="flex flex-col items-center justify-center h-screen bg-gray-100">
      <h1 className="text-4xl font-bold text-blue-600">Counter App</h1>
      <p className="mt-4 text-2xl font-semibold">Counter: {counter}</p>
      <button
        onClick={incrementCounter}
        className="mt-4 px-6 py-2 bg-blue-500 text-white text-lg font-semibold rounded-lg shadow-md hover:bg-blue-700 transition"
      >
        Increment
      </button>
    </div>
  );
}
