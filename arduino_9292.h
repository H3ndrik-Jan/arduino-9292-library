

#ifndef arduino_9292_h
#define arduino_9292_h

#include "Arduino.h"
#include "WiFiClient.h"
#include "HTTPClient.h"

//fetches the Dutch time from "http://worldtimeapi.org/api/timezone/Europe/Amsterdam", 
//and parses it into a format that can be directly inserted into a 9292API URL.
String getTime(void);		


//parses the unprocessed string from an departure-times request into the wished data. 
//The dataType is the kind of information you want, for example the departure time or destination.
//The listposition is the train you want to know the data of. When 1, it will return the first train coming,
//and when 6, it will return the 6th train coming.
String _9292GetInfo(String payload, int dataType, int listPosition);


//Fetches and returns an unprocessed string with the data fetched from the given url
String _9292GetData(String url);

#endif