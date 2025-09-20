#pragma once

#include "Company.h"
#include "Display.h"

class NewsHandler {
public:
    void breakingNews(Company company); 
    void bankruptcyNews(Company company); 
    void negativeNews(Company company, float weight);
    void positiveNews(Company company, float weight);

    NewsHandler(Display* display) : m_display(display),
    newsTextNegative {
        (char*)"$ CEO tweets controversial political takes.",
        (char*)"Products by $ begin to fall behind the competition",
        (char*)"The new product by $ is a commercial failure."
    }, newsTextPositive {
        (char*)"$ commit to being fully carbon neutral in the next 5 years",
        (char*)"The new product from $ is hit amongst consumers"
    }, newsTextBreaking {
        (char*)"BREAKING: $ found to have stolen product designs from competitors.",
        (char*)"BREAKING: $ CEO arrested and charged with embezzlement"
    } {};
private:
    Display* m_display;
    const char* newsTextBreaking[2];
    const int newsTextBreakingQuantity = 1;
    const char* newsTextNegative[3];
    const int newsTextNegativeQuantity = 3;
    const char* newsTextPositive[2];
    const int newsTextPositiveQuantity = 1;

    char news[80];

    void insertCompanyInString(Company company, const char* string, char* exitString);
};