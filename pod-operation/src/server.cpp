#include "../include/server.hpp"
#include <boost/json.hpp>
#include <iostream>

using boost::asio::ip::tcp;

Server::Server(int port)
    : port_(port), client_socket_(nullptr) {}

void Server::initialize()
{
    try
    {
        tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), port_));
        std::cout << "Server listening on port " << port_ << "...\n";

        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor.accept(*socket);

        std::cout << "Client connected from " << socket->remote_endpoint() << "\n";
        client_socket_ = socket;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server initialization error: " << e.what() << "\n";
    }
}

void Server::send_json(const boost::json::value& json_data)
{
    if (client_socket_ && client_socket_->is_open())
    {
        try
        {
            std::string serialized = boost::json::serialize(json_data);
            boost::asio::write(*client_socket_, boost::asio::buffer(serialized));
            std::cout << "Sent JSON to client.\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error sending JSON: " << e.what() << "\n";
        }
    }
    else
    {
        std::cerr << "No client connected.\n";
    }
}

void Server::send_json(const boost::json::object& obj)
{
    send_json(boost::json::value(obj));
}
