#include <iostream>
#include <reimbursements/price.h>

using namespace std;

int main(void){
    try{
        price p;
        p.updatePrice();
    }
    catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }

  return 0;
}