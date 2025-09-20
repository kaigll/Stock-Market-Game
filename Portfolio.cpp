#include "Portfolio.h"
#include <string.h>

Portfolio::Portfolio(Market* market, unsigned int companyQuantity) : 
m_market(market), 
m_companyQuantity(companyQuantity) {
    for(int i = 0; i < companyQuantity; i++) sharesOwned[i] = 0;
} 

void Portfolio::purchaseShare(unsigned int companyIndex, unsigned int quantity, bool& outcome) {
    if ((m_market->company[companyIndex].getShareValue() * quantity) < balance && !m_market->company[companyIndex].bankrupt) {
        sharesOwned[companyIndex] += quantity;
        balance -= (m_market->company[companyIndex].getShareValue() * quantity);
        outcome = true;
    } else {
        outcome = false;
    }
}

void Portfolio::sellShare(unsigned int companyIndex, unsigned int quantity, bool& outcome) {
    if (sharesOwned[companyIndex] >= quantity) {
        sharesOwned[companyIndex] -= quantity;
        balance += (m_market->company[companyIndex].getShareValue() * quantity);
        outcome = true;
    } else {
        outcome = false;
    }
}

float Portfolio::getBalance() {
    return balance;
}

float Portfolio::getSharesValue() {
    float total;
    for (int i = 0; i < m_companyQuantity; i++) {
        total += sharesOwned[i] * m_market->company[i].getShareValue();
    }
    return total;
}

unsigned int Portfolio::getSharesOwned(unsigned int index) {
    return sharesOwned[index];
}