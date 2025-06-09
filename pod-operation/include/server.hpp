#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <memory>

class Server {
public:
    explicit Server(int port);
    void initialize();

    // Send boost::json objects or values
    void send_json(const boost::json::value& json_data);
    void send_json(const boost::json::object& obj);

private:
    int port_;
    boost::asio::io_context io_context_;
    std::shared_ptr<boost::asio::ip::tcp::socket> client_socket_;
};

#endif // SERVER_HPP

