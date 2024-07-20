#include <iostream>
#include <string>
#include <boost/asio.hpp>

void handle_client(boost::asio::ip::tcp::socket& socket) {
    try {
        while (true) {
            char data[512];
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; 
            else if (error)
                throw boost::system::system_error(error); 

            std::cout << "Received: " << data << std::endl;

            
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));

        while (true) {
            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::thread(handle_client, std::move(socket)).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}