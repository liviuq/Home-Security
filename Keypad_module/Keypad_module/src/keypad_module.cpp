#include <Arduino.h>
#include <Keypad.h>
#include "keypad_module.h"

void Greetings()
{
    Serial.println("Yo wduppp");
}

/*void resetEntered( char entered[])
{
    int i;
    i = strlen( entered);

    if( i > 0)                                  //If the user entered something, clear the enteres string
    {
        for( i=0; entered[i] != '\0' ; i++)       
        { 
            entered[i] = '\0';                      //Overwrites all the cells with the string terminator
        }
    }
    i = 0;                                    //resets the position to 0 to overwrite if incorrect password entered
}*/
