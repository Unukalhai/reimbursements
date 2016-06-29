#include <reimbursements/Price.h>

int main(void)
{
    try
    {
        Price p;
        p.updatePrice();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}