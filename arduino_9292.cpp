#include "arduino_9292.h"
#include "Arduino.h"

HTTPClient http;
WiFiClient client;

String getTime(int dataType)
{
  String timeURL = "http://worldtimeapi.org/api/timezone/Europe/Amsterdam";
  String payload = _9292GetData(timeURL);
  
  int lookPos = payload.indexOf("\"datetime");
  int untilDate = payload.indexOf("T", lookPos);
  String timeAndDate;
  
  switch(dataType)
  {
	case 0:
	timeAndDate = payload.substring((lookPos + 12), (untilDate+6));
	timeAndDate.remove(13, 1); //remove ':' from the string so the variable can be pasted into the url
	break;
  
	case 1:
	timeAndDate = payload.substring((untilDate + 1) , (untilDate + 6));
	break;
  }
  return timeAndDate;
}

String _9292GetDepartureInfo(String payload, int dataType, int listPosition)
{
  int lookPos = 0;        //position of the individual char in the string it should look for the data
  int offset = 0;         //offset from the lookPos to the actual data
  String searchWord;  //the word it will look up in the string

  switch (dataType)
  {
    case 0: searchWord = "\"time";  offset = 9; break;
    case 1: searchWord = "destinationName"; offset = 19; break;
    case 2: searchWord = "viaNames"; offset = 12; break;
    case 3: searchWord = "type";  offset = 8; break;
    case 4: searchWord = "\"name";  offset = 9; listPosition+=5; break;
    case 5: searchWord = "operatorName";  offset = 16; break;
    case 6: searchWord = "service"; offset = 11; break;
    case 7: searchWord = "platform\""; offset = 12; break;
    case 8: searchWord = "platformChanged";  offset = 19; break;
    case 9: searchWord = "remark";  offset = 10; break;
    case 10: searchWord = "realtimeState";  offset = 17; break;
    case 11: searchWord = "realtimeText";  offset = 16; break;
	case 12: searchWord = "\"departure"; offset = 14; break;		//journey
  }
  
  for(int i=0; i < (listPosition+1); i++)
  {
    lookPos = payload.indexOf(searchWord, lookPos+1);
  }
  int until = payload.indexOf("\"", lookPos + offset);
  String stringpart = payload.substring((lookPos + offset), until);
  Serial.println(stringpart);
  
  return stringpart;
}


String _9292GetData(String url)
{
  String payload;
  
  http.begin(client, url);
  
  int httpCode = http.GET();
  if (httpCode > 0){
    if (httpCode == HTTP_CODE_OK)
    {
      payload = http.getString(); 
    }
	  else{
		  Serial.println("HTTPCode" + httpCode);
		  Serial.println("Forcing getString");
		  delay(1000);
		  payload = http.getString(); 
		}
  }
  http.end();
  return payload;
}


String makeURL(String fromStation, String toStation)
{
    String urlpart1 = "http://api.9292.nl/0.1/journeys?before=0&sequence=1&byFerry=true&bySubway=true&byBus=true&byTram=true&byTrain=true&lang=nl-NL&from=" + fromStation + "&dateTime=";
    String urlpart2 = "&searchType=departure&interchangeTime=standard&after=0&to=" + toStation;
    String URL2;
    URL2 = urlpart1 + getTime(0) + urlpart2;
    return URL2;
}
