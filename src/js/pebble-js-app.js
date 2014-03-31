/*global icon:true*/
/*global temperature:true*/
/*global city:true*/
/*global code:true*/
/*global description:true*/
/*global conditions:true*/
/*global status:true*/
/*global iconCode:true*/
/*global locationWatcher:true*/
/*global window:true*/
/*jshint smarttabs:true*/


/*to do list. 
1. Auto update
2. Hope auto update doesn't hoark current data
2. Cache data
2. Set fonts
 //appID 9f001a597927140d919cc512193dadd2    &APPID=9f001a597927140d919cc512193dadd2

*/

function fetchWeather(latitude, longitude) {
  var response;
  var req = new XMLHttpRequest();
//	console.log("fixin' to get ready to open URL");
/*	Pebble.sendAppMessage({	
		"commState":"GET",		
		});  */
	console.log("commState GET");	
//	req.open('GET', "http://api.openweathermap.org/data/2.5/weather?" + "lat=" + latitude + "&lon=" + longitude + "&cnt=2", true);
	req.open('GET', "http://api.openweathermap.org/data/2.5/weather?" + "lat=" + latitude + "&lon=" + longitude + "&cnt=2&APPID=9f001a597927140d919cc512193dadd2", true);
	console.log("http://api.openweathermap.org/data/2.5/weather?" + "lat=" + latitude + "&lon=" + longitude + "&cnt=2"); 
	req.onload = function(e) {
		console.log("get request gotten");
		var offset = new Date().getTimezoneOffset() / 60;
		console.log("offset " + offset); 
//		Pebble.sendAppMessage({
//			"commState":"SET",
//			})
	console.log("commState SET");
		console.log("req.responseText " + req.responseText);
/*		console.log("req.readyState " + req.readyState); */
		
		
		if (req.readyState == 4) {
				if(req.status == 200) {
					//console.log("req.status " + req.status);
					//console.log("req.responseText.length " + req.responseText.length);
					//console.log(req.responseText);
					response = JSON.parse(req.responseText);

//					if (response && response.list && response.list.length > 0) {
//					if (response && response.cod && response.cod.length > 0) {
//					if (response && response.length > 0) {
//					if (1 == 1) {  console.log("DANGER WARNING ACHTUNG!!!!! BYPASS RESPONSE CHECK!!"); console.log("VALUES MAY BE OVERWRITTEN BY DEFAULTS!"); 
					if (req.responseText.length > 0) {  
//					if (1==1) {      
//					console.log("if condition satisfied");

//						icon = iconFromWeatherIconCode(response.weather[0].icon);
						console.log("weather ID " + response.weather[0].id);
						console.log("reported conditions long(desc)/short(main): " + response.weather[0].description + response.weather[0].main);
						console.log("assigned icon " + response.weather[0].icon);
						icon = iconFromWeatherId(response.weather[0].id);  
						console.log("icon " + icon); 
						temperature = Math.round((response.main.temp - 273.15) * 1.8 + 32);
						city = response.name;
						console.log("city: " + city);
						//status = new Date(response.dt *1000);
						// UNIX epoch for PM value 1391745210
						var timeStamp = new Date(response.dt * 1000); // Your timezone!
						var setDate = new Date(1391745210000); // Your timezone!
						var staleDate = new Date(response.dt * 1000);
//						var sunriseStamp = new Date(response.sys.sunrise * 1000);
//						console.log(response.sys.sunrise);
//						console.log(response.sys.sunset); 
						var sunsetStamp = new Date(response.sys.sunset * 1000);
						var myEpoch = timeStamp.getTime()/1000.0;
						var myTime = timeStamp.getTime();
						var myHours = timeStamp.getHours();
						var myMinutes = timeStamp.getMinutes();
						var days = 0;
						var difference = 0;
						var today = new Date();
						var epoch =  Math.round(today.getTime() / 1000); 
						var diffdiff = epoch - response.dt; 
//						console.log("new Date() " + today);
						difference = today - staleDate;
//						console.log("difference " + difference);
						days = Math.round(difference/(1000*60*60*24));
//						console.log("days " + days);
						var hours = Math.round(difference/(1000*60*60));
						var minutes = Math.round(difference/(1000*60));
						var seconds = Math.round(difference/(1000));
						status = staleDate; 
//						sunset = sunsetStamp; 
						conditions = response.weather[0].main;
						description = response.weather[0].description;
                        console.log("description = " + description); 
						code = response.weather[0].id + "-" + response.weather[0].icon + "-" + icon + ":" + description;
						iconCode = response.weather[0].icon;
//						sunrise = new Date(response.sys.sunrise * 1000); 
//						console.log("sunrise " + sunrise);
//						sunset = new Date(response.sys.sunset * 1000); 
//						console.log("sunset " + sunset);
						
						sunrise = response.sys.sunrise;
						sunset = response.sys.sunset; 
						staleEpoch = response.dt; 
						
						
/*						console.log("staleEpoch " + staleEpoch);
//						staleEpoch = 1393334037,  
						console.log("sunrise " + sunrise);
						console.log("hours " + hours);
						console.log("minutes " + minutes);
						console.log("sunset " + sunset);
							
						console.log("icon " + icon);
						console.log("temperature " + temperature + "\u00B0F");
						console.log("city " + city);
						console.log("status " + status);
*/						console.log("sunrise " + sunrise); 
						console.log("sunset " + sunset); 
						console.log("seconds " + seconds); 
/*						console.log("conditions " + conditions);
						console.log("description " + description);
						console.log("code " + code);
						console.log("iconCode " + iconCode);
						console.log("staleDate " + staleEpoch); */

						Pebble.sendAppMessage({							
							"icon":icon,
//							 "icon":8,
							 "temperature":temperature + "\u00B0F",
//							 "temperature":temperature + "",
//							 "city":"Los Angeles",
							 "status":status,
							 "conditions":conditions,
							 "offset":offset, 
							 "description":description,
							 "code":code,
							 "iconCode":iconCode,
							 "staleDate":staleEpoch,
//							 "staleDate":1394146503,							  
							 "city":city,
//							 "city":"Shorelineses",
//							 "city":"Lynnwood",
//							 "city":"Tanglewilde- Thompson Place",
//							 "city":"de la Reina de Los Angeles",
//							 "city":"San Lius Obispo",
							 "sunrise":sunrise,
							 "sunset":sunset,
							 });
						console.log("-------success --- end -------");
//						console.log(Pebble.sendAppMessage);
					} else {
						console.log("response not greater than 0");
						console.log("if response > 0 check failed");
					}		
      } else {
        console.log("Error");
		  console.log("IF condition failed");
      }
    }
  };
  req.send(null);
}

