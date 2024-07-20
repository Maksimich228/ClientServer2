#include <mysql/mysql.h>

void save_message_to_db(const std::string& message) {
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
        return;
    }

    if (mysql_real_connect(conn, "localhost", "user", "password", "chat_db", 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return;
    }

    std::string query = "INSERT INTO messages (content) VALUES ('" + message + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "INSERT failed. Error: " << mysql_error(conn) << std::endl;
    }

    mysql_close(conn);
}