#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Keypad.h>
#include "keypad_module.h"

#define CE_PIN 9
#define CSN_PIN 10

const byte addressToWrite[5] = {'W', 'U', 'S', 'L', 'L'};

const byte rows = 4; //4 rows
const byte cols = 4; //4 columns
//Mapping of the real keyboard layout:
char hexaKeys[rows][cols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

char message = '1'; // This is the message it's telling the main module. It's like " I AM THE KEYPAD BRO IT S ME CARL CHILL CHILL"
char alert = '9';
int alertCount = 0;

byte rowPins[rows] = {2, 3, 4, 5};  //Arduino's digital pins for the rows
byte colPins[cols] = {6, 7, 8, A0}; //Arduino's digital pins for the columns

char password[] = "0000";    //Master password. Change it to your needs
const short passwordlen = 4; //Lenght of the master password to compare with the len. of the entered password
char entered[15];

char key; //It is going to hold the current key press
short i, j;
bool ok; //ok == true -> Enter; ok == false -> Retype the password

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, rows, cols); //Create the object
RF24 radio(CE_PIN, CSN_PIN);                                                      //Create the radio

void setup()
{
  Serial.begin(9600);

  Serial.println("Start of transmission");
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(2, 8); //Delay, count
  radio.openWritingPipe(addressToWrite);
}

void loop()
{
  Greetings(); //What you see on the display outside

  key = customKeypad.waitForKey(); //Waits for the user to press the '*' to start entereing the password

  if (customKeypad.isPressed('*')) //Checks if user started to enter password
  {
    while ('#' != key) //The entered password must end with a '#'
    {
      Serial.print("Enter the character nr ");
      Serial.println(i + 1);
      key = customKeypad.waitForKey(); //Program waits for the user to input another character
      entered[i++] = key;              //Adding the pressed key to the entered password array
    }
    entered[i - 1] = '\0'; //Removes the '#' from the end of the entered password

    if ((i - 1) == passwordlen) //Checks if the lengts are equal
    {                           //If not, it will promt you to enter again the password
      for (j = 0; j < (i - 1); j++)
      {
        if (entered[j] == password[j]) //Actual checking of the passwords
        {
          ok = true;
        }
        else
        {
          ok = false; //If a single character doesn't matches, it breaks
          break;
        }
      }
    }
    else
    {
      ok = false; //If the lenghts don't match, it sets ok to false
    }

    if (ok == true)
    {
      if (radio.write(&message, sizeof(message)))
      {
        Serial.println("Received");
      }
      else
      {
        Serial.println("Failed");
      }
      delay(500);
    }
    else
    {
      Serial.println("Send message to Nrf24l01_receive, alert phones");
      if (alertCount++ >= 2)
      {
        radio.write(&alert, sizeof(alert));
        alertCount = 0;
      }
    }
  }
  else
  {
    Serial.println("Press '*' to begin auth");
    delay(50);
  }

  if (i > 0) //If the user entered something, clear the entered string after checking
  {
    for (i = 0; entered[i] != '\0'; i++)
    {
      entered[i] = '\0'; //Overwrites all the cells with the string terminator
    }
    i = 0; //resets the position to 0 to overwrite if incorrect password entered
  }
  delay(200);
}