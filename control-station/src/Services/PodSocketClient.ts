import { Dispatch, SetStateAction } from "react";
import { createWebSocket } from "./socketHandler";

export enum State {
  Disconnected = "Disconnected",
  Init = "Init",
  Load = "Load",
  Precharge = "PreCharge",
  Running = "Running",
  Stopped = "Stopped",
  Halted = "Halted",
  Fault = "Faulted",
}

interface Message {
  timestamp: Date;
  message: string;
}

export interface PodData {
  connected: boolean;
  state: State;
  gyroscope: Gyroscope;
  wheel_encoder: WheelEncoder;
  acceleration: number;
  position: Position;
  temperature: Temperature;
  pressure: Pressure;
  voltage: Voltage;
  current: Current;
  messages: Message[];
}

interface WheelEncoder {
  distance: number;
  velocity: number;
}

interface Gyroscope {
  pitch: number;
  roll: number;
  yaw: number;
}

interface Position {
  position: number;
  track_height: number;
}

interface Temperature {
  lim_temp: number;
  coolant_temp: number;
  ambient_temp: number;
  batt_temp: number;
}

interface Pressure {
  pneumatic_press: number;
  coolant_press: number;
}

interface Voltage {
  hv_batt1: number;
  hv_batt2: number;
  hv_batt3: number;
  hv_batt4: number;
  hv_batt5: number;
  hv_batt6: number;
  hv_batt7: number;
  lv_battb: number;
}

interface Current {
  hv_batt1: number;
  hv_batt2: number;
  hv_batt3: number;
  hv_batt4: number;
  hv_batt5: number;
  hv_batt6: number;
  hv_batt7: number;
  lv_battb: number;
}

export const MOCK_POD_DATA: PodData = {
  connected: true,
  state: State.Running,
  gyroscope: { pitch: 0, roll: 0, yaw: 0 },
  wheel_encoder: { distance: 10, velocity: 50 },
  acceleration: 0,
  position: { position: 0, track_height: 0 },
  temperature: {
    lim_temp: 25,
    coolant_temp: 20,
    ambient_temp: 22,
    batt_temp: 30,
  },
  pressure: { pneumatic_press: 100, coolant_press: 90 },
  voltage: {
    hv_batt1: 46,
    hv_batt2: 45,
    hv_batt3: 41,
    hv_batt4: 42,
    hv_batt5: 46,
    hv_batt6: 45,
    hv_batt7: 46,
    lv_battb: 12,
  },
  current: {
    hv_batt1: 10,
    hv_batt2: 10,
    hv_batt3: 10,
    hv_batt4: 10,
    hv_batt5: 10,
    hv_batt6: 10,
    hv_batt7: 10,
    lv_battb: 2,
  },
  messages: [
    { timestamp: new Date(), message: "Pod initialized successfully" },
  ],
};

type SetPodData = Dispatch<SetStateAction<PodData>>;

class PodSocketClient {
  socket: WebSocket | null = null;
  setPodData: SetPodData;
  reconnectInterval = 5000; // 5-second reconnect interval

  constructor(setPodData: SetPodData) {
    this.setPodData = setPodData;
    this.connect();
  }

  connect(): void {
    this.socket = createWebSocket("control-station");

    this.socket.onopen = () => {
      console.log("Connected to WebSocket server");
      this.setPodData((d) => ({ ...d, connected: true, state: State.Init }));
    };

    this.socket.onmessage = (event) => {
      try {
        const data: Partial<PodData> = JSON.parse(event.data);
        console.log("Server response:", data);
        this.setPodData((d) => ({ ...d, ...data }));
      } catch (error) {
        console.error("Error parsing server message:", error);
      }
    };

    this.socket.onerror = (event) => {
      console.error("WebSocket encountered an error:", event);
    };

    this.socket.onclose = (event) => {
      console.log("WebSocket disconnected:", event.reason);
      this.setPodData((d) => ({
        ...d,
        connected: false,
        state: State.Disconnected,
      }));
      setTimeout(() => this.connect(), this.reconnectInterval); // Auto-reconnect
    };
  }

  disconnect(): void {
    if (this.socket) {
      console.log("Closing WebSocket connection");
      this.socket.close();
      this.socket = null;
    }
  }

  sendMessage(command: string): void {
    if (this.socket && this.socket.readyState === WebSocket.OPEN) {
      this.socket.send(command);
    } else {
      console.warn("WebSocket not connected. Cannot send:", command);
    }
  }

  sendLoad(): void {
    this.addMessage("Pod in Load state", State.Load);
    this.sendMessage("load");
  }

  sendRun(): void {
    this.addMessage("Pod in Run state", State.Running);
    this.sendMessage("run");
  }

  sendStop(): void {
    this.addMessage("Pod in Stop state", State.Stopped);
    this.sendMessage("stop");
  }

  sendHalt(): void {
    this.addMessage("Pod in Halt state", State.Halted);
    this.sendMessage("halt");
  }

  private addMessage(response: string, newState: State): void {
    const timestamp = new Date();
    const newMessage = { timestamp, message: response };

    this.setPodData((d) => ({
      ...d,
      state: newState,
      messages: [...d.messages, newMessage],
    }));
  }
}

export default PodSocketClient;
