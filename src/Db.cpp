#include <reimbursements/Db.h>

Db::Db()
{
    stringstream ss;
    ifstream file("config.ini");
    if(file.is_open())
    {
        ss << file.rdbuf();
        file.close();
    }
    else throw runtime_error("Unable to open configuration file");
    INI::Parser config(ss);

    Db::driver = get_driver_instance();
    Db::con = Db::driver->connect(config.top()["hostname"], config.top()["username"], config.top()["password"]);
    Db::con->setSchema(config.top()["database"]);
    Db::stmt = con->createStatement();
}

Db::~Db()
{
    //delete Db::driver;
    /*
     * /usr/include/cppconn/driver.h:39:10: error: ‘virtual sql::Driver::~Driver()’ is protected
  virtual ~Driver() {}
          ^
/home/unukalhai/Desktop/cpr/reimbursements/Db.cpp:19:16: error: within this context
     delete Db::driver;
                ^
                */
    delete Db::stmt;
    delete Db::con;
}

sql::ResultSet* Db::query(string query)
{
    return Db::stmt->executeQuery(query);
}

void Db::execute(string query)
{
    Db::stmt->execute(query);
}

