#include "Company.h"
#include "Market.h"

class Portfolio {
public:
    Portfolio(Market* market, unsigned int companyQuantity);
    void purchaseShare(unsigned int companyIndex, unsigned int quantity, bool& outcome);
    void sellShare(unsigned int companyIndex, unsigned int quantity, bool& outcome);
    float getBalance();
    float getSharesValue();
    unsigned int getSharesOwned(unsigned int index);
private:
    Market* m_market;
    float balance = 1000.00;
    unsigned int sharesOwned[4]; // hard coded the company quantity for this as it caused a memory leak before.
    const unsigned int m_companyQuantity;
};