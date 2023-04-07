#include "Company.h"

class Market {
public:
    Market();
    void InitialiseCompanies();
    void breakingNews(Company company, char (& string)[], float weight, float volatility); 
    void negativeNews(Company company, char (& string)[], float weight);
    void positiveNews(Company company, char (& string)[], float weight);
};