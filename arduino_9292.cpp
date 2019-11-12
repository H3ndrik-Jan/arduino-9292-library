
#include "arduino_9292.h"

String getTime(void)
{
  String payload;
  String timeURL = "http://worldtimeapi.org/api/timezone/Europe/Amsterdam";
  http.begin(client, timeURL);
  
  int httpCode = http.GET();
  if (httpCode > 0){
    if (httpCode == HTTP_CODE_OK)
    {
      payload = http.getString(); 
    }
  }
  http.end();
  int lookPos = payload.indexOf("\"datetime");
  int untilDate = payload.indexOf("T", lookPos);
  String timeAndDate = payload.substring((lookPos + 12), (untilDate+6));
  timeAndDate[13] = '\0'; //remove ':' from the string so the variable can be pasted into the url
  return timeAndDate;
}

int lijstpositie(String payload, int listPosition, String searchWord)
{
  int lookPos;
  for(int i=0; i < (listPosition+1); i++)
  {
    lookPos = payload.indexOf(searchWord, lookPos+1); /*, (lookPos + listPosition));*/
  }
  return lookPos;
}

String _9292GetInfo(String payload, int dataType, int listPosition)
{
  int lookPos;        //position of the individual char in the string it should look for the data
  int offset;         //offset from the lookPos to the actual data
  String searchWord;  //the word it will look up in the string

  switch (dataType)
  {
    case 0: searchWord = "\"time";  offset = 9; break;
    case 1: searchWord = "destinationName"; offset = 19; break;
    case 2: searchWord = "viaNames"; offset = 12; break;
    case 3: searchWord = "type";  offset = 8; break;
    case 4: searchWord = "name";  offset = 8; break;
    case 5: searchWord = "operatorName";  offset = 16; break;
    case 6: searchWord = "service"; offset = 11; break;
    case 7: searchWord = "platform\""; offset = 12; break;
    case 8: searchWord = "platformChanged";  offset = 19; break;
    case 9: searchWord = "remark";  offset = 10; break;
    case 10: searchWord = "realtimeState";  offset = 17; break;
    case 11: searchWord = "realtimeText";  offset = 16; break;
  }
  
  lookPos = lijstpositie(payload, listPosition, searchWord);
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
  }
  http.end();
  return payload;
}