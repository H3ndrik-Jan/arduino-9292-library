/*
 * 9292departureTimes_ucg.ino
 * Date: 12-11-2019, changed 15-1-2020
 * made by H3ndrik-Jan
 * 
 * Example for the arduino_9292 library. This example is tested on the ESP32 dev module and Generic ESP8266 module,
 * but should work on almost any board using this microcontroller. 
 * It prints the departure times from the given station on a 128x160 TFT LCD module (ST7735).
 * The times automatically update about every 100 seconds.
 * Change the variables to your needs.
 * 
 * Check out the library documentation for further explanation about the library/functions, a reference list, and example pictures.
 * https://github.com/H3ndrik-Jan/arduino-9292-library
 */
 #include <Wire.h>
#include <SPI.h>
#include "Ucglib.h"
#include <arduino_9292.h>
#include <WiFiClient.h>
  #if defined(ESP32)
  #include <WiFi.h> 
  #elif defined(ESP8266)
  #include <ESP8266WiFi.h> 
  #endif

  Ucglib_ST7735_18x128x160_HWSPI ucg(/*cd=*/ 26, /*cs=*/ 27, /*reset=*/ 25);

/////////////////////////////////////////////////////////////////////////////////
//CONFIGURATION:
/////////////////////////////////////////////////////////////////////////////////
void  ISRupdate();   //ESP8266: void ICACHE_RAM_ATTR ISRupdate();

String stationId = "station-utrecht-centraal";  //Station you will see departuretimes from. Format: "station-xxxx-xxxx-...". 
                                                //You can check if the stationId is right by making a request using your 
                                                //favorite web-browser on your favorite device

char ssid[] = "xxxx";     // your WiFi-network SSID
char password[] = "xxxx"; // your WiFi-network password

int buttonPin = 13;   // optional button for manually updating the departure times.
//////////////////////////////////////////////////////////////////////////////////

int timeSinceLastUpdate = 0;
int trainlist = 0;
String URL1 = "http://api.9292.nl/0.1/locations/" + stationId + "/departure-times?lang=nl-NL";
bool getmegastring = false;
bool bupdateTime = true;
int block[7] = {8, 28, 48, 68, 88, 108, 128};
String payload;

void printTimes(int x);



void setup() {
  //SPI.beginTransaction(SPISettings(26000000, MSBFIRST, SPI_MODE0));
  Serial.begin(115200);
  delay(1000);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.clearScreen();
  ucg.setRotate90();
  ucg.setFont(ucg_font_helvR08_tr);
  ucg.setColor(255, 255, 255);
  ucg.setPrintPos(0,25);
  ucg.print("Connecting wifi:");
  Serial.print("Connecting wifi:");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
    delay(500);
    ucg.print(".");
    Serial.print(".");
    }
  delay(1000);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISRupdate, FALLING);
}

void loop()
{
  if (bupdateTime)
  {
    payload = "";
    payload = _9292GetData(URL1);
   
    //Background is drawn
    ucg.setColor(220, 220, 255);
    ucg.drawBox(0,0,130,block[0]);
    ucg.setColor(10, 10, 150);
    ucg.drawBox(130,0, 160, block[0]);
    ucg.setColor(255, 255, 255);
    ucg.drawBox(0,block[0],160,block[1]);
    ucg.setColor(220, 220, 255);
    ucg.drawBox(0,block[1],160,block[2]);
    ucg.setColor(255, 255, 255);
    ucg.drawBox(0,block[2],160,block[3]);
    ucg.setColor(220, 220, 255);
    ucg.drawBox(0,block[3],160,block[4]);
    ucg.setColor(255, 255, 255);
    ucg.drawBox(0,block[4],160,block[5]);
    ucg.setColor(220, 220, 255);
    ucg.drawBox(0,block[5],160,block[6]);
  
    ucg.setColor(10, 10, 150); //textcolor
    ucg.setFont(ucg_font_profont10_mf);
    ucg.setPrintPos(1,7);
    ucg.print("Vertrek");
    ucg.setPrintPos(135,7);
    ucg.setColor(255, 255, 255); //textcolor
    ucg.print(getTime(1));
    ucg.setColor(10, 10, 150); //textcolor
    ucg.setFont(ucg_font_helvR08_tr);
    
      for(int x = 1; x<7; x++){
        printTimes(x);
      }

    bupdateTime = false;
    timeSinceLastUpdate = 0;
    payload = "";
  }
  
  delay(1000);
  timeSinceLastUpdate++;
  if(timeSinceLastUpdate>100){
    bupdateTime = true;
  }
}

void printTimes(int x) {
  trainlist = (x-1);
      ucg.setPrintPos(0, block[x]-10);
    ucg.print(_9292GetDepartureInfo(payload, 0, (trainlist))); //prints time

    ucg.setPrintPos(120, block[x]);
    String type1 = _9292GetDepartureInfo(payload, 4, (trainlist));
    ucg.print(type1);

      if(_9292GetDepartureInfo(payload, 10, (trainlist)) != "ontime")
      {
        ucg.setColor(255, 10, 10);
        ucg.setPrintPos(40, block[x]-10);
        ucg.print("Vertraagd!"); //prints realtimestate (if there's a delay or not)
        ucg.setColor(10, 10, 150);
      }

    ucg.setPrintPos(120, block[x]-10);
    ucg.print(_9292GetDepartureInfo(payload, 7, trainlist));
    
    ucg.setPrintPos(0, block[x]);
    ucg.print(_9292GetDepartureInfo(payload, 1, trainlist)); //prints destination
}

void ISRupdate() {
  bupdateTime = true;
}
