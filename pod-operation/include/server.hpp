#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <string>

class Server {
public:
    Server(int port);
    
    // Initializes the server and waits for one client connection
    void initialize();

    // Sends JSON string to connected client
    void send_json(const std::string& json_data);

private:
    int port_;
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::socket> client_socket_;
};

#endif // SERVER_HPP

