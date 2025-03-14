#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
// #include "../include/components/cpp/signal_light.hpp"
#include "signal_light.hpp"
#include <iostream>
#include <future>
#include <thread>
#include <unordered_map>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = net::ip::tcp;

const int PORT = 8080;
std::unordered_map<std::string, websocket::stream<tcp::socket>> clients;

// Initialize SignalLight
SignalLight signalLight;

void update_led_strip(uint32_t color)
{
    for (int i = 0; i < LED_NUMBER; i++)
    {
        color_matrix[i] = color;
    }
    signalLight.sendColors(color_matrix);
}

void handle_message(websocket::stream<tcp::socket> &ws, const std::string &msg)
{
    std::cout << "Received: " << msg << std::endl;
    json::object response;

    if (msg == "run")
    {
        response["status"] = "success";
        response["state"] = "Running";
        update_led_strip(0x200000); // Green
    }
    else if (msg == "stop" || msg == "halt")
    {
        response["status"] = "success";
        response["state"] = msg == "stop" ? "Stopped" : "Halted";
        update_led_strip(0x002000); // Red
    }
    else if (msg == "load")
    {
        response["status"] = "success";
        response["state"] = "Loading";
        update_led_strip(0x202000); // Yellow
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Unknown command";
    }

    ws.write(net::buffer(json::serialize(response)));
}

void handle_websocket(tcp::socket socket)
{
    try
    {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        std::cout << "WebSocket client connected.\n";

        beast::flat_buffer buffer;
        while (true)
        {
            ws.read(buffer);
            std::string received_msg = beast::buffers_to_string(buffer.data());
            buffer.consume(buffer.size()); // Clear buffer after reading

            handle_message(ws, received_msg);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "WebSocket Error: " << e.what() << std::endl;
    }
}

int main()
{
    if (wiringPiSetup() == -1)
    {
        std::cerr << "WiringPi setup failed!" << std::endl;
        return 1;
    };
    pinMode(LED_PIN, OUTPUT);
    try
    {
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), PORT));

        std::cout << "WebSocket Server is running on port " << PORT << "...\n";

        while (true)
        {
            tcp::socket socket(ioc);
            acceptor.accept(socket);
            std::thread(handle_websocket, std::move(socket)).detach();
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Server error: " << e.what() << "\n";
    }

    return 0;
}
