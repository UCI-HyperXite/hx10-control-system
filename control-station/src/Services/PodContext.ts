import { createContext } from "react";
import  { PodData } from "./PodSocketClient";
import PodSocketClient from "./PodSocketClient";

interface PodContextType {
  podSocketClient: PodSocketClient;
  podData: PodData;
}

export const PodContext = createContext<PodContextType | undefined>(undefined);
