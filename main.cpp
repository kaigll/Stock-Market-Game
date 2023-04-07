#include "mbed.h"
#include <string.h>
#include "TextLCD.h"

#pragma warning(suppress: 4101)

using namespace std;

TextLCD lcd(D0, D1, D2, D3, D4, D5);


int stringLength(const char* string) {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

void displayTextGenerator(const char* source, char* display) {
    // this runs into memory issues
    int length = stringLength(source);
    for(int i = 0; i < 16; ++i) {
        *display = ' ';
        display++;
    }
    for(int i = 0; i < length; ++i) {
        *display = source[i];
        display++;
    }
    *display = '\0';  
}

void arrayShift(char* array) {
    for (int i = 0; i < stringLength(array); ++i) {
        *array = *(array + 1);
        array++;
    }
}

void ScrollingPrint(char* string) {
    //problems with memory, never reaches first print of text.
    int length;
    length = strlen(string); 

    if (length > 16) {
        char* displayText;
        displayTextGenerator(string, displayText);
        for (int i = 0; i <= length - 16; i++) {
            arrayShift(displayText);

            //lcd.cls();
            //lcd.printf("%s\n", displayText);
            printf("%s\n", displayText);
            printf("Memory address of displayText: %p\n", (void *)&displayText);
            ThisThread::sleep_for(100ms); 
        }
    }
    else lcd.printf("%s", string);
}
// for the sake of my time im ignoring all of the scrolling text to make it functional later
int main()
{
    lcd.locate(0, 0);
    printf("%d", strlen("hello world!"));
    while (true) {
        lcd.cls();        
        //lcd.printf();
        while(1);
    }
}