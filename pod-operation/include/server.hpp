#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>

class Server {
public:
    explicit Server(int port);
    void initialize();

private:
    void do_accept();
    void do_session(boost::beast::tcp_stream&& stream);

    int port_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif // SERVER_HPP

