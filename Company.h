#include <string.h>

class Company {
    float m_volatility;
    double m_shareValue;
    

public:
    Company(char companyName[], float volatility, double shareValue);
    
    char *name;
    
    void setVolatility(float volatility);
    void changeShareValue(float weight);
    void bankrupt();
};