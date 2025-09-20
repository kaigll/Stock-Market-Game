#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Company.h"
#include "NewsHandler.h"

class Market {
public:
    // not the best way - but a simple way is to store my company information in the market header
    Market(NewsHandler* newsHandler) : company {
        Company((char*)"Tesla", (char*)"TSLA", 1.0f, 187.71f),
        Company((char*)"AMD", (char*)"AMD", 0.3f, 82.11f),
        Company((char*)"Intel", (char*)"INTC", 0.2f, 25.53f),
        Company((char*)"NVIDIA", (char*)"NVDA", 0.3f, 232.88f)
    }, m_newsHandler(newsHandler) {}
    Company company[4];
    const int companyQuantity = 4;
    
    void activate();
    void deactivate();
    
private:
    NewsHandler* m_newsHandler;

    bool active = false;

    Thread newsTrigger_thread;

    void newsTriggerCallback();
    void newsTrigger();
    void randomIntervalTrigger();
};

#endif // CONSTANTS_H