//comment
#include "pebble.h"
#include "bluetooth.h"

#define ICON_PKEY 1
#define ICON_DEFAULT 7

#define LOCATION_PKEY 2
#define LOCATION_DEFAULT "open weather map org"
static char *location_read_key = LOCATION_DEFAULT;

//static char *iconKey = "WEATHER_KEY_NIGHT";
const char *new_location = LOCATION_DEFAULT;
const char *old_location = LOCATION_DEFAULT;
//char *iconkey = "WEATHER_ICONS_NIGHT"; 

#define TEMP_PKEY 3
#define TEMP_DEFAULT "99.9\u00B0K"
static char *temp_read_key = TEMP_DEFAULT;
const char *new_temp = TEMP_DEFAULT;
const char *old_temp = TEMP_DEFAULT;

static char *comm_get = "GET";
static char *comm_set = "SET"; 

#define STALE_INT_PKEY 4
#define STALE_INT_DEFAULT 1095429600
static uint32_t staleInt		= STALE_INT_DEFAULT;
static uint32_t oldStaleInt 	= STALE_INT_DEFAULT;
static uint32_t diffInt; 
uint32_t lclTimeInt; 


//const static	
//	const 
//		GFont custom_font_time; // = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_27));			
//const static	
//	const 
//		GFont custom_font_large_location; // = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_27));
//const static	
//	const 
//		GFont custom_font_small_location; // = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_22));
//const static	
//	const 
//		GFont custom_font_tiny_location; // = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_17)); */


//const char *staleString;

#define SUNRISE_PKEY 5
#define SUNRISE_DEFAULT 1095429600
static uint32_t sunriseInt	= SUNRISE_DEFAULT;
static uint32_t oldSunriseInt = SUNRISE_DEFAULT;

#define SUNSET_PKEY 6
#define SUNSET_DEFAULT 1095472800
//static uint32_t sunsetKeyInt	= SUNSET_DEFAULT;
static uint32_t sunsetInt;
static uint32_t oldSunsetInt;

//uint32_t sunriseInt = 1095429600;
//uint32_t sunsetInt = 1095472800;

#define OFFSET_PKEY 7
#define OFFSET_DEFAULT 0000000007
uint32_t offsetInt;
uint32_t oldOffsetInt;

static Window *window;
static TextLayer *time_layer;
static TextLayer *date_layer;
static BitmapLayer *date_border_layer;
static TextLayer *line_layer;
static TextLayer *line_layer2;
static BitmapLayer *battery_bar_bitmap_layer;
static BitmapLayer *icon_layer;
static BitmapLayer *comm_status_layer;
static TextLayer *temperature_layer;
static TextLayer *location_layer;
static TextLayer *description_layer;
static TextLayer *status_layer;
static TextLayer *conditions_layer;
static TextLayer *code_layer;
static TextLayer *build_layer;
TextLayer *bt_layer; 		
static InverterLayer *inverter_layer;

const char *time_stamp;
const char *commStateString;
//char* countChecker;

uint32_t iconInt			= ICON_DEFAULT;
uint32_t oldIconInt		= ICON_DEFAULT;
uint32_t checkInt;

//static char time_status;
//static struct tm status_time;
//static int time_stamp_int;
static int refresher_int;
static int icon_counter;
static int battery_counter;
static int refresh_counter;
//static int size_i;
int debug_flag = 0;
int light_flag = 0;
int night_flag = 0; 
int font_size_flag = 1; 

static GBitmap *icon_bitmap = NULL;
static GBitmap *date_layer_bitmap = NULL;
static GBitmap *battery_bar_bitmap = NULL;
static GBitmap *comm_status_bitmap = NULL;

static AppSync sync;
static uint8_t sync_buffer[2048];

enum WeatherKey {
	
	WEATHER_ICON_KEY        = 0,  //    "icon": 0
	WEATHER_TEMPERATURE_KEY = 1,  //    "temperature": 1,
	WEATHER_STALEDATE_KEY   = 2, //    "staleDate": 2,
	WEATHER_STATUS_KEY      = 3,    //    "status": 3,
	WEATHER_SUNRISE_KEY     = 4,  //    "sunrise": 4,
	WEATHER_SUNSET_KEY      = 5, //"sunset": 5,
	WEATHER_CONDITIONS_KEY  = 6, //    "conditions": 6,
	WEATHER_OFFSET_KEY      = 7, //    "offset": 7,
	WEATHER_LOCATION_KEY    = 8,  //    "city": 8,
	WEATHER_CODE_KEY        = 9,  //     "code": 9,
	WEATHER_ICONCODE_KEY    = 10,  //    "iconCode": 10,
	COMMSTATE_KEY           = 11,  //"commState": 11,
    WEATHER_DESCRIPTION_KEY = 12,
};


static uint32_t WEATHER_ICONS[] = {
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
    RESOURCE_ID_IMAGE_TSTORM,
    RESOURCE_ID_IMAGE_TSTORM, 
};

uint32_t WEATHER_ICONS_NIGHT[] = {
	RESOURCE_ID_IMAGE_MOON, //0
	RESOURCE_ID_IMAGE_MOON,  //1
	RESOURCE_ID_IMAGE_MOON,  //2
	RESOURCE_ID_IMAGE_MOON,  //3
	RESOURCE_ID_IMAGE_MOON,  //4
	RESOURCE_ID_IMAGE_MOON,  //5
	RESOURCE_ID_IMAGE_MOON,  //6
	RESOURCE_ID_IMAGE_MOON,  //7
}; 

