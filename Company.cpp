#include "Company.h"
#include "mbed.h"

Company::Company(char* companyName, float volatility, double shareValue) : name(companyName), m_volatility(volatility), m_shareValue(shareValue) {}

// set the volatility, which is the maximum value of change avaliable due to random variable.
void Company::setVolatility(float volatility) {
   m_volatility = volatility;
}

// changes share value by adding "amount" to the total, if the change makes the value 0 or below the bankrupt() method is called.
void Company::changeShareValue(float weight) {
    float randomf = rand()/(float)RAND_MAX;
    float adjustedVolatility = m_volatility*randomf;
    double amount = m_shareValue*weight*adjustedVolatility;

    if ((m_shareValue - amount) >= 0) {
        m_shareValue += amount;
    } else {
        m_shareValue = 0;
        bankrupt();
    }
}

void Company::bankrupt() {
    // send message to news and screen to display bankruptcy, and prevent user from buying this stock
}