#include <reimbursements/price.h>

void price::fillItems() {
    sql::ResultSet *res;
    std::stringstream errMsg;
    std::vector<int> insurance_id;
    try {
        res = price::database.query("SELECT id, name, type FROM prices");
    }
    catch(sql::SQLException &e) {
        errMsg << "# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
        errMsg << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )";
        throw std::runtime_error(errMsg.str());
    }
    while (res->next()) {
        price::item item;
        std::string i_id = res->getString("id");
        std::string i_type = res->getString("type");
        item.id = std::stoi(i_id);
        item.type = std::stoi(i_type);
        std::string name = res->getString("name");
        if(item.type == 1 || item.type == 2) insurance_id.push_back(item.id);
        unsigned int crest_price = 0;
        try {
            if (name == "") name = cr.getName(item.id);
            /* 10000002 - The Forge
             * 60003760 - Jita IV - Moon 4 - Caldari Navy Assembly Plant */
            if(item.type == 2) { //if capital -> search in region
                crest_price = cr.getMarketPrice(item.id, 10000002 , "sell", 0);
            }
            else {
                crest_price = cr.getMarketPrice(item.id, 10000002 , "sell", 60003760);
            }

        }
        catch(std::exception& e) {
            throw std::runtime_error(e.what());
        }
        item.name = name;
        item.price = crest_price;
        price::items.push_back(item);
    }
    price::fillInsurances(&insurance_id[0]);
}

void price::updatePrice() {
    price::fillItems();
    std::stringstream errMsg;
    try {
        for(price::item item : price::items) {
            std::string query_insurance = (item.type == 1 || item.type == 2) ? ", insurance_cost='" +  std::to_string(item.insurance_cost)
                                                                               + "', insurance_payout='" +  std::to_string(item.insurance_payout) + "'": "";
            std::string query = "UPDATE prices SET name='" + item.name + "', price='" + std::to_string(item.price) + "'"
                                + query_insurance + " WHERE id='" + std::to_string(item.id) + "'";
            price::database.execute(query);
        }
    }
    catch(sql::SQLException &e) {
        errMsg << "# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
        errMsg << "# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )";
        throw std::runtime_error(errMsg.str());
    }
}

void price::fillInsurances(int *ids) {
    std::list<crest::insurance> insurances;
    int s = sizeof(ids) / sizeof(int);
    insurances = cr.getInsurance(ids, s, "Platinum");
    for(std::list<price::item>::iterator item = price::items.begin(); item != price::items.end(); ++item) {
        price::item & it(*item);
        for(crest::insurance insurance : insurances) {
            if(insurance.id == it.id) {
                it.insurance_cost = insurance.cost;
                it.insurance_payout = insurance.payout;
            }
        }
    }
}