function locationSuccess(pos) {
  var coordinates = pos.coords;
  fetchWeather(coordinates.latitude, coordinates.longitude);
}

function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  Pebble.sendAppMessage({
    "city":"no location",
    "temperature":"n/a"
  });
}

var locationOptions = { "timeout": 15000, "maximumAge": 60000 }; 


Pebble.addEventListener("ready",
                        function(e) {
//							console.log("ready listener created, status: " + e.ready);
							locationWatcher = window.navigator.geolocation.watchPosition(locationSuccess, locationError, locationOptions);
//							console.log("e.type " + e.type);
                        });

Pebble.addEventListener("appmessage",
                        function(e) {
                          window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
//                          console.log(e.type);
//                          console.log(e.payload.temperature);
//                          console.log("appmessage listener created");
                        });

Pebble.addEventListener("webviewclosed",
						 
						 function(e) {
							 console.log("webview closed listener created");
							 console.log(e.type);
							 console.log(e.response);
						 });


function statusFromWeatherResponseDate(timeStamp) {
							var x=new Date();
	console.log("x " + x);
//						var today = new Date(timeStamp);
//	console.log(today);
if (timeStamp>x-600)
  {
	  
//	  status = "Future";
	  return "Currently";
//  alert("Today is before 14th January 2100");
  }
else
  {
//	  status = "Stale" ;
	  return new Date(timeStamp * 1000); //  alert("Today is after 14th January 2100");
  }
	
}

