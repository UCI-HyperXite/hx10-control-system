import React, { useState, useEffect, ReactNode, useRef } from "react";
// import { PodContext } from "./PodContext";
import PodContext from "./PodContext";
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

  const podSocketClientRef = useRef<PodSocketClient | null>(null);

  useEffect(() => {
    if (!podSocketClientRef.current) {
      podSocketClientRef.current = new PodSocketClient(setPodData);
      podSocketClientRef.current.connect();
    }

    return () => {
      podSocketClientRef.current?.disconnect();
      podSocketClientRef.current = null;
    };
  }, []);

  return (
    <PodContext.Provider
      value={{
        podSocketClient: podSocketClientRef.current as PodSocketClient,
        podData,
      }}
    >
      {children}
    </PodContext.Provider>
  );
};

export default PodProvider;
