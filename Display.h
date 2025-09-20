#ifndef DISPLAY_H
#define DISPLAY_H

#include "mbed.h"
#include <string.h>
#include "TextLCD.h"

class Display {
public:
    Display();
    void scrollingPrint(char* string, unsigned int line, chrono::milliseconds speedms);
    void upperDisplay(const char* string);
    void lowerDisplay(const char* string);

    bool overrideScrollingLower = false;

private:
    void print();
    void shiftLeft(char* string, int length);
    char upperText[17];
    char lowerText[17];
    Mutex displayMutex;

    void copyString(const char* source, char* destination, int length);
};

#endif //DISPLAY_H