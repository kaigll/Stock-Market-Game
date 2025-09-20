#include "Display.h"


TextLCD lcd(D0, D1, D2, D3, D4, D5);


Display::Display() {
    memset(upperText, ' ', 16);
    upperText[16] = '\0';
    memset(lowerText, ' ', 16);
    lowerText[16] = '\0';
}

void Display::shiftLeft(char* string, int length) {
    for (int i = 0; i < length; i++) {
        string[i] = string[i + 1];
    }
    string[length] = '\0';
}

void Display::print() {
    //printf("Upper text: %s\n", upperText); // debug
    //printf("Lower text: %s\n", lowerText); // debug
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("%s", upperText);
    lcd.locate(0,1);
    lcd.printf("%s", lowerText);
}


void Display::scrollingPrint(char* string, unsigned int line, chrono::milliseconds speedms) {
    int length = strlen(string);
    // create a string of 16 digits to fit onto the 16 digit wide display
    char displayText[17];
    memset(displayText, ' ', 16);
    displayText[16] = '\0';
    for (int i = 0; i < length + 16; i++) {
        if (overrideScrollingLower && line > 0) { overrideScrollingLower = false; return; }
        if (i < length) displayText[15] = string[i]; //sets the final character of the display to be the next character in sequence
        shiftLeft(displayText, 16);
        if (line == 0) upperDisplay(displayText);
        else lowerDisplay(displayText);
        ThisThread::sleep_for(speedms); 
    }
}

void Display::upperDisplay(const char* string) {
    displayMutex.lock();
    strncpy(upperText, string, 16);
    upperText[16] = '\0';
    print();
    displayMutex.unlock();
}

void Display::lowerDisplay(const char* string) {
    displayMutex.lock();
    strncpy(lowerText, string, 16);
    lowerText[16] = '\0';
    print();
    displayMutex.unlock();
}