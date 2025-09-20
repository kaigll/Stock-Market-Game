#include "NewsHandler.h"
#include "mbed.h"
#include "string.h"


void NewsHandler::breakingNews(Company company) {
    unsigned int randInt = rand() % newsTextBreakingQuantity;
    news[0]= '\0';
    insertCompanyInString(company, newsTextBreaking[randInt], news);
    m_display->scrollingPrint(news, 0, 300ms);
}

void NewsHandler::bankruptcyNews(Company company) {
    news[0]= '\0';
    insertCompanyInString(company, "$ goes bankrupt and shares are left valueless", news);
    m_display->scrollingPrint(news, 0, 300ms);
}

void NewsHandler::negativeNews (Company company, float weight) {
    unsigned int randInt = rand() % newsTextNegativeQuantity;
    news[0]= '\0';
    insertCompanyInString(company, newsTextNegative[randInt], news);
    m_display->scrollingPrint(news, 0, 300ms);
}

void NewsHandler::positiveNews(Company company, float weight) {
    unsigned int randInt = rand() % newsTextPositiveQuantity;
    news[0]= '\0';
    insertCompanyInString(company, newsTextPositive[randInt], news);
    m_display->scrollingPrint(news, 0, 300ms);
}

void NewsHandler::insertCompanyInString(Company company, const char* string, char* exitString) {
    int length = strlen(string);
    int companyNameIndex;
    bool foundNameLocation = false;
    for (int i = 0; i < length; i++) {
        if (string[i] == '$') { 
            companyNameIndex = i;
            foundNameLocation = true;
            break;
        }
    }
    if (foundNameLocation) {
        char displayString[length + strlen(company.name) - 1];
        displayString[0] = '\0';
        if (companyNameIndex > 0) {
            char stringStart[companyNameIndex-1];
            for (int i = 0; i < companyNameIndex; i++) {
                stringStart[i] = string[i];
            }
            stringStart[companyNameIndex] = '\0';
            strcat(displayString, stringStart);
        }
        char stringEnd[length-(companyNameIndex + 1)];
        for (int i = (companyNameIndex + 1); i < length; i++) {
            stringEnd[i - (companyNameIndex + 1)] = string[i];
        }
        stringEnd[length-(companyNameIndex + 1)] = '\0';

        strcat(displayString, company.name);
        strcat(displayString, stringEnd);

        strcpy(exitString, displayString);
    }
}