static uint32_t BATTERY_ICONS[] = {
	RESOURCE_ID_IMAGE_10,   //0
	RESOURCE_ID_IMAGE_20,   //1
	RESOURCE_ID_IMAGE_30,   //2
	RESOURCE_ID_IMAGE_40,   //3
	RESOURCE_ID_IMAGE_50,   //4
	RESOURCE_ID_IMAGE_60,   //5
	RESOURCE_ID_IMAGE_70,   //6
	RESOURCE_ID_IMAGE_80,   //7
	RESOURCE_ID_IMAGE_90,   //8
	RESOURCE_ID_IMAGE_100,   //9
};

static void fetch_msg(void) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "static void fetch_msg(void) requested"); 	
	refresh_counter = 0;
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    
    if (iter == NULL) {
        //		layer_set_hidden(text_layer_get_layer(build_layer), false);
        return;
	}
    //	dict_write_end(iter);
	app_message_outbox_send();
}

static void handle_battery(BatteryChargeState charge_state) {
	
	if (battery_bar_bitmap) {
		gbitmap_destroy(battery_bar_bitmap);
	}
	
	battery_bar_bitmap = gbitmap_create_with_resource(BATTERY_ICONS[(charge_state.charge_percent / 10) - 1]);
	bitmap_layer_set_bitmap(battery_bar_bitmap_layer, battery_bar_bitmap);
	if (charge_state.is_charging) {
        layer_set_hidden(text_layer_get_layer(line_layer), false);
        layer_set_hidden(text_layer_get_layer(line_layer2), false);
	} else {
		layer_set_hidden(text_layer_get_layer(line_layer), true);
        layer_set_hidden(text_layer_get_layer(line_layer2), true);
	}
}


static 
	void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Error: %i - %s", app_message_error, translate_error(app_message_error));
    
}

int countChar(char *s)
 {
 int len = 0;
 for(; *s != '\0'; s++, len++);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "from inside countChar %d", len);
 return len;
 } 

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	
//	char iconKey; 
	
	if (night_flag == 1) {
		
	} else if (night_flag == 0) {
//		iconKey = "WEATHER_ICONS"; 
	}
	
	switch (key) {
            
        case WEATHER_ICON_KEY:
            if (icon_bitmap) {
				gbitmap_destroy(icon_bitmap);
            }
			iconInt = new_tuple->value->uint32;
            
            if (oldIconInt == iconInt ) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldIconInt %lu == iconInt %lu", oldIconInt, iconInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Bypassing write to persistent storage! TURBOFAST MODE!!!");
//              icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[(new_tuple->value->uint32)]);
                icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[(new_tuple->value->uint32 + night_flag)]);
                bitmap_layer_set_bitmap(icon_layer, icon_bitmap);
                
            } else if (oldIconInt != iconInt) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldIconInt %lu == iconInt %lu", oldIconInt, iconInt);
                icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[(new_tuple->value->uint32 + night_flag)]);
                bitmap_layer_set_bitmap(icon_layer, icon_bitmap);
                persist_write_int(ICON_PKEY, iconInt);
                oldIconInt=new_tuple->value->uint32;
            }
            break;
            
        case WEATHER_TEMPERATURE_KEY:
            new_temp = new_tuple->value->cstring;
            //APP_LOG(APP_LOG_LEVEL_DEBUG, "old_temp %s new_temp %s", old_temp, new_temp);
            
            if (strcmp	(new_temp, old_temp) == 0) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Bypassing write to persistent storage! TURBOFAST MODE!!!");
				text_layer_set_text(temperature_layer, new_temp);
                
            } else 	if (strcmp	(new_temp, old_temp) != 0) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "old_temp != new_temp (strcompare)");
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "%s != %s", old_temp, new_temp);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "writing %s to persistent storage!", new_temp);
                persist_write_string(TEMP_PKEY, new_temp);
                text_layer_set_text(temperature_layer, new_temp);
                old_temp = new_tuple->value->cstring;
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "updating old_temp");
            }
            break;
		
		case WEATHER_LOCATION_KEY:
            new_location = new_tuple->value->cstring;
            //APP_LOG(APP_LOG_LEVEL_DEBUG, "old_location %s new_location %s", old_location, new_location);
			text_layer_set_text(location_layer, new_location); 			

            static char location_counter[32];  //= "White Center";
            strcpy(location_counter, new_location);
            int charCount = countChar(location_counter);
            APP_LOG(APP_LOG_LEVEL_DEBUG, "STRING [02] %s has %d characters", location_counter, charCount);
            if (charCount < 10 ) {
                GFont custom_font_large_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_28));
                text_layer_set_font(location_layer, custom_font_large_location);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "charCount %d reads less than 10", charCount);
                layer_set_frame(text_layer_get_layer(location_layer), (GRect(-10, 115, 164, 35)));
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "setting location_layer, custom_font_large_location");
                text_layer_set_overflow_mode(location_layer, GTextOverflowModeTrailingEllipsis);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "setting ocation_layer, GTextOverflowModeTrailingEllipsis");
            } else if (charCount < 16) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "charCount %d reads greater than 10 less than 16", charCount);
                text_layer_set_overflow_mode(location_layer, GTextOverflowModeWordWrap);
                GFont custom_font_small_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_18));
                //GFont custom_font_small_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_22));
                text_layer_set_font(location_layer, custom_font_small_location);
                layer_set_frame(text_layer_get_layer(location_layer), (GRect(-10, 122, 164, 100)));
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "setting location_layer, custom_font_small_location");
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "setting ocation_layer, GTextOverflowModeWordWrap");
            } else
            {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "charCount %d reads 17 or more", charCount);
                GFont custom_font_tiny_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_BOLD_16));
                text_layer_set_font(location_layer, custom_font_tiny_location);
                layer_set_frame(text_layer_get_layer(location_layer), (GRect(-10, 125, 164, 100)));
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "setting location_layer, custom_font_tiny_location");
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "setting ocation_layer, GTextOverflowModeWordWrap");
                text_layer_set_overflow_mode(location_layer, GTextOverflowModeWordWrap); 
            }
            //APP_LOG(APP_LOG_LEVEL_DEBUG, "exiting charCount if/then stack");
            break;  

            if (strcmp (new_location, old_location) == 0) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Bypassing write to persistent storage! TURBOFAST MODE!!!");
            } else if (strcmp (new_location, old_location) != 0) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "%s != %s", old_location, new_location);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "SLOW TRAIN COMING - writing %s to persistent storage!", new_location);
                persist_write_string(LOCATION_PKEY, new_location);
