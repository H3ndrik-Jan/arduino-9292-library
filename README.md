# arduino-9292-library
Arduino library for easily fetching data from the 9292 (Dutch public transport) API to an arduino-compatible microcontroller.


Functions in library

<b>_9292GetData: </b> returns a string fetched from the url given with the function.

<b>_9292GetInfo: </b> returns a string containing the information you specified in what you gave with it while calling the function.

<b>getTime: </b> returns a string with the time and date, fetched from "http://worldtimeapi.org/api/timezone/Europe/Amsterdam". Neccesary for the journey function (not yet implemented).
	
	
Whenever you do a request for the departure times, you basically get an array of strings like these:

		{
		  "time": "20:43",
		  "destinationName": "S Hertogenbosch",
		  "viaNames": null,
		  "mode": {
		    "type": "train",
		    "name": "Sprinter"
		  },
		  "operatorName": "NS",
		  "service": null,
		  "platform": "1",
		  "platformChanged": false,
		  "remark": null,
		  "realtimeState": "ontime",
		  "realtimeText": null
		},

Note that not all variables are available in the _9292GetDepartureInfo function (yet)! 
Allthough you can all all functions with their corresponding numbers, the variables with an ! will not always give sensible information!

This function expects the input data in a string, and next the datatype (e.g. time, destinationName etc) in an integer, 
and finally the listPosition (first coming train, or 5 trains ahead?) in an integer . It then returns a string with the data you asked for.
		
REFERENCE LIST: 

		time = 0
		
		destinationName = 1
		
		!viaNames = 2
		
		type = 3
		
		name = 4
		
		operatorName = 5
		
		!service = 6
		
		platform = 7
		
		!platformChanged = 8
		
		!remark = 9
		
		realtimeState = 10
		
		!realtimeText = 11
		
		
The library is still quiet messy. It will be updated and improved soon.
		