function iconFromWeatherIconCode(weatherIconCode) {
	if (weatherIconCode == "01d") {
		return 0;   //sun (code 0)
	} else if (weatherIconCode == "01n") {
		return 1;  //moon (code 1)
	} else if (weatherIconCode == "02d") {
		return 2;  //partly cloudy sun (code 2)
	} else if (weatherIconCode == "02n") {
		return 3;  //partly cloudy moon
	} else if (weatherIconCode == "03d") {
		return 4; //clouds
	} else if (weatherIconCode == "03n") {
		return 4; //clouds
	} else if (weatherIconCode == "04d") {
		return 2; //partly cloudy sun
	} else if (weatherIconCode == "04n") {
		return 4; //partly cloudy moon
	} else if (weatherIconCode == "09d") {
		return 5; //rain
	} else if (weatherIconCode == "09n") {
		return 5; //rain
	} else if (weatherIconCode == "10d") {
		return 5; //rain cloud sun or just rain
	} else if (weatherIconCode == "10n") {
		return 5; //rain cloud moon or just rain
	} else if (weatherIconCode == "11d") {
		return 5; //thundrestorm
	} else if (weatherIconCode == "11n") {
		return 5; //thunderstorm
	} else if (weatherIconCode == "13d") {
		return 6; //snow
	} else if (weatherIconCode == "13n") {
		return 6; //snow
	} else if (weatherIconCode == "50d") {
		return 7; //lines
	} else if (weatherIconCode == "50n") {
		return 7; //lines
	} else {
		return 4; //lines
	}

}

/*
	RESOURCE_ID_IMAGE_SUN, //0
	RESOURCE_ID_IMAGE_MOON,  //1
	RESOURCE_ID_IMAGE_PCLOUDY,  //2
	RESOURCE_ID_IMAGE_PCLOUDY_MOON,  //3
	RESOURCE_ID_IMAGE_CLOUD,  //4
	RESOURCE_ID_IMAGE_RAIN,  //5
	RESOURCE_ID_IMAGE_SNOW,  //6
	RESOURCE_ID_IMAGE_FOG,  //7
	RESOURCE_ID_IMAGE_WIND,  //8
	
01d.png 01n.png sky is clear - Clear
02d.png	02n.png	few clouds
03d.png 03n.png scattered clouds - Clouds
04d.png 04n.png broken clouds
09d.png 09n.png shower rain - Rain
10d.png 10n.png Rain - Rain
11d.png 11n.png Thunderstorm - ?
13d.png 13n.png snow - Snow
50d.png 50n.png mist - ? 
  
  
*/


						
						//below tells how old it is! Hooray! Attempting to move logic into pebble app so we don't 
						//have to refresh to see how old the stamp is
						//and instead we can just update the screen based on time-tic
						/*
	RESOURCE_ID_IMAGE_SUN, //0 clear 0 
	RESOURCE_ID_IMAGE_MOON,  //1  clear +1 (+1 = night)
	RESOURCE_ID_IMAGE_PCLOUDY,  //2 PC 
	RESOURCE_ID_IMAGE_PCLOUDY_MOON,  //3  PC +1 for night
	RESOURCE_ID_IMAGE_CLOUD,  //4
	RESOURCE_ID_IMAGE_CLOUD,  //5 for full clouds at night
	RESOURCE_ID_IMAGE_RAIN,  //6
	RESOURCE_ID_IMAGE_RAIN,  //7 for 6 + 1 or rain at night
	RESOURCE_ID_IMAGE_SNOW,  //8
	RESOURCE_ID_IMAGE_SNOW,  //9 is 8 + 1 or snow at night
	RESOURCE_ID_IMAGE_FOG,  //10 
	RESOURCE_ID_IMAGE_FOG,  //11, or 10 + 1 fog at night 
	
	clear == 0
	partly cloudy == 2
	full cloudy == 4
	rain == 6
	snow == 8
	fog, mist, lines == 10
	
	+ 1 renders night version, +1 is added in c program, not here in javasdript
						*/




function iconFromWeatherId(weatherId) {
  if (weatherId < 200) {		    // 0-199 undefined, return lines? 
	  return 10;
  } else if (weatherId < 300) {     // 200 series - thunderstorms,
      return 12;
  } else if (weatherId < 600) {      // 300 to 321 defined as rain, 400-499 not defined, 500-599 is rain
	  return 6;	  
  } else if (weatherId < 700) { 	// 600-699 defined as snow	
	  return 8;
  } else if (weatherId < 800) {		// 700-799 is mist, smoke, fog, etc. Return lines
	  return 10;						// 900-99 is crazy atmospheric shit, 
  } else  if (weatherId == 800 ) {		// 800 is clear
	  return 0; 
  } else if (weatherId < 804 ) {	// 801, 802, 803 are all partly cloudy
	  return 2;
  } else if (weatherId = 804 ) {   // 804 = overcast. Should it be clouds, or lines? I love lines. So, lines. But it shoudl probably be clouds
	  return 10; 
  } else {							// 900 to 962 ranges from tornado to calm. Most strange. 
	  return 10;
  }
}
