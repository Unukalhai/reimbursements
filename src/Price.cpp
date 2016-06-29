#include <reimbursements/Price.h>

void Price::fillItems()
{
    sql::ResultSet *res;
    stringstream errMsg;
    vector<int> insurance_id;
    try
    {
        res = Price::database.query("SELECT id, name, type FROM prices");
    }
    catch(sql::SQLException &e)
    {
        errMsg << "# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
        errMsg << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )";
        throw runtime_error(errMsg.str());
    }
    while(res->next())
    {
        Price::item item;
        string i_id = res->getString("id");
        string i_type = res->getString("type");
        item.id = stoi(i_id);
        item.type = stoi(i_type);
        string name = res->getString("name");
        if(item.type == 1 || item.type == 2) insurance_id.push_back(item.id);
        unsigned int crest_price = 0;
        try
        {
            if (name == "") name = Price::cr.getName(item.id);
            /* 10000002 - The Forge
             * 60003760 - Jita IV - Moon 4 - Caldari Navy Assembly Plant */
            if(item.type == 2) //if capital -> search in region
            {
                crest_price = Price::cr.getMarketPrice(item.id, 10000002 , "sell", 0);
            }
            else
            {
                crest_price = Price::cr.getMarketPrice(item.id, 10000002 , "sell", 60003760);
            }
        }
        catch(exception& e)
        {
            errMsg << "# ERR: CREST Exception in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
            errMsg << "# ERR: " << e.what();
            throw runtime_error(errMsg.str());
        }
        item.name = name;
        item.price = crest_price;
        Price::items.push_back(item);
    }
    Price::fillInsurances(&insurance_id[0]);
}

void Price::updatePrice()
{
    Price::fillItems();
    stringstream errMsg;
    try
    {
        for(Price::item item : Price::items)
        {
            string query_insurance = (item.type == 1 || item.type == 2) ? ", insurance_cost='"
                + to_string(item.insurance_cost) + "', insurance_payout='" +  to_string(item.insurance_payout)
                + "'" : "";
            string query = "UPDATE prices SET name='" + item.name + "', Price='" + to_string(item.price) + "'"
                + query_insurance + " WHERE id='" + to_string(item.id) + "'";
            Price::database.execute(query);
        }
    }
    catch(sql::SQLException &e)
    {
        errMsg << "# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
        errMsg << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )";
        throw runtime_error(errMsg.str());
    }
}

void Price::fillInsurances(int *ids)
{
    list<Crest::insurance> insurances;
    int s = sizeof(ids) / sizeof(int); //maybe wrong...
    insurances = cr.getInsurance(ids, s, "Platinum");
    for(list<Price::item>::iterator item = Price::items.begin(); item != Price::items.end(); ++item)
    {
        Price::item & it(*item);
        for(Crest::insurance insurance : insurances)
        {
            if(insurance.id == it.id)
            {
                it.insurance_cost = insurance.cost;
                it.insurance_payout = insurance.payout;
            }
        }
    }
}
