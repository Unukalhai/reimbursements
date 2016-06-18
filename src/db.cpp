#include <reimbursements/db.h>

db::db() {
    db::driver = get_driver_instance();
    db::con = db::driver->connect("tcp://127.0.0.1:3306", "root", "");
    db::con->setSchema("auth");
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

