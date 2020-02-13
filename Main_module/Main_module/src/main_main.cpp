#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

const byte addressToRead[5] = {'W', 'U', 'S', 'L', 'L'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived; // this must match message in the TX
bool newData = false;

void setup()
{

  Serial.begin(9600);

  Serial.println("SimpleRx Starting");
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, addressToRead);
  radio.startListening();
}

void loop()
{

  if (radio.available())
  {
    radio.read(&dataReceived, sizeof(dataReceived));

    switch (dataReceived)
    {
    case '1':
      Serial.println("FrntKpd");
      break;

    case '9':
      Serial.println("Alert");
      break;
    }
    newData = true;
  }

  if (newData == true)
  {
    //Serial.print("Data received ");
    //Serial.println(dataReceived);
    newData = false;
  }
}