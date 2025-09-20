#include "mbed.h"
#include "Market.h"
#include "NewsHandler.h"
#include "Display.h"
#include "Portfolio.h"

Display display;
NewsHandler news(&display);
Market market(&news);
Portfolio portfolio(&market, market.companyQuantity);

DigitalOut greenLed(D6);
DigitalOut redLed(D7);
InterruptIn acceptButton(D8);
InterruptIn leftButton(D9);
InterruptIn rightButton(D10);

Thread acceptThread;
Semaphore acceptSemaphore(0, 1);
Thread leftThread;
Semaphore leftSemaphore(0, 1);
Thread lerightThread;
Semaphore rightSemaphore(0, 1);

Thread displayThread;
Semaphore displaySemaphore(0, 1);

Timer pageTimer;

bool buttonsEnable = false;

unsigned int buySellAmount = 0;

Timeout debounceTimeout;

enum Page {
    Portfolio,
    CompanyInfo,
    BuySell,
    Buy,
    Sell,
    Purchased,
    Sold,
    Error
};

Page pageCurrent;
unsigned int companyPageIndex;

void formatMoney(float buySellAmount, char* result) {
    snprintf(result, 8, "$%.2f", buySellAmount);
}


void portfolioPageFunction() {
    char displayString[16];
    char balance[8];
    char value[8];
    formatMoney(portfolio.getBalance(), balance);
    sprintf(displayString, "Bal:%s", balance);
    display.lowerDisplay(displayString);
    bool sharesDisplayed = false;
    while (pageCurrent == Portfolio) {
        pageTimer.start();
        if (pageTimer.elapsed_time() >= 3s  && pageTimer.elapsed_time() < 6s && !sharesDisplayed) {
            formatMoney(portfolio.getSharesValue(), value);
            sprintf(displayString, "Shares:%s", value);
            display.lowerDisplay(displayString);
            sharesDisplayed = true;
        } else if (pageTimer.elapsed_time() >= 6s) {
            formatMoney(portfolio.getBalance(), balance);
            sprintf(displayString, "Bal:%s", balance);
            display.lowerDisplay(displayString);
            pageTimer.stop();
            pageTimer.reset();
            sharesDisplayed = false;
        }
    }
}

void companyInfoPageFunction() {
    char displayString[16];
    char value[8];
    while (pageCurrent == CompanyInfo) {
        pageTimer.start();
        if (pageTimer.elapsed_time() >= 300ms) {
            formatMoney(market.company[companyPageIndex].getShareValue(), value);
            snprintf(displayString, 16, "%s:%s|%i", market.company[companyPageIndex].stockName, value, portfolio.getSharesOwned(companyPageIndex));
            display.lowerDisplay(displayString);
            pageTimer.stop();
            pageTimer.reset();
        }
    }
}

void buyPageFunction() {
    char displayString[16];
    while (pageCurrent == Buy) {
        pageTimer.start();
        if (pageTimer.elapsed_time() >= 30ms) {
            sprintf(displayString, "Buy amount: %i", buySellAmount);
            display.lowerDisplay(displayString);
            pageTimer.stop();
            pageTimer.reset();
        }
    }
}

void sellPageFunction() {
    char displayString[16];
    while (pageCurrent == Sell) {
        pageTimer.start();
        if (pageTimer.elapsed_time() >= 30ms) {
            sprintf(displayString, "Sell amount: %i", buySellAmount);
            display.lowerDisplay(displayString);
            pageTimer.stop();
            pageTimer.reset();
        }
    }
}

void purchasedPageFunction() {
    char displayString[16];
    while (pageCurrent == Purchased) {
        pageTimer.start();
        if (pageTimer.elapsed_time() >= 30ms) {
            sprintf(displayString, "Sell amount: %i", buySellAmount);
            display.lowerDisplay(displayString);
            pageTimer.stop();
            pageTimer.reset();
        }
    }
}

void soldPageFunction() {
    char displayString[16];
    while (pageCurrent == Sold) {
        pageTimer.start();
        if (pageTimer.elapsed_time() >= 30ms) {
            sprintf(displayString, "Sell amount: %i", buySellAmount);
            display.lowerDisplay(displayString);
            pageTimer.stop();
            pageTimer.reset();
        }
    }
}

void ShowPage() {
    while (true) {
        displaySemaphore.acquire();
        char displayString[80];
        switch(pageCurrent) {
            case Page::Portfolio:
                pageTimer.stop();
                pageTimer.reset();
                portfolioPageFunction();
                break;
            case Page::CompanyInfo:
                pageTimer.stop();
                pageTimer.reset();
                companyInfoPageFunction();
                break;
            case Page::BuySell:
                pageTimer.stop();
                pageTimer.reset();
                display.lowerDisplay((char*)"L: Buy | R: Sell");
                break;
            case Page::Buy:
                pageTimer.stop();
                pageTimer.reset();
                buyPageFunction();
                break;    
            case Page::Sell:
                pageTimer.stop();
                pageTimer.reset();
                sellPageFunction();
                break;
            case Page::Purchased:
                pageTimer.stop();
                pageTimer.reset();
                sprintf(displayString, "Successfully purchased %i shares of %s", buySellAmount, market.company[companyPageIndex].stockName);
                display.scrollingPrint(displayString, 1, 150ms);
                break;
            case Page::Sold:
                pageTimer.stop();
                pageTimer.reset();
                char shareValue[8];
                formatMoney(market.company[companyPageIndex].getShareValue(), shareValue);
                if (buySellAmount == 1) sprintf(displayString, "Successfully sold 1 share of %s for %s", market.company[companyPageIndex].stockName, shareValue);
                else {
                    char totalValue[8];
                    formatMoney(market.company[companyPageIndex].getShareValue()*buySellAmount, totalValue);
                    sprintf(displayString, "Successfully sold %i shares of %s for %s each (total: %s)", buySellAmount, market.company[companyPageIndex].stockName, shareValue, totalValue);
                }
                display.scrollingPrint(displayString, 1, 150ms);
                break;
            case Page::Error:
                pageTimer.stop();
                pageTimer.reset();
                display.lowerDisplay((char*)"-----Error-----");
                break;
            default:
                printf("pageCurrent invalid value\n");
                break;
        }
    }
}

