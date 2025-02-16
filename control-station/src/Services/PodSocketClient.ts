import { Dispatch, SetStateAction } from "react";
import { Socket } from "socket.io-client";
import { ioNamespace } from "./socketHandler";

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

interface ServerToClientEvents {
  connect: () => void;
  disconnect: (reason: Socket.DisconnectReason) => void;
  serverResponse: (data: Partial<PodData>) => void;
  fault: (data: string) => void;
}

interface Message {
  timestamp: Date;
  message: string;
}

interface ClientToServerEvents {
  load: (ack: (data: string) => void) => void;
  run: (ack: (data: string) => void) => void;
  stop: (ack: (data: string) => void) => void;
  halt: (ack: (data: string) => void) => void;
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

export const MOCK_POD_DATA: PodData = {
  connected: true,
  state: State.Running,
  gyroscope: {
    pitch: 0,
    roll: 0,
    yaw: 0,
  },
  wheel_encoder: {
    distance: 10,
    velocity: 50,
  },
  acceleration: 0,
  position: {
    position: 0,
    track_height: 0,
  },
  temperature: {
    lim_temp: 25,
    coolant_temp: 20,
    ambient_temp: 22,
    batt_temp: 30,
  },
  pressure: {
    pneumatic_press: 100,
    coolant_press: 90,
  },
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
    {
      timestamp: new Date(),
      message: "Pod initialized successfully",
    },
  ],
};

type SetPodData = Dispatch<SetStateAction<PodData>>;

// Not entirely safe to use but better than casting with `as`
// From https://stackoverflow.com/a/60142095
type Entries<T> = {
  [K in keyof T]: [K, T[K]];
}[keyof T][];

class PodSocketClient {
  socket: Socket<ServerToClientEvents, ClientToServerEvents>;
  serverEvents: ServerToClientEvents;
  setPodData: SetPodData;

  constructor(setPodData: SetPodData) {
    this.socket = ioNamespace("control-station");
    this.serverEvents = {
      connect: this.onConnect.bind(this),
      disconnect: this.onDisconnect.bind(this),
      serverResponse: this.onData.bind(this),
      fault: this.onFault.bind(this),
    } as const;
    this.setPodData = setPodData;
  }

  enable(): void {
    this.socket.connect();
    console.debug("Enabling socket event handlers");
    (
      Object.entries(this.serverEvents) as Entries<ServerToClientEvents>
    ).forEach(([event, handler]) => {
      this.socket.on(event, handler);
    });
  }

  disable(): void {
    console.debug("Disabling socket event handlers");
    Object.keys(this.serverEvents).forEach((event) => {
      this.socket.off(event as keyof ServerToClientEvents);
    });
    this.socket.disconnect();
  }

  sendLoad(): void {
    this.socket.emit("load", (response: string) => {
      console.log("Server acknowledged:", response);
      this.addMessage(response, State.Load);
    });
  }

  sendRun(): void {
    this.socket.emit("run", (response: string) => {
      console.log("Server acknowledged:", response);
      this.addMessage(response, State.Running);
    });
  }

  sendStop(): void {
    this.socket.emit("stop", (response: string) => {
      console.log("Server acknowledged:", response);
      this.addMessage(response, State.Stopped);
    });
  }

  sendHalt(): void {
    this.socket.emit("halt", (response: string) => {
      console.log("Server acknowledged:", response);
      this.addMessage(response, State.Halted);
    });
  }

  private onConnect(): void {
    // TODO: On connecting, the state below should be what's provided by the pod
    // if it's already running. Otherwise, the states should be State.Init
    console.log("Connected to server as", this.socket.id);
    this.setPodData((d) => ({ ...d, connected: true, state: State.Init }));
  }

  private onDisconnect(reason: Socket.DisconnectReason): void {
    console.log(`Disconnected from server: ${reason}`);
    this.setPodData((d) => ({
      ...d,
      connected: false,
      state: State.Disconnected,
    }));
  }

  private onData(data: Partial<PodData>): void {
    console.log("server says", data);
    this.setPodData((d) => ({ ...d, ...data }));
  }

  private onFault(data: string): void {
    console.error("Server fault with message:", data);
    this.addMessage(data, State.Fault);
  }

  private addMessage(response: string, newState: State): void {
    const timestamp = new Date();
    const newMessage = {
      timestamp,
      message: response,
    };

    this.setPodData((d) => ({
      ...d,
      state: newState,
      messages: [...d.messages, newMessage],
    }));
  }
}

export default PodSocketClient;
