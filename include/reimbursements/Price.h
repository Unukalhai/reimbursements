#include <reimbursements/Db.h>

#include <crest/Crest.h>

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
        /* type = 0, 1 - module
         * type = 2 - subcapital
         * type = 3 - capital */
    };

    void updatePrice();

private:
    Db database;
    Crest cr;
    list<item> items;

    void fillItems();
    void fillInsurances(int* ids);
};
