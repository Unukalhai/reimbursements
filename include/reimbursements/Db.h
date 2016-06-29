#include <ini.hpp>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <sstream>

using namespace std;

class Db
{
public:
    Db();
    ~Db();
    sql::ResultSet* query(string query);
    void execute(string query);

private:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
};