//                text_layer_set_text(location_layer, new_location);
                old_location = new_tuple->value->cstring;
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "updating old_location");
            }
            break;		
            
		case WEATHER_STALEDATE_KEY:
            staleInt = new_tuple->value->uint32;
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "THIS SHOULD SET THE staleINT back to the new TUPLE and overwrite TMZ instructions!!!!"); 
            if (oldStaleInt == staleInt){
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldStaleInt %lu == staleInt %lu", oldStaleInt, staleInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Bypassing write to persistent storage! TURBOFAST MODE!!!");
                
            } else if (oldStaleInt != staleInt) {
                persist_write_int(STALE_INT_PKEY, staleInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldStaleInt %lu != staleInt %lu", oldStaleInt, staleInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "SLOW TRAIN COMING - wriring %lu to persistent storage!", staleInt);
                oldStaleInt = new_tuple->value->uint32;
            }
            break;
            
		case WEATHER_SUNRISE_KEY:
            sunriseInt = new_tuple->value->uint32;
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "THIS SHOULD SET THE sunriseINT back to the new TUPLE and overwrite TMZ instructions!!!!"); 
            if (oldSunriseInt == sunriseInt) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldSunriseInt %lu == sunriseInt %lu", oldSunriseInt, sunriseInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Bypassing write to persistent storage! TURBOFAST MODE!!!");
                
            } else if (oldSunriseInt != sunriseInt) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldSunriseInt %lu != sunriseInt %lu", oldSunriseInt, sunriseInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "SLOW TRAIN COMING - writing %lu to persistne storage!", sunsetInt);
                persist_write_int(SUNRISE_PKEY, sunriseInt);
                oldSunriseInt = new_tuple->value->uint32;
            }
            break;
            
		case WEATHER_SUNSET_KEY:
            sunsetInt = new_tuple->value->uint32;
            if (oldSunsetInt == sunsetInt) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldSunsetInt %lu == sunsetInt %lu", oldSunsetInt, sunsetInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "Bypassing write to persistent storage! TURBOFAST MODE!!!");
                
            } else if (oldSunsetInt != sunsetInt) {
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "oldSunsetInt %lu != sunsetInt %lu", oldSunsetInt, sunsetInt);
                persist_write_int(SUNSET_PKEY, sunsetInt);
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "SLOW TRAIN COMING - writing %lu to persistne storage!", sunsetInt);
                oldSunsetInt = new_tuple->value->uint32;
            }
            break;
		
		case WEATHER_OFFSET_KEY: 
		offsetInt = new_tuple->value->uint32; 
		if (oldOffsetInt == offsetInt) {		
		} else if (oldOffsetInt != offsetInt) {
			persist_write_int(OFFSET_PKEY, offsetInt);
			oldOffsetInt = new_tuple->value->uint32; 
		}
		break; 
            
            
        case WEATHER_STATUS_KEY:
            break;
            
        case WEATHER_CONDITIONS_KEY:
		text_layer_set_text(conditions_layer, new_tuple->value->cstring); 
            break;
            
        case WEATHER_DESCRIPTION_KEY:
            text_layer_set_text(description_layer, new_tuple->value->cstring);
            
        case WEATHER_CODE_KEY:
            break;
            
        case WEATHER_ICONCODE_KEY:
            break;
	}
}

void debug_functions(void) { 
		Layer *window_layer = window_get_root_layer(window);
        
		if	(lclTimeInt % 1 == 0) {
            
            if (icon_counter < 10) {
                icon_counter = icon_counter +1;
            } else {
                icon_counter = 0;
            }
            
            if (icon_bitmap) {
                gbitmap_destroy(icon_bitmap);
            }
            //          icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[icon_counter]);
            icon_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[lclTimeInt % 8]);
			//APP_LOG(APP_LOG_LEVEL_DEBUG, "weather icon set to %lu", (lclTimeInt % 8));
            bitmap_layer_set_bitmap(icon_layer, icon_bitmap);
            if (battery_counter > 0) {
                battery_counter = battery_counter - 1;
            } else {
                battery_counter = 9;
            }
			if (battery_bar_bitmap) {
                gbitmap_destroy(battery_bar_bitmap);
                battery_bar_bitmap = gbitmap_create_with_resource(BATTERY_ICONS[battery_counter]);
                bitmap_layer_set_bitmap(battery_bar_bitmap_layer, battery_bar_bitmap);
                
                if (refresher_int < 24) {
                    refresher_int = refresher_int + 1;
                } else {
                    refresher_int = 0;
                    fetch_msg();
                }
			}
		}
}

void hour_update() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "hour update triggered"); 
}

char * timeStringer(int32_t timeInt) {
//char timeStringer(timeInt) {
	time_t calc_time_t = timeInt;
	struct tm *calc_tm; 
	calc_tm = localtime (&timeInt);
	char calc_tmString[32];
//	char *calc_tmString; 
	strftime(calc_tmString, sizeof(calc_tmString), "%l:%M%P", calc_tm); 
	char *calc_return; 
	calc_return = calc_tmString; 
	return calc_return; 
}

void minute_update() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "minute update triggered");
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "offsetIntFrom minute_update = %lu", offsetInt); 

	time_t lclTime = time(NULL);
	uint32_t lclTimeInt = lclTime; 
	uint32_t zuluTimeInt = lclTime + (60*60*offsetInt);
	time_t zuluTime = zuluTimeInt; 
	struct tm *timer_tm; //(zuluTime, zulu_time); 	
//	zuluTm = zuluTime; 
	timer_tm = localtime (&zuluTime);
//	strftime(zuluTimeText, sizeof(zuluTimeText), %h, zuluTm); 
	char displayTimeString[32]; 
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "zulu Time rendered is %s", displayTimeString);
	timer_tm = localtime (&lclTime); 
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "local Time rendered is %s", displayTimeString);

	uint32_t diffInt;
	diffInt = (zuluTimeInt) - staleInt;
	
	if (lclTimeInt % 3600 == 0)  {
		hour_update(); 
	}
	///translate sun ints into something you can read
//	struct tm *timer_tm; 
	time_t display_time_t = sunriseInt; 
	timer_tm = localtime (&display_time_t); 
//	char displayTimeString[32]; 
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm); 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "GMT sunrise: %s", displayTimeString); 
	
	uint32_t localSunriseInt = sunriseInt - (3600*offsetInt); 
	display_time_t = localSunriseInt; 
	timer_tm = localtime (&display_time_t); 
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm); 
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "adjusted sunrise %s, and adjusted sunriseInt %% 86400 %lu", displayTimeString, (sunriseInt % 86400)); 
	
	display_time_t = staleInt; 
	timer_tm = localtime(&display_time_t);	
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm); 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "RAW stale time: %s", displayTimeString); 
	
	
	uint32_t localStaleInt = staleInt - (3600*offsetInt); 
	display_time_t = localStaleInt; 
	timer_tm = localtime (&display_time_t); 
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm); 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "adjusted stale time: %s", displayTimeString); 
	
	//change zulu sunset into a local sunset
		
	uint32_t localSunsetInt = sunsetInt - (3600*offsetInt); 
	display_time_t = localSunsetInt; 
	timer_tm = localtime (&display_time_t); 
	strftime(displayTimeString, sizeof(displayTimeString), "%A %l:%M%P", timer_tm); 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "adjusted sunset %s, and adjusted sunsetInt %% 86400 %lu", displayTimeString, (sunsetInt % 86400)); 
	
//	if ( lclTimeInt % 86400 > (sunsetInt - (3600*offsetInt)) % 86400  )  	{    //invert if after sunset
	if ( lclTimeInt % 86400 > localSunsetInt % 86400  )  	{    //invert if after sunset
        layer_set_hidden((inverter_layer_get_layer(inverter_layer)), false);
		night_flag = 1; //set night flag to night
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "after sunset!");
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "sunset %lu > now %lu", 
//				(lclTimeInt % 86400) / 3600, 
//				(sunsetInt - (3600*offsetInt)) % 86400 / 3600);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "now %lu > sunset %lu", 
				(lclTimeInt % 86400) / 3600, 
				(localSunsetInt % 86400) / 3600);
	} else if ( lclTimeInt % 86400 < localSunriseInt % 86400 )  {   //invert if before sunrise
		layer_set_hidden((inverter_layer_get_layer(inverter_layer)), false);
		night_flag = 1; //set flag to night
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "before sunrise!");
		APP_LOG(APP_LOG_LEVEL_DEBUG, "now %lu < sunrise %lu", 
				(lclTimeInt % 86400) / 3600, 
//				(sunriseInt - (3600*offsetInt)) % 86400 / 3600);
				(localSunriseInt % 86400) / 3600);
	
	} else {		//it must be daytime! Hide inverter layer
    	layer_set_hidden((inverter_layer_get_layer(inverter_layer)), true);
		night_flag = 0; //set night flag to day
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "must be day!");
//		APP_LOG(APP_LOG_LEVEL_DEBUG, "sunrise %lu < now %lu > sunset %lu", 
//				(sunriseInt - (3600*offsetInt)) % 86400 / 3600, 
//				(lclTimeInt % 86400) / 3600, 
//				(sunsetInt - (3600*offsetInt)) % 86400 / 3600 );
		APP_LOG(APP_LOG_LEVEL_DEBUG, "sunrise %lu < now %lu > sunset %lu", 
				(localSunriseInt % 86400) / 3600, 
				(lclTimeInt % 86400) / 3600, 
				(localSunsetInt % 86400) / 3600);
	}
	   
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "lclTimeInt %lu", lclTimeInt); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "lclTimeInt mod 86400 %lu %lu", lclTimeInt % 86400, (lclTimeInt % 86400) / 3600); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "sunriseInt %lu %lu", sunriseInt - (3600*offsetInt), sunriseInt); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "sunriseInt mod 86400 %lu %lu", (sunriseInt - (3600*offsetInt)) % 86400, ((sunriseInt - (3600*offsetInt)) % 86400) / 3600); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "sunsetInt %lu %lu", sunsetInt + (3600*offsetInt), sunsetInt); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "sunsetInt mod 86400 %lu %lu", (sunsetInt - (3600*offsetInt)) % 86400, ((sunsetInt - (3600*offsetInt)) % 86400) / 3600); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt %lu", diffInt + (3600*offsetInt)); 
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt mod 86400 %lu %lu", (diffInt + (3600*offsetInt)) % 86400, ((diffInt + (3600*offsetInt)) % 86400) / 3600); 
	

				
		static char diffString[24]; // = "1111111111";
	snprintf(diffString, sizeof(diffString), "%lu", diffInt);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt %lu = zuluTimeInt %lu - staleInt %lu and diffString %s", diffInt, zuluTimeInt, staleInt, diffString);
		
		if (diffInt < 60) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt < 60"); 
		
		snprintf(diffString, 32, "%lu", diffInt);  // snprintf(count_text, sizeof(count_text), "%d", (time_stamp_int / 1 / 1 / 1));
		APP_LOG(APP_LOG_LEVEL_DEBUG, "diffString %s", diffString);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt (%lu) = zuluTimeInt (%lu) - staleInt (%lu) and diffString (%s)", diffInt, zuluTimeInt, staleInt, diffString);
		char time_id[8] = " sec";
		strcat(diffString, time_id); 