void ChangePage(Page page) {
    if (pageCurrent != page) {
        pageCurrent = page;
        displaySemaphore.release();
    }
}

// had issues with button repeats so simple debounce for them
void enableButtons() { buttonsEnable = true; }
void startDebounce() { buttonsEnable = false; debounceTimeout.attach(enableButtons, 200ms); }

void acceptCallback() { acceptSemaphore.release(); }
void leftCallback() { leftSemaphore.release(); }
void rightCallback() { rightSemaphore.release(); }

void acceptFunction() {
    while (true) {
        acceptSemaphore.acquire();
        if (buttonsEnable) {
            printf("accept pressed\n");
            if (pageCurrent == Page::CompanyInfo) {
                ChangePage(Page::BuySell);
            } else if (pageCurrent == Page::BuySell) {
                ChangePage(Page::CompanyInfo);
            } else if (pageCurrent == Page::Buy) {
                bool outcome;
                portfolio.purchaseShare(companyPageIndex, buySellAmount, outcome);
                if(outcome) { ChangePage(Page::Purchased); }
                else { ChangePage(Page::Error); }
                buySellAmount = 0;
                ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::Sell) {
                bool outcome;
                portfolio.sellShare(companyPageIndex, buySellAmount, outcome);
                if(outcome) { ChangePage(Page::Sold); }
                else { ChangePage(Page::Error); }
                buySellAmount = 0;
                ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::Purchased) {
                display.overrideScrollingLower = true;
                ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::Sold) {
                display.overrideScrollingLower = true;
                ChangePage(Page::Portfolio);
            }

            startDebounce();
        }
    }
}

void leftFunction() {
    while (true) {
        leftSemaphore.acquire();
        if (buttonsEnable) {
            printf("left pressed\n");
            if (pageCurrent == Page::Portfolio) {
                ChangePage(Page::CompanyInfo);
                companyPageIndex = market.companyQuantity-1;
            } else if (pageCurrent == Page::CompanyInfo) {
                if (companyPageIndex > 0) companyPageIndex--;
                else ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::BuySell) {
                if (!market.company[companyPageIndex].bankrupt) ChangePage(Page::Buy);
            } else if (pageCurrent == Page::Buy) {
                if (buySellAmount > 0) buySellAmount--;
            } else if (pageCurrent == Page::Sell) {
                if (buySellAmount > 0) buySellAmount--;
            } else if (pageCurrent == Page::Purchased) {
                display.overrideScrollingLower = true;
                ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::Sold) {
                display.overrideScrollingLower = true;
                ChangePage(Page::Portfolio);
            }
            startDebounce();
        }
    }
}

void rightFunction() {
    while (true) {
        rightSemaphore.acquire();
        if (buttonsEnable) {
            printf("right pressed %d\n", pageCurrent);
            if (pageCurrent == Page::Portfolio) {
                ChangePage(Page::CompanyInfo);
                companyPageIndex = 0;
            } else if (pageCurrent == Page::CompanyInfo) {
                if (companyPageIndex < market.companyQuantity-1) companyPageIndex++;// issue here
                else ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::BuySell) {
                if (0 < portfolio.getSharesOwned(companyPageIndex)) ChangePage(Page::Sell);
            } else if (pageCurrent == Page::Buy) {
                buySellAmount++;
            } else if (pageCurrent == Page::Sell) {
                if (buySellAmount < portfolio.getSharesOwned(companyPageIndex)) buySellAmount++;
            } else if (pageCurrent == Page::Purchased) {
                display.overrideScrollingLower = true;
                ChangePage(Page::Portfolio);
            } else if (pageCurrent == Page::Sold) {
                display.overrideScrollingLower = true;
                ChangePage(Page::Portfolio);
            }
            startDebounce();
        }
    }
}

int main()
{
    market.activate();

    buttonsEnable = false;

    acceptButton.rise(acceptCallback);
    leftButton.rise(leftCallback);
    rightButton.rise(rightCallback);

    acceptThread.start(&acceptFunction);
    leftThread.start(&leftFunction);
    lerightThread.start(&rightFunction);

    thread_sleep_for(50);
    buttonsEnable = true;

    displayThread.start(&ShowPage);

    // initialise starting page
    pageCurrent = Portfolio;
    displaySemaphore.release();

    while (true);
}