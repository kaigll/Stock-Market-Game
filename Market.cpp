#include "Market.h"


void Market::breakingNews(Company company, char (& string)[], float weight, float volatility) {
    company.setVolatility(volatility);
    company.changeShareValue(-1);
}

void Market::negativeNews (Company company, char (& string)[], float weight) {
    company.changeShareValue(-weight);
}

void Market::positiveNews(Company company, char (& string)[], float weight) {
    company.changeShareValue(weight);
}



/*
void Market::InitialiseCompanies() {
    Company Tesla((char*)"Tesla", 1, 187.71);
    Company AMD((char*)"AMD", 0.3, 82.11);
    Company Intel((char*)"Intel", 0.2, 25.53);
    Company NVIDEA((char*)"NVIDIA", 0.3, 232.88);
}
*/