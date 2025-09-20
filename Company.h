#ifndef COMPANY_H
#define COMPANY_H
#include <string.h>

class Company {
public:
    Company(const char* companyName, const char* companyStockName, float volatility, float shareValue);
    
    const char *name;
    const char *stockName;
    float m_shareValue;
    bool bankrupt = false;

    
    void setVolatility(float volatility);
    void changeShareValue(float weight);
    float getShareValue();
    
private:
    float m_volatility; // range 0-1
    
};

#endif