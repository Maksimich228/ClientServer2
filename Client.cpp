#include <iostream>
#include <string>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::string message;
        while (std::getline(std::cin, message)) {
            boost::asio::write(socket, boost::asio::buffer(message));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}