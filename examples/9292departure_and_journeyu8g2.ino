/*
 * 9292departureTimes.ino
 * Date: 12-11-2019, changed 15-1-2020
 * made by H3ndrik-Jan
 * 
 * Example for the arduino_9292 library. This example is tested on the ESP32 dev module and Generic ESP8266 module,
 * but should work on almost any board using this microcontroller. 
 * It prints some details about the first 3 coming trains on a 128x62 OLED display.
 * When another button is pressed, it also gives the departure time for 2 specified journeys.
 * Change the variables to your needs.
 * 
 * See the library documentation for further explanation about the library/functions and a reference list.
 * 
 */
#include <Wire.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <arduino_9292.h>
#include <WiFiClient.h>
  #if defined(ESP32)
  #include <WiFi.h> 
  #elif defined(ESP8266)
  #include <ESP8266WiFi.h> 
  #endif

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/////////////////////////////////////////////////////////////////////////////////
//CONFIGURATION:
/////////////////////////////////////////////////////////////////////////////////
void  ISRupdate();   //ESP8266: void ICACHE_RAM_ATTR ISRupdate();
void  ISRjourney();  //ESP8266: void ICACHE_RAM_ATTR ISRjourney();

String stationId = "station-utrecht-centraal";  //Station you will see departuretimes from. Also the journeys will depart from this station.
String stationId2 = "station-amsterdam-centraal";
String stationId3 = "station-rotterdam-centraal";

char ssid[] = "XXXX";     // your WiFi-network SSID
char password[] = "XXXX"; // your WiFi-network password

int buttonPin = 12;   //button for updating the departure-times info
int button2Pin = 14;  //button for showing journey departure-times
//////////////////////////////////////////////////////////////////////////////////

int timeSinceLastUpdate = 0;
int trainlist = 0;
String URL1 = "http://api.9292.nl/0.1/locations/" + stationId + "/departure-times?lang=nl-NL";
bool getmegastring = false;
bool bupdateTime = true;

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(20, 30);
  u8g2.print("connecting...");
  u8g2.sendBuffer();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    u8g2.print(".");
    delay(500);
    u8g2.sendBuffer();
  }
  
  u8g2.clear();
  u8g2.setCursor(20, 30);
  u8g2.print("connected!");
  u8g2.setCursor(20, 50);
  u8g2.print(WiFi.localIP());
  u8g2.sendBuffer();
  delay(1000);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), ISRupdate, FALLING);
    attachInterrupt(digitalPinToInterrupt(button2Pin), ISRjourney, FALLING);
  u8g2.clearBuffer();
}

void loop()
{
  if (bupdateTime)
  {
    String payload;
    payload = "";
    payload = _9292GetData(URL1);
    u8g2.clearBuffer();
    u8g2.clear();

    u8g2.setCursor(0, 9);
    u8g2.print(_9292GetDepartureInfo(payload, 0, trainlist)); //prints time

    u8g2.setCursor(40, 9);
    String type1 = _9292GetDepartureInfo(payload, 4, (trainlist));
      if(_9292GetDepartureInfo(payload, 10, (trainlist)) != "ontime")
      {
        u8g2.print("Vertraagd!"); //prints realtimestate (if there's a delay or not)
      }
      
      else
      {
        u8g2.print(type1);
      }
    u8g2.setCursor(110, 13);
    u8g2.print(_9292GetDepartureInfo(payload, 7, trainlist));
    
    u8g2.setCursor(0, 19);
    u8g2.print(_9292GetDepartureInfo(payload, 1, trainlist)); //prints destination
    u8g2.drawLine(0, 20, 128, 20);
    
    u8g2.setCursor(0, 29);
    u8g2.print(_9292GetDepartureInfo(payload, 0, (trainlist+1))); //prints time
    
    u8g2.setCursor(40, 29);
    type1 = _9292GetDepartureInfo(payload, 4, (trainlist+1));
    Serial.print(type1);
      if(_9292GetDepartureInfo(payload, 10, (trainlist+1)) != "ontime")
      {
        u8g2.print("Vertraagd!"); //prints realtimestate (if there's a delay or not)
      }
      else
      {
        u8g2.print(type1);
      }

    u8g2.setCursor(110, 33);
    u8g2.print(_9292GetDepartureInfo(payload, 7, trainlist+1));
    
    u8g2.setCursor(0, 39);
    u8g2.print(_9292GetDepartureInfo(payload, 1, (trainlist+1))); //prints destination
    u8g2.drawLine(0, 40, 128, 40);

    u8g2.setCursor(0, 49);
    u8g2.print(_9292GetDepartureInfo(payload, 0, (trainlist+2))); //prints time
    
    u8g2.setCursor(40, 49);
    type1 = _9292GetDepartureInfo(payload, 4, (trainlist+2));
    
      if(_9292GetDepartureInfo(payload, 10, (trainlist+2)) != "ontime")
      {
        u8g2.print("Vertraagd!"); //prints realtimestate (if there's a delay or not)
      }
      else
      {
        u8g2.print(type1);
      }
    u8g2.setCursor(110, 53);
    u8g2.print(_9292GetDepartureInfo(payload, 7, trainlist+2));

    u8g2.setCursor(0, 59);
    u8g2.print(_9292GetDepartureInfo(payload, 1, (trainlist+2))); //prints destination

    u8g2.sendBuffer();          // transfer internal memory to the display
    bupdateTime = false;
    timeSinceLastUpdate = 0;
    payload = "";
  }
  
  delay(1000);
  timeSinceLastUpdate++;
  if(timeSinceLastUpdate>100){
    bupdateTime = true;
  }

  if(getmegastring == true){
    u8g2.clear();
    u8g2.clearBuffer();
    u8g2.setCursor(0,10);
    u8g2.print("Departure:");
    u8g2.setCursor(98,10);
    u8g2.print(getTime(1));
    u8g2.drawLine(96, 12, 128, 12);
    u8g2.drawLine(95, 0, 95, 12);
    
      String payload = _9292GetData(makeURL(stationId, stationId2));
      String departure = "";
      departure = _9292GetDepartureInfo(payload, 12, 1);
      Serial.println(stationId2 + departure);
      u8g2.setCursor(0,25);
      u8g2.print(stationId2.substring(8));
      u8g2.setCursor(0,35);
      for(int i = 11; i<17; i++){
        u8g2.print(departure[i]);
      }

      payload = _9292GetData(makeURL(stationId, stationId3));
      departure = "";
      departure = _9292GetDepartureInfo(payload, 12, 1);
      Serial.println(stationId3 + departure);
      u8g2.setCursor(0,50);
      u8g2.print(stationId3.substring(8));
      u8g2.setCursor(0,60);
      for(int i = 11; i<17; i++){
        u8g2.print(departure[i]);
      }
      
    u8g2.sendBuffer();
    payload = "";
    getmegastring = false;
  }
}

void ISRjourney(){
    getmegastring = true;
}
void ISRupdate() {
  bupdateTime = true;
}
