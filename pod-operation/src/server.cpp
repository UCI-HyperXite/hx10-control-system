#include "../include/server.hpp"
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace json = boost::json;

Server::Server(int port)
    : port_(port),
      acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {}

void Server::initialize()
{
    std::cout << "WebSocket server listening on port " << port_ << "...\n";
    do_accept();
    io_context_.run();
}

void Server::do_accept()
{
    acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::cout << "New connection: " << socket.remote_endpoint() << "\n";
            do_session(beast::tcp_stream(std::move(socket)));
        } else {
            std::cerr << "Accept error: " << ec.message() << "\n";
        }

        do_accept(); // Keep accepting new connections
    });
}

void Server::do_session(beast::tcp_stream&& stream)
{
    auto ws = std::make_shared<websocket::stream<beast::tcp_stream>>(std::move(stream));

    ws->async_accept([this, ws](beast::error_code ec) {
        if (ec) {
            std::cerr << "Handshake failed: " << ec.message() << "\n";
            return;
        }

        auto buffer = std::make_shared<beast::flat_buffer>();
        auto read_handler = std::make_shared<std::function<void(beast::error_code, std::size_t)>>();

        *read_handler = [this, ws, buffer, read_handler](beast::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "Read failed: " << ec.message() << "\n";
                return;
            }

            std::string msg = beast::buffers_to_string(buffer->data());
            std::cout << "Received: " << msg << "\n";

            // Parse and prepare response
            json::object response;
            if (msg == "run") response["state"] = "START";
            else if (msg == "stop") response["state"] = "STOP";
            else if (msg == "halt") response["state"] = "HALT";
            else response["error"] = "Unknown command";

            response["status"] = "received";

            // Send JSON response
            ws->async_write(net::buffer(json::serialize(response)),
                [ws](beast::error_code ec, std::size_t) {
                    if (ec) std::cerr << "Write error: " << ec.message() << "\n";
                });

            buffer->consume(bytes_transferred); // Clear buffer
            ws->async_read(*buffer, *read_handler); // Continue reading
        };

        // Initial read
        ws->async_read(*buffer, *read_handler);
    });
}

int main(){
  Server server(8080);
  server.initialize();
}
