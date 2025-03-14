import React, { useState, useEffect, ReactNode } from "react";
import { PodContext } from "./PodContext";
import { PodData, State } from "./PodSocketClient";
import PodSocketClient from "./PodSocketClient";


interface PodProviderProps {
  children: ReactNode;
}

const PodProvider: React.FC<PodProviderProps> = ({ children }) => {
  const [podData, setPodData] = useState<PodData>({
    connected: false,
    state: State.Disconnected,
    gyroscope: { pitch: 0, roll: 0, yaw: 0 },
    wheel_encoder: { distance: 50, velocity: 30 },
    acceleration: 40,
    position: { position: 100, track_height: 5 },
    temperature: {
      lim_temp: 25,
      coolant_temp: 20,
      ambient_temp: 22,
      batt_temp: 30,
    },
    pressure: { pneumatic_press: 50, coolant_press: 30 },
    voltage: {
      hv_batt1: 30,
      hv_batt2: 30,
      hv_batt3: 30,
      hv_batt4: 30,
      hv_batt5: 30,
      hv_batt6: 30,
      hv_batt7: 30,
      lv_battb: 30,
    },
    current: {
      hv_batt1: 10,
      hv_batt2: 10,
      hv_batt3: 10,
      hv_batt4: 10,
      hv_batt5: 10,
      hv_batt6: 10,
      hv_batt7: 10,
      lv_battb: 10,
    },
    messages: [],
  });

  const podSocketClient = new PodSocketClient(setPodData);

  // useEffect(() => {
  //   // Enable the socket connection when the component mounts
  //   podSocketClient.enable();

  //   return () => {
  //     // Disable the socket connection when the component unmounts
  //     podSocketClient.disable();
  //   };
  // }, [podSocketClient]);
  useEffect(() => {
    const interval = setInterval(() => {
      setPodData((prevData) => ({
        ...prevData,
        gyroscope: {
          pitch: prevData.gyroscope.pitch + (Math.random() - 0.5) * 2,
          roll: prevData.gyroscope.roll + (Math.random() - 0.5) * 2,
          yaw: prevData.gyroscope.yaw + (Math.random() - 0.5) * 2,
        },
        wheel_encoder: {
          distance: prevData.wheel_encoder.distance + prevData.wheel_encoder.velocity * 0.1,
          velocity: prevData.wheel_encoder.velocity + (Math.random() - 0.5) * 2,
        },
        acceleration: prevData.acceleration + (Math.random() - 0.5) * 5,
        position: {
          position: prevData.position.position + prevData.wheel_encoder.velocity * 0.05,
          track_height: prevData.position.track_height,
        },
        temperature: {
          ...prevData.temperature,
          lim_temp: prevData.temperature.lim_temp + (Math.random() - 0.5) * 1,
          coolant_temp: prevData.temperature.coolant_temp + (Math.random() - 0.5) * 1,
          ambient_temp: prevData.temperature.ambient_temp + (Math.random() - 0.5) * 1,
          batt_temp: prevData.temperature.batt_temp + (Math.random() - 0.5) * 1,
        },
        pressure: {
          ...prevData.pressure,
          pneumatic_press: prevData.pressure.pneumatic_press + (Math.random() - 0.5) * 2,
          coolant_press: prevData.pressure.coolant_press + (Math.random() - 0.5) * 2,
        },
        voltage: {
          ...prevData.voltage,
          hv_batt1: prevData.voltage.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt2: prevData.voltage.hv_batt2 + (Math.random() - 0.5) * 0.5,
          hv_batt3: prevData.voltage.hv_batt3 + (Math.random() - 0.5) * 0.5,
          hv_batt4: prevData.voltage.hv_batt4 + (Math.random() - 0.5) * 0.5,
          hv_batt5: prevData.voltage.hv_batt5 + (Math.random() - 0.5) * 0.5,
          hv_batt6: prevData.voltage.hv_batt6 + (Math.random() - 0.5) * 0.5,
          hv_batt7: prevData.voltage.hv_batt7 + (Math.random() - 0.5) * 0.5,
          lv_battb: prevData.voltage.lv_battb + (Math.random() - 0.5) * 0.5,
        },
        current: {
          ...prevData.current,
          hv_batt1: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt2: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt3: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt4: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt5: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt6: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          hv_batt7: prevData.current.hv_batt1 + (Math.random() - 0.5) * 0.5,
          lv_battb: prevData.current.lv_battb + (Math.random() - 0.5) * 0.5,
        },
      }));
    }, 1000); // Update every second
  
    return () => clearInterval(interval);
  }, []);
  

  return (
    <PodContext.Provider value={{ podSocketClient, podData }}>
      {children}
    </PodContext.Provider>
  );
};

export default PodProvider;
