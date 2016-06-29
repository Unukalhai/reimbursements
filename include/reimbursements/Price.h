#include <reimbursements/Db.h>

#include <crest/crest.h>

#include <sstream>

using namespace std;

class Price
{
public:
    struct item
    {
        int id = 0, type;
        unsigned int price = 0, insurance_cost = 0, insurance_payout = 0;
        string name;
        /* type = 0 - module
         * type = 1 - subcapital
         * type = 2 - capital */
    };

    void updatePrice();

private:
    Db database;
    crest cr;
    list<item> items;

    void fillItems();
    void fillInsurances(int* ids);
};