//        APP_LOG(APP_LOG_LEVEL_DEBUG, diffString);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "time_id: = %s", time_id);
	    text_layer_set_text(status_layer, diffString);
		
	} else if (diffInt < 3600) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt < 3600"); 
		snprintf(diffString, 32, "%lu", (diffInt / 60));  // snprintf(count_text, sizeof(count_text), "%d", (time_stamp_int / 1 / 1 / 1));
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffString %s", diffString);
		char time_id[8] = " min";
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "time_id: = %s", time_id);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt (%lu) = zuluTimeInt (%lu) - staleInt (%lu) and diffString (%s)", diffInt, zuluTimeInt, staleInt, diffString);
		strcat(diffString, time_id); 
        //APP_LOG(APP_LOG_LEVEL_DEBUG, diffString);
	    text_layer_set_text(status_layer, diffString);
    } else if (diffInt < 86400) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt < 86400"); 
        snprintf(diffString, 32, "%lu", (diffInt / 60 / 60));  // snprintf(count_text, sizeof(count_text), "%d", (time_stamp_int / 1 / 1 / 1));
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffString %s", diffString);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt (%lu) = zuluTimeInt (%lu) - staleInt (%lu) and diffString (%s)", diffInt, zuluTimeInt, staleInt, diffString);
		char time_id[8] = " hr";
		strcat(diffString, time_id); 
        //APP_LOG(APP_LOG_LEVEL_DEBUG, diffString);
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "time_id: = %s", time_id);
	    text_layer_set_text(status_layer, diffString);
	} else if (diffInt > 2678400 ) {   //if greater than one month, must be an errro!! (but probably not completely invalid)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "ERROR: diffInt > 2678400"); 
		text_layer_set_text(status_layer, "     ");	
	} else {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt else");  //everything else that doesn't match is in days
		snprintf(diffString, 32, "%lu", (diffInt / 60 / 60 / 24));  // snprintf(count_text, sizeof(count_text), "%d", (time_stamp_int / 1 / 1 / 1));
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffString %s", diffString);
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "diffInt (%lu) = zuluTimeInt (%lu) - staleInt (%lu) and diffString (%s)", diffInt, zuluTimeInt, staleInt, diffString);
		char time_id[8] = " d";
		strcat(diffString, time_id); 
        //APP_LOG(APP_LOG_LEVEL_DEBUG, diffString);
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "time_id: = %s", time_id);
	    text_layer_set_text(status_layer, diffString);
	}	
    
    if (debug_flag == 1) {
		debug_functions(); 
    } else {	
		APP_LOG(APP_LOG_LEVEL_DEBUG, "lclTimeInt %lu %% 600 = %lu", lclTimeInt, lclTimeInt % 600); 
		if (lclTimeInt % 600 == 0) {
            fetch_msg();
        }   
    }
}

static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
    static char time_text[] = "00:00AAA"; // Needs to be static because it's used by the system later. "%l:%M %y"
	strftime(time_text, sizeof(time_text), "%l:%M%P", tick_time);
	text_layer_set_text(time_layer, time_text);
    static char date_text[] = "00/00";
	strftime(date_text, sizeof(date_text), "%d", tick_time);
	text_layer_set_text(date_layer, date_text);

	time_t lclTime = time(NULL);
	uint32_t zuluTimeInt = lclTime + (60*60*offsetInt);
	uint32_t lclTimeInt = lclTime; 
	time_t zuluTime = zuluTimeInt; 

	if (lclTimeInt % 12 == 4) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "false, true, true");
        layer_set_hidden(text_layer_get_layer(status_layer), false);
        layer_set_hidden(text_layer_get_layer(conditions_layer), true);
        layer_set_hidden(text_layer_get_layer(description_layer), true);
    

    } else if (lclTimeInt % 12 == 8) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "true, false, true");
    layer_set_hidden(text_layer_get_layer(status_layer), true);
    layer_set_hidden(text_layer_get_layer(conditions_layer), false);
    layer_set_hidden(text_layer_get_layer(description_layer), true);

    } else if (lclTimeInt % 12 == 0) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "true, true, false");
        layer_set_hidden(text_layer_get_layer(status_layer), true);
        layer_set_hidden(text_layer_get_layer(conditions_layer), true);
        layer_set_hidden(text_layer_get_layer(description_layer), false);
    }
	if (lclTimeInt % 60 == 0) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "offsetIntFrom handle_second_tick = %lu", offsetInt); 
		minute_update(); 
