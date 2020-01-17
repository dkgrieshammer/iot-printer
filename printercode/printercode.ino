/*------------------------------------------------------------------------

  ------------------------------------------------------------------------*/
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include "Adafruit_Thermal.h"

#define TX_PIN D6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN D7 // Arduino receive   GREEN WIRE   labeled TX on printer
const char *ssid     = "SSID";
const char *password = "PWD";

// PRINTER
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

// NTP TIME
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"europe.pool.ntp.org", 3600);

// BUTTON
const int printButton = D3;
bool buttonPressed = false;
const int debounceTimeInMillis = 50;
int lastPress = 0;


// -----------------------------------------------------------------------

void setup() {
  pinMode(printButton, INPUT);
  digitalWrite(printButton, HIGH);
  Serial.begin(9600);
  mySerial.begin(19200);  // Initialize SoftwareSerial
  printer.begin();        // Init printer (same regardless of serial type)
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
//    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {

  if(digitalRead(printButton) == LOW && !buttonPressed) {
    buttonPressed = true;
    printText();
  }
  if(digitalRead(printButton) == HIGH && buttonPressed) {
    buttonPressed = false;
    delay(debounceTimeInMillis);
  }
  
}

void printText() {
  timeClient.update();
  printer.wake();       // MUST wake() before printing again, even if reset
  printer.println(F("I'm crawling this fridge since"));
  printer.print(getToday());
  printer.print(". ");
  printer.print(getMonth());
  printer.print(" ");
  printer.println(getYear());
  printer.setLineHeight(90);
  printer.feed(3);
  printer.setLineHeight();
  printer.sleep();
}

String getMonth() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime(&rawtime);
  int month = (ti->tm_mon + 1) < 10 ? 0 + (ti->tm_mon + 1) : (ti->tm_mon + 1);
  switch(month) {
    case 1:
      return "January";
      break;
    case 2:
      return "February";
      break;
    case 3:
      return "March";
      break;
    case 4:
      return "April";
      break;
    case 5:
      return "May";
      break;
    case 6:
      return "June";
      break;
    case 7:
      return "July";
      break;
    case 8:
      return "August";
      break;
    case 9:
      return "September";
      break;
    case 10:
      return "October";
      break;
    case 11:
      return "November";
      break;
    case 12:
      return "December";
      break;
    default:
      return "Forever";
                break;
  }
}

int getYear() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  int year = ti->tm_year + 1900;
  return year;
}

int getToday() {
  time_t rawtime = this->getEpochTime();
  struct tm * ti;
  ti = localtime (&rawtime);
  int day = (ti->tm_mday) < 10 ? 0 + (ti->tm_mday) : (ti->tm_mday);
  return day;  
}

void onPress() {
    
}
