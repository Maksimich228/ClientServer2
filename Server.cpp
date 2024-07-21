#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <mysql/mysql.h>

void save_message_to_db(const std::string& message) {
    MYSQL* conn = mysql_init(NULL);
    if (conn && mysql_real_connect(conn, "localhost", "user", "password", "chat_db", 0, NULL, 0)) {
        std::string query = "INSERT INTO messages (content) VALUES ('" + message + "')";
        mysql_query(conn, query.c_str());
        mysql_close(conn);
    }
}

std::vector<std::string> get_users_from_db() {
    std::vector<std::string> users;
    MYSQL* conn = mysql_init(NULL);
    if (conn && mysql_real_connect(conn, "localhost", "user", "password", "chat_db", 0, NULL, 0)) {
        if (mysql_query(conn, "SELECT username FROM users") == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                users.push_back(row[0]);
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    }
    return users;
}

std::vector<std::string> get_messages_from_db() {
    std::vector<std::string> messages;
    MYSQL* conn = mysql_init(NULL);
    if (conn && mysql_real_connect(conn, "localhost", "user", "password", "chat_db", 0, NULL, 0)) {
        if (mysql_query(conn, "SELECT content FROM messages") == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                messages.push_back(row[0]);
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    }
    return messages;
}

void handle_client(boost::asio::ip::tcp::socket socket) {
    try {
        char data[512];
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(data), error);
        if (error) return;

        std::string message(data, length);
        save_message_to_db(message);

        std::vector<std::string> users = get_users_from_db();
        std::vector<std::string> messages = get_messages_from_db();

        std::string response = "Users:\n";
        for (const auto& user : users) {
            response += user + "\n";
        }

        response += "Messages:\n";
        for (const auto& msg : messages) {
            response += msg + "\n";
        }

        boost::asio::write(socket, boost::asio::buffer(response));
    } catch (std::exception& e) {
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
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
