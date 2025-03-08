import { useEffect, useMemo, useState } from "react";
import PodSocketClient, { PodData, State } from "./PodSocketClient";

function usePodData() {
  const [podData, setPodData] = useState<PodData>({
    connected: false,
    state: State.Disconnected,
    gyroscope: { roll: 0, pitch: 0, yaw: 0 },
    wheel_encoder: { distance: 0, velocity: 0 },
    acceleration: 0,
    position: { track_height: 0, position: 0 },
    temperature: {
      lim_temp: 0,
      coolant_temp: 0,
      ambient_temp: 0,
      batt_temp: 0,
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
    messages: [{ timestamp: new Date(), message: "System initialized." }],
  });

  const podSocketClient = useMemo(() => new PodSocketClient(setPodData), []);

  useEffect(() => {
    podSocketClient.enable();
    // disable socket instance on cleanup
    return podSocketClient.disable.bind(podSocketClient);
  }, [podSocketClient]);

  return { podData, podSocketClient };
}

export default usePodData;
