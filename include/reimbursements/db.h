#include <sstream>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <ini.hpp>

class db {

public:
    db();
    ~db();
    sql::ResultSet* query(std::string query);
    void execute(std::string query);
private:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
};