//			text_layer_set_text(status_layer, "refresh");
//            fetch_msg();
		
	}
	
    handle_battery(battery_state_service_peek());
	/*
			if (custom_font_small_location) {}
		else { 
			GFont custom_font_small_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_22));
		}
		
		if (custom_font_tiny_location) {}
		else {
			GFont custom_font_tiny_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_17));
		}
		*/
	
	/*
	APP_LOG(APP_LOG_LEVEL_DEBUG, "lclTimeInt %% 6 = %lu", lclTimeInt % 6); 
	if (lclTimeInt % 6 == 1 ) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "setting text to large, lclTimeInt %lu %% 6 = %lu", lclTimeInt % 6, lclTimeInt % 3); 
	GFont custom_font_large_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_27));
	text_layer_set_font(location_layer, custom_font_large_location);
	layer_set_frame(text_layer_get_layer(location_layer), (GRect(0, 115, 144, 40)));
	text_layer_set_overflow_mode(location_layer, GTextOverflowModeTrailingEllipsis); 
//	layer_mark_dirty(text_layer_get_layer(location_layer)); 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "setting ocation_layer, GTextOverflowModeTrailingEllipsis");
} else if (lclTimeInt % 6 == 3) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "setting text to small, lclTimeInt %lu %% 6 = %lu", lclTimeInt % 6, lclTimeInt % 3);
	text_layer_set_overflow_mode(location_layer, GTextOverflowModeWordWrap); 
	GFont custom_font_small_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_22));
	text_layer_set_font(location_layer, custom_font_small_location); 
	layer_set_frame(text_layer_get_layer(location_layer), (GRect(0, 119, 144, 100)));
//	layer_mark_dirty(text_layer_get_layer(location_layer));

} else if (lclTimeInt % 6 == 5) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "setting text to tiny, lclTimeInt %lu %% 6 = %lu", lclTimeInt % 6, lclTimeInt % 3); 
	GFont custom_font_tiny_location = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_17));
	text_layer_set_font(location_layer, custom_font_tiny_location); 
	layer_set_frame(text_layer_get_layer(location_layer), (GRect(0, 125, 144, 100)));
	text_layer_set_overflow_mode(location_layer, GTextOverflowModeWordWrap);
//	layer_mark_dirty(text_layer_get_layer(location_layer));
}  */
}


static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "window load, tuplet default values");
	Tuplet initial_values[] = {
		TupletInteger(WEATHER_ICON_KEY, (int) iconInt),
        TupletCString(WEATHER_TEMPERATURE_KEY, temp_read_key),
		TupletCString(WEATHER_LOCATION_KEY, location_read_key),
        //		APP_LOG(APP_LOG_LEVEL_DEBUG, "inside tuple initial vales location_read_key = %s", location_read_key);
		TupletCString(WEATHER_STATUS_KEY, "checking"),
		TupletCString(WEATHER_CONDITIONS_KEY, "weather"),
        TupletCString(WEATHER_DESCRIPTION_KEY, "gas clouds"),
//		TupletCString(WEATHER_DESCRIPTION_KEY, "Gas Attack!"),
		TupletInteger(WEATHER_OFFSET_KEY, (int) 23), 
		TupletCString(WEATHER_CODE_KEY, "000"),
		TupletCString(WEATHER_ICONCODE_KEY, "xFm"),
		TupletInteger(WEATHER_STALEDATE_KEY, (int) staleInt),
		TupletInteger(WEATHER_SUNRISE_KEY, (int) sunriseInt),
		TupletInteger(WEATHER_SUNSET_KEY, (int) sunsetInt),
	};
    
	//layer create, x, y, w, h
	time_layer = text_layer_create(GRect(0, 1, 144 * .75, 50));
	date_layer = text_layer_create(GRect(92+4, -1, 52, 32));
	date_border_layer = bitmap_layer_create(GRect(99+4, 1, 38, 35));
	line_layer = text_layer_create(GRect(0, 36, 144, 1));
	line_layer2 = text_layer_create(GRect(0, 39, 144, 1));
	if (battery_bar_bitmap_layer) {
		bitmap_layer_destroy(battery_bar_bitmap_layer);
	}
	battery_bar_bitmap_layer = bitmap_layer_create(GRect(0, 37, 144, 2));
	if (icon_layer) {
		bitmap_layer_destroy(icon_layer);
	}
	icon_layer = bitmap_layer_create(GRect(9, 54+4, 50, 50));
	temperature_layer = text_layer_create(GRect(64, 63, 74, 36));
	status_layer = text_layer_create(GRect(65, 92, 144, 36));
	conditions_layer = text_layer_create(GRect(65, 92, 144, 36));
	description_layer = text_layer_create(GRect(65, 92, 144, 36));
	location_layer = text_layer_create(GRect(-10, 111-3, 164, 50));
	code_layer = text_layer_create(GRect(0, 168-20, 144, 40));
	
	bt_layer = text_layer_create(GRect(2, 41, 15, 19)); 
	build_layer = text_layer_create(GRect(130, 168-127, 144-130, 40));
	comm_status_layer = bitmap_layer_create(GRect(144-18, 168-127, 18, 18));
    //	comm_status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_COMM_STATUS);
    //	bitmap_layer_set_alignment(comm_status_layer, GAlignCenter);
    //	bitmap_layer_set_bitmap(comm_status_layer, comm_status_bitmap); //
    
    
    //Layer set attributes
	
	//set text attributes: 

