#include <sstream>
#include <reimbursements/db.h>
#include <crest/crest.h>

class price {
public:
    struct item{
        int id = 0, type;
        unsigned int price = 0, insurance_cost = 0, insurance_payout = 0;
        std::string name;
        /* type = 0 - module
         * type = 1 - subcapital
         * type = 2 - capital */
    };

    void updatePrice();

private:
    db database;
    crest cr;
    std::list<item> items;

    void fillItems();
    void fillInsurances(int* ids);
};
