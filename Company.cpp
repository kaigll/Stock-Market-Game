#include "Company.h"
#include "mbed.h"

Company::Company(const char* companyName, const char* companyStockName, float volatility, float shareValue) : 
name(companyName), 
stockName(companyStockName), 
m_volatility(volatility), 
m_shareValue(shareValue) {
    if (shareValue <= 0) bankrupt = true;
}

// set the volatility, which is the maximum value of change avaliable due to random variable.
void Company::setVolatility(float volatility) {
   m_volatility = volatility;
}

// changes share value by adding "amount" to the total
void Company::changeShareValue(float weight) {
    float randomf = rand()/(float)RAND_MAX;
    float adjustedVolatility = m_volatility*randomf;
    float amount = m_shareValue*weight*adjustedVolatility;
    if (amount < 0.05f) amount = 0.05f; // stop values too low
    if ((m_shareValue - amount) >= 0.01f) {
        m_shareValue += amount;
    } else {
        m_shareValue = 0;
        bankrupt = true;
    }
}

float Company::getShareValue() {
    return m_shareValue;
}