GFont custom_font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TAHOMA_BOLD_27));			
	
	//attributes: time_layer
	text_layer_set_text_color(time_layer, GColorBlack);
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_alignment(time_layer, GTextAlignmentLeft);
	text_layer_set_overflow_mode(time_layer, GTextOverflowModeWordWrap);
	text_layer_set_font(time_layer, custom_font_time);
	text_layer_set_overflow_mode(time_layer, GTextOverflowModeTrailingEllipsis);
    
    
	//attributes: date_layer
	text_layer_set_text_color(date_layer, GColorBlack);
	text_layer_set_background_color(date_layer, GColorClear);
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	
    
	//attributes: date_border_layer
	date_layer_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DATE_BORDER);
	bitmap_layer_set_bitmap(date_border_layer, date_layer_bitmap);
	
	//attributes: line_layer
	text_layer_set_background_color(line_layer, GColorBlack);
	text_layer_set_background_color(line_layer2, GColorBlack);
    
	text_layer_set_text_color(temperature_layer, GColorBlack);
	text_layer_set_background_color(temperature_layer, GColorClear);
	text_layer_set_font(temperature_layer, custom_font_time);
	text_layer_set_text_alignment(temperature_layer, GTextAlignmentLeft);
    
	//attributes status_layer
	text_layer_set_text_color(status_layer, GColorBlack);
	text_layer_set_background_color(status_layer, GColorWhite);
	text_layer_set_background_color(conditions_layer, GColorWhite);
	text_layer_set_text_alignment(status_layer, GTextAlignmentLeft);
	text_layer_set_text_alignment(conditions_layer, GTextAlignmentLeft);
    //	text_layer_set_font(status_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
	GFont custom_font_status = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ARIAL_17));
	text_layer_set_font(status_layer, custom_font_status);
	text_layer_set_font(conditions_layer, custom_font_status);
	text_layer_set_font(description_layer, custom_font_status);
	text_layer_set_overflow_mode(status_layer, GTextOverflowModeTrailingEllipsis);
	text_layer_set_overflow_mode(conditions_layer, GTextOverflowModeTrailingEllipsis);
	text_layer_set_overflow_mode(description_layer, GTextOverflowModeTrailingEllipsis);
	
	
	//attributes location_layer
	text_layer_set_text_color(location_layer, GColorBlack);
	text_layer_set_background_color(location_layer, GColorClear);
//	text_layer_set_font(location_layer, custom_font_time);
  	text_layer_set_text_alignment(location_layer, GTextAlignmentCenter);
	
	//attributes code_layer
	text_layer_set_text_color(code_layer, GColorBlack);
	text_layer_set_background_color(code_layer, GColorClear);
	text_layer_set_font(code_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	
	    
	text_layer_set_text_alignment(bt_layer, GTextAlignmentCenter);
	text_layer_set_background_color(bt_layer, GColorBlack);
	text_layer_set_text_color(bt_layer, GColorWhite);
	text_layer_set_font(bt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(bt_layer, "B");
	layer_set_hidden(text_layer_get_layer(bt_layer), true);
	
	//attributes build_layer
	text_layer_set_text_alignment(build_layer, GTextAlignmentCenter);
	text_layer_set_background_color(build_layer, GColorBlack);
	text_layer_set_text_color(build_layer, GColorWhite);
	text_layer_set_font(build_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(build_layer, "N");
    
    
    // add layers to window
	layer_add_child(window_layer, text_layer_get_layer(time_layer));
	layer_add_child(window_layer, bitmap_layer_get_layer(date_border_layer));
	layer_add_child(window_layer, text_layer_get_layer(date_layer));
	layer_add_child(window_layer, text_layer_get_layer(line_layer)); //
	layer_add_child(window_layer, bitmap_layer_get_layer(battery_bar_bitmap_layer));
	layer_add_child(window_layer, bitmap_layer_get_layer(icon_layer));
	layer_add_child(window_layer, text_layer_get_layer(temperature_layer));
	layer_add_child(window_layer, text_layer_get_layer(status_layer));
	layer_add_child(window_layer, text_layer_get_layer(conditions_layer));
	layer_add_child(window_layer, text_layer_get_layer(description_layer));
	layer_add_child(window_layer, text_layer_get_layer(location_layer));
	layer_add_child(window_layer, text_layer_get_layer(build_layer));
	layer_add_child(window_layer, text_layer_get_layer(code_layer));
	layer_add_child(window_layer, text_layer_get_layer(bt_layer)); 
	layer_add_child(window_layer, bitmap_layer_get_layer(comm_status_layer));
	layer_set_hidden(bitmap_layer_get_layer(comm_status_layer), true);
	layer_set_hidden(text_layer_get_layer(build_layer), true);
	
	layer_add_child(window_layer, text_layer_get_layer(line_layer));
	layer_add_child(window_layer, text_layer_get_layer(line_layer2)); //
	layer_set_hidden(text_layer_get_layer(line_layer), true);
	layer_set_hidden(text_layer_get_layer(line_layer2), true);
    
	
    //	InverterLayer inverter_layer;
	inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
	layer_set_hidden(inverter_layer_get_layer(inverter_layer), true);
	layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
	
	handle_bluetooth(bluetooth_connection_service_peek());
	
	hour_update();
	
	minute_update();
			
	tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
	
	bluetooth_connection_service_subscribe(&handle_bluetooth);
	
	
//	battery_state_service_subscribe(batteryStateHandler);	

	
	app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
                  sync_tuple_changed_callback, sync_error_callback, NULL);
	
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "window load complete");
	//psleep(1000); 
	//window_stack_pop_all(animated); 
}

static void window_unload(Window *window) {
    app_sync_deinit(&sync);
    
	if (icon_bitmap) {
		gbitmap_destroy(icon_bitmap);
	}
	if (date_layer_bitmap) {
		gbitmap_destroy(date_layer_bitmap);
	}
	
	if (battery_bar_bitmap) {
		gbitmap_destroy(battery_bar_bitmap);
	}
    
	text_layer_destroy(time_layer);
	text_layer_destroy(date_layer);
	bitmap_layer_destroy(date_border_layer);
	text_layer_destroy(line_layer);
	text_layer_destroy(line_layer2);
    bitmap_layer_destroy(battery_bar_bitmap_layer);
	bitmap_layer_destroy(icon_layer);
	text_layer_destroy(temperature_layer);
	text_layer_destroy(status_layer);
	text_layer_destroy(conditions_layer); 
	text_layer_destroy(location_layer);
	text_layer_destroy(code_layer);
	text_layer_destroy(bt_layer); 
	text_layer_destroy(build_layer);
	bitmap_layer_destroy(comm_status_layer);
	inverter_layer_destroy(inverter_layer);
	
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	
	if (layer_get_hidden(inverter_layer_get_layer(inverter_layer)) == true) {
		layer_set_hidden((inverter_layer_get_layer(inverter_layer)), false);
	} else if (layer_get_hidden(inverter_layer_get_layer(inverter_layer)) == false) {
		layer_set_hidden((inverter_layer_get_layer(inverter_layer)), true);
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	light_enable(false);
	if (light_flag == 0) {
		light_flag = 1;
	} else if (light_flag == 1) {
		light_flag = 0;
	}
    //	light_enable(true);
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    //	light_enable(false);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	text_layer_set_text(status_layer, "button...");
	fetch_msg();
	if (debug_flag == 0) {
		debug_flag = 1;
	} else if (debug_flag == 1) {
        //		Layer *window_layer = window_get_root_layer(window);
        //		layer_remove_from_parent(text_layer_get_layer(code_layer));
        //		layer_remove_from_parent(text_layer_get_layer(build_layer));
        debug_flag = 0;
	}
	
	if (night_flag == 1) {
		night_flag = 0; 
	} else if (night_flag == 0) {
		night_flag = 1; 
	}
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    //	window_long_click_subscribe(BUTTON_ID_DOWN, 500, down_long_click_handler, NULL);
    //	window_long_click_subscribe(BUTTON_ID_DOWN, 0, select_long_click_handler, NULL);
    
}

static void init() {
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "weather nigthly build");
    //	APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather 2.5 stable");
    
	window = window_create();
	window_set_background_color(window, GColorWhite);
	window_set_fullscreen(window, true);
	window_set_click_config_provider(window, click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
        
    });
    
    const int inbound_size = 2048;
    const int outbound_size = 2048;
    app_message_open(inbound_size, outbound_size);
    
    const bool animated = true;
	
	sunriseInt	 	= (persist_exists(SUNRISE_PKEY)) ? persist_read_int(SUNRISE_PKEY)  : SUNRISE_DEFAULT;
	oldSunriseInt	= sunriseInt;
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = sunrise %lu", sunriseInt);
	
	sunsetInt 		= persist_exists(SUNSET_PKEY) ? persist_read_int(SUNSET_PKEY)  : SUNSET_DEFAULT;
	oldSunsetInt = sunsetInt; 
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = sunset %lu", sunsetInt);
    
	
	staleInt 	= persist_exists(STALE_INT_PKEY)	? persist_read_int(STALE_INT_PKEY) 	: STALE_INT_DEFAULT;
	oldStaleInt = staleInt; 
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = staleInt %lu", staleInt);
	
	iconInt 		= persist_exists(ICON_PKEY) 		? (persist_read_int(ICON_PKEY)) 		: ICON_DEFAULT;
	oldIconInt		= iconInt; 
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = icon %lu", iconInt);
	
	offsetInt 		= persist_exists(OFFSET_PKEY) 		? (persist_read_int(OFFSET_PKEY)) 		: OFFSET_DEFAULT;
	oldOffsetInt	= offsetInt; 
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = offset %lu", offsetInt);
	
	
	if (persist_exists(LOCATION_PKEY)) {
        //	APP_LOG(APP_LOG_LEVEL_DEBUG, "location key exists");
		persist_read_string(LOCATION_PKEY, location_read_key, 24);
        //		old_location = location_read_key; 
        //		new_location = location_read_key; 
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = location %s", location_read_key); 
	} else {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "FAST BYPASS: location key does not exist");
	}
    //	location_string = masterblaster; 
	
	if (persist_exists(TEMP_PKEY)) {
        //	APP_LOG(APP_LOG_LEVEL_DEBUG, "temperature key exists");
		persist_read_string(TEMP_PKEY, temp_read_key, 24);
        //		old_temp = temp_read_key; 
        //		new_temp = temp_read_key; 
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "persistent read = temperature %s", temp_read_key); 
	} else {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "FAST BYPASS: temperature does not exist");
	}
	
	window_stack_push(window, animated);
}

static void deinit() {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}