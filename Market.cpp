#include "mbed.h"
#include "Market.h"

void Market::activate() {
    active = true;
    newsTrigger_thread.start(callback(this, &Market::newsTrigger));
}

void Market::deactivate() {
    active = false;
}

void Market::newsTrigger() {
    while (true) {
        int delay = rand() % 20000 + 10000;
        thread_sleep_for(delay);

        int newsRand = rand() % 10;

        int companyRand;
        do {
            companyRand = rand() % companyQuantity;
        } while (company[companyRand].bankrupt);

        float weight = rand()/(float)RAND_MAX;
        
        if (newsRand < 5) {
            m_newsHandler->positiveNews(company[companyRand], weight);
            company[companyRand].changeShareValue(weight);
        } else if (newsRand < 9) {
            m_newsHandler->negativeNews(company[companyRand], weight);
            company[companyRand].changeShareValue(-weight);
            if (company[companyRand].bankrupt) m_newsHandler->bankruptcyNews(company[companyRand]);
        } else {
            m_newsHandler->breakingNews(company[companyRand]);
            company[companyRand].setVolatility(1);
            company[companyRand].changeShareValue(-1);
            if (company[companyRand].bankrupt) m_newsHandler->bankruptcyNews(company[companyRand]);
        }
        
    }
}