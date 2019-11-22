

#ifndef arduino_9292_h
#define arduino_9292_h

#include "WiFiClient.h"
#include "arduino.h"
#include "HTTPClient.h"
//HTTPClient http;

//fetches the Dutch time from "http://worldtimeapi.org/api/timezone/Europe/Amsterdam", 
//and parses it into a format (date+time) that can be directly inserted into a 9292API URL when a 0 is given as dataType (format 'YYYY-MM-DDThhmm').
//When dataType is 1, the function returns only the time in the format: 'hh:mm'
String getTime(int dataType);		


//parses the unprocessed string from an departure-times request into the wished data. 
//The dataType is the kind of information you want, for example the departure time or destination.
//The listposition is the train you want to know the data of. When 1, it will return the first train coming,
//and when 6, it will return the 6th train coming.
String _9292GetDepartureInfo(String payload, int dataType, int listPosition);

//returns a URL which you can use for a journey request. The URL includes the current time and the destination-station
String makeURL(String fromStation, String toStation);


//Fetches and returns an unprocessed string with the data fetched from the given url
String _9292GetData(String url);

#endif