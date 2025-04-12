import { createContext } from "react";
import  { PodData } from "./PodSocketClient";
import PodSocketClient from "./PodSocketClient";

interface PodContextType {
  podSocketClient: PodSocketClient;
  podData: PodData;
}

// export const PodContext = createContext<PodContextType | undefined>(undefined);

const PodContext = createContext<PodContextType>({
	podSocketClient: {} as PodSocketClient,
	podData: {} as PodData,
});

export default PodContext;