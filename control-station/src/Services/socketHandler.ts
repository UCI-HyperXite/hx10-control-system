const SERVER_URL = "ws://localhost:8080";

export const createWebSocket = (namespace: string): WebSocket => {
  const socket = new WebSocket(`${SERVER_URL}/${namespace}`);

  socket.onopen = () => {
    console.log(`Connected to WebSocket namespace: ${namespace}`);
  };

  socket.onclose = (event) => {
    console.log(
      `Disconnected from WebSocket namespace: ${namespace}`,
      event.reason
    );
  };

  socket.onerror = (event) => {
    console.error(`WebSocket error in namespace: ${namespace}`, event);
  };

  return socket;
};
