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
    wheel_encoder: { distance: 0, velocity: 0 },
    acceleration: 0,
    position: { position: 0, track_height: 0 },
    temperature: {
      lim_temp: 25,
      coolant_temp: 20,
      ambient_temp: 22,
      batt_temp: 30,
    },
    pressure: { pneumatic_press: 0, coolant_press: 0 },
    voltage: {
      hv_batt1: 0,
      hv_batt2: 0,
      hv_batt3: 0,
      hv_batt4: 0,
      hv_batt5: 0,
      hv_batt6: 0,
      hv_batt7: 0,
      lv_battb: 0,
    },
    current: {
      hv_batt1: 0,
      hv_batt2: 0,
      hv_batt3: 0,
      hv_batt4: 0,
      hv_batt5: 0,
      hv_batt6: 0,
      hv_batt7: 0,
      lv_battb: 0,
    },
    messages: [],
  });

  const podSocketClient = new PodSocketClient(setPodData);

  useEffect(() => {
    // Enable the socket connection when the component mounts
    podSocketClient.enable();

    return () => {
      // Disable the socket connection when the component unmounts
      podSocketClient.disable();
    };
  }, [podSocketClient]);

  return (
    <PodContext.Provider value={{ podSocketClient, podData }}>
      {children}
    </PodContext.Provider>
  );
};

export default PodProvider;
