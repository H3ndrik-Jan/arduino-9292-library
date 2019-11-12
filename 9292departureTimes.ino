/*
 * 9292departureTimes.ino
 * Date: 12-11-2019
 * made by H3ndrik-Jan
 * 
 * Example for the arduino_9292 library. This example is made to run on an ESP32 dev module. 
 * It prints some details about the first and second coming train on a 2004 LCD display.
 * 
 * See the library documentation for further explanation about the library/functions and a reference list.
 * 
 */
#include <arduino_9292.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

String stationId = "station-utrecht-centraal";  //insert trainstation, bus-stop etc of choice here.

char ssid[] = "XXXXX";     // your network SSID
char password[] = "XXXXX"; // your network key
LiquidCrystal lcd(18, 19, 14, 27, 25, 26); //configure the pins your 2004 LCD display is connected to

bool bupdateTime = true;
int timeSinceLastUpdate = 0;
int trainlist = 1;
String URL1 = "http://api.9292.nl/0.1/locations/" + stationId + "/departure-times?lang=nl-NL";
extern HTTPClient http;

void setup() {
  Serial.begin(115200);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("connecting...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    lcd.print(".");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected!");
  lcd.setCursor(1, 2);
  lcd.print(WiFi.localIP());
  delay(1000);
}

void loop()
{
  if (bupdateTime)
  {
    String payload;
    payload = "";
    payload = _9292GetData(URL1);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(_9292GetDepartureInfo(payload, 0, trainlist)); //prints time

    lcd.setCursor(13, 0);
    lcd.print(_9292GetDepartureInfo(payload, 10, trainlist)); //prints realtimestate (if there's a delay or not)

    lcd.setCursor(0, 1);
    lcd.print(_9292GetDepartureInfo(payload, 1, trainlist)); //prints destination
    
    lcd.setCursor(0, 2);
    lcd.print(_9292GetDepartureInfo(payload, 0, (trainlist+1))); //prints time
    
    lcd.setCursor(13, 2);
    lcd.print(_9292GetDepartureInfo(payload, 10, (trainlist+1))); //prints realtimestate (if there's a delay or not)

    lcd.setCursor(0, 3);
    lcd.print(_9292GetDepartureInfo(payload, 1, (trainlist+1))); //prints destination
     
    bupdateTime = false;
    timeSinceLastUpdate = 0;
  }
}
