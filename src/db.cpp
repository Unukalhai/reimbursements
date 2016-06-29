#include <reimbursements/db.h>

db::db() {
    std::stringstream ss;
    std::ifstream file("config.ini");
    if (file.is_open()) {
        ss << file.rdbuf();
        file.close();
    }
    else throw std::runtime_error("Unable to open configuration file");
    INI::Parser config(ss);

    db::driver = get_driver_instance();
    db::con = db::driver->connect(config.top()["hostname"], config.top()["username"], config.top()["password"]);
    db::con->setSchema(config.top()["database"]);
    db::stmt = con->createStatement();
}

db::~db() {
    //delete db::driver;
    /*
     * /usr/include/cppconn/driver.h:39:10: error: ‘virtual sql::Driver::~Driver()’ is protected
  virtual ~Driver() {}
          ^
/home/unukalhai/Desktop/cpr/reimbursements/db.cpp:19:16: error: within this context
     delete db::driver;
                ^
                */
    delete db::stmt;
    delete db::con;
}

sql::ResultSet* db::query(std::string query) {
    return db::stmt->executeQuery(query);
}

void db::execute(std::string query) {
    db::stmt->execute(query);
}

