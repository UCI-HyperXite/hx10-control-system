import React, { useState, useRef, useEffect } from "react";
import "./speedometer.css";

interface SpeedometerProps {
  value: number;
}

const Speedometer = (prop: SpeedometerProps) => {
  const maxValue = 100;
  const canvasRef = useRef<HTMLCanvasElement | null>(null);

  // Draw the gauge based on the current value
  const drawGauge = (value: number): void => {
    if (!canvasRef.current) return;
    const canvas = canvasRef.current;
    const ctx = canvas.getContext("2d");
    if (!ctx) return;

    const centerX = canvas.width / 2;
    const centerY = canvas.height; // Vertically adjusted to center the arc
    const radius = (canvas.width * 0.8) / 2; // Set radius to 80% of the canvas width

    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Draw gauge background (80% arc)
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, Math.PI, 2 * Math.PI); // Arc range from 180° to 360°
    ctx.lineWidth = 30;
    ctx.strokeStyle = "#ddd";
    ctx.stroke();

    // Draw gauge value
    const startAngle = Math.PI; // Starting angle (leftmost point of the arc)
    const endAngle = (value / maxValue) * Math.PI + startAngle; // End angle based on the value
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, startAngle, endAngle); // Adjust the arc length based on value
    ctx.lineWidth = 29;
    ctx.strokeStyle = getColor(value);
    ctx.stroke();

    // Draw the centered value text
    ctx.font = "30px Arial";
    ctx.fillStyle = "#333";
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";
    ctx.fillText(value.toString(), centerX, centerY - 10); // Centered text vertically adjusted
  };

  // Get the color for the gauge based on the value
  const getColor = (value: number): string => {
    if (value < maxValue / 5) {
      return "#00ff00"; // Low - Green
    } else if (value < (maxValue * 2) / 5) {
      return "#66ff33"; // Low to Moderate - Light Green
    } else if (value < (maxValue * 3) / 5) {
      return "#ffff00"; // Moderate - Yellow
    } else if (value < (maxValue * 4) / 5) {
      return "#ff9900"; // Moderate to High - Orange
    } else {
      return "#ff0000"; // High - Red
    }
  };

  // Update the gauge whenever the value changes
  useEffect(() => {
    drawGauge(prop.value);
  }, [prop.value]);

  return (
    <div className="gauge-container">
      <canvas ref={canvasRef}></canvas>{" "}
      {/* Canvas size will be responsive based on parent */}
    </div>
  );
};

export default Speedometer;
