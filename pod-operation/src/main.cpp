// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/version.hpp>
// #include <boost/asio.hpp>
// #include <boost/json.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <future>
#include <thread>
#include <cmath>

#include "seven_segment_display.hpp"

std::future<void> main() {
    
}

void initializeDisplay(HT16K33& display, const std::string& label) {
    const uint8_t brightness = 10;
    const HT16K33::BlinkFreq_e blink = HT16K33::BlinkFreq_e::BlinkOff;
    const uint8_t digits = 4;
    const HT16K33::DisplayType_e type = HT16K33::DisplayType_e::SegType7;

    display.DisplayInit(brightness, blink, digits, type);
    display.DisplayOn();
}

void updateDisplay(HT16K33& display, const std::string& label) {
    for (uint8_t pos = 0; pos < 4; ++pos) {
        char randomChar = '0' + (std::rand() % 10);
        rdlib::Return_Codes_e result = display.displayChar(pos, randomChar, HT16K33plus_Model1::DecimalPoint_e::DecPointOff);

        if (result != rdlib::Success) {
            std::cerr << label << ": Error displaying character at position "
                      << static_cast<int>(pos) << ": Code "
                      << display.DisplayI2CErrorGet() << std::endl;
        }
    }
}

int main() {
    HT16K33 display(1, 0x70, 0);
    initializeDisplay(display, "pressure");

    while (true){
        update Display(display, "pressure");
    }

}


// namespace beast = boost::beast;
// namespace http = beast::http;
// namespace net = boost::asio;
// namespace json = boost::json;
// using tcp = net::ip::tcp;

// const int PORT = 8080;

// double get_speed()
// {
//     return rand() % 100 + (rand() % 100) / 100.0;
// }

// double get_acceleration()
// {
//     return ((rand() % 200) - 100) / 10.0;
// }

// std::string generate_json_response()
// {
//     json::object obj;
//     obj["status"] = "success";
//     obj["speed"] = get_speed();
//     obj["acceleration"] = get_acceleration();
//     return json::serialize(obj);
// }

// void handle_client(tcp::socket socket)
// {
//     try
//     {
//         beast::flat_buffer buffer;
//         http::request<http::string_body> req;
//         http::read(socket, buffer, req);
//         std::string json_response = generate_json_response();
//         http::response<http::string_body> res{http::status::ok, req.version()};
//         res.set(http::field::content_type, "application/json");
//         res.set(http::field::access_control_allow_origin, "*");
//         res.body() = json_response;
//         res.prepare_payload();
//         http::write(socket, res);
//         socket.shutdown(tcp::socket::shutdown_send);
//     }
//     catch (std::exception &e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
// }

// int main()
// {
//     try
//     {
//         net::io_context ioc;
//         tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), PORT));
//         std::cout << "Server is running on port " << PORT << "...\n";
//         while (true)
//         {
//             tcp::socket socket(ioc);
//             acceptor.accept(socket);
//             std::cout << "Client connected.\n";
//             std::thread(handle_client, std::move(socket)).detach();
//         }
//     }
//     catch (std::exception &e)
//     {
//         std::cerr << "Server error: " << e.what() << "\n";
//     }
//     return 0;
// }
