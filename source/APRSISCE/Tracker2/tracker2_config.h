#define OPT_SMARTBEACON 0x80
#define OPT_ALTITUDE 0x40
#define OPT_VELOCITY 0x20
#define OPT_TIMESTAMP 0x10
#define OPT_GPSQUALITY 0x08
#define OPT_PTTINPUT 0x04
#define OPT_TIMESLOT 0x02
#define OPT_COMPRESSED 0x01

#define OPT2_TEMPERATURE 0x80
#define OPT2_VOLTAGE 0x40
#define OPT2_REQUIREALL 0x20
#define OPT2_FIXEDPOSITION 0x10
#define OPT2_XMITONSWITCH 0x08
#define OPT2_POWERCTRL 0x04
#define OPT2_OUTPUT1 0x02
#define OPT2_TIME_HMS 0x01

#define OPT3_COUNTER 0x80
#define OPT3_COUNTER_RESET 0x40
#define OPT3_CD_INVERT 0x20
#define OPT3_PTT_SHARED 0x10
#define OPT3_NOFIX 0x08
//#define OPT3_TELEMETRY 0x04
// Telemetry is now in interval only
#define OPT3_DAO 0x02
// T2 only
#define OPT3_CUSTOM_SYMBOLS 0x01

#define OPT4_RUN_SCRIPT 0x80
#define OPT4_DIGI_MYCALL 0x40
#define OPT4_SOFTWARE_DCD 0x20
#define OPT4_MONITOR 0x10
#define OPT4_WAYPOINTS 0x08
#define OPT4_1WIREWX 0x04
#define OPT4_WPT_LEN_LIMIT 0x03

#define OPT5_HEADERLN 0x80
#define OPT5_RING 0x40
#define OPT5_FAHRENHEIT 0x20
#define OPT5_AUTOBAUD 0x10
#define OPT5_CLIMB 0x08
#define OPT5_CAMERA 0x04
#define OPT5_AUTOSAVE 0x02
#define OPT5_EXTSQL 0x01

#define OPT6_ALT_PATH 0x80
#define OPT6_BOTTOM_SLOT 0x40
#define OPT6_PASSALL 0x20
#define OPT6_FILTER 0x10
#define OPT6_TRACE 0x08
#define OPT6_PROPWPT 0x04
#define OPT6_OTPAUTH 0x02
#define OPT6_DOUBLE 0x01

#define OPT7_GAIN4X	0x80
#define OPT7_GAIN8X	0x40
#define OPT7_EXTTEMP 0x04
#define OPT7_USBKISS 0x01

// Global options
#define OPTG_POLARITY 0x20

#define CS_JUMPER 0x01
#define CS_ADC0 0x02
#define CS_ADC1 0x04
#define CS_ADC2 0x08
#define CS_ALTITUDE 0x10
#define CS_SPEED 0x20
#define CS_NOFIX 0x40

#define DIGI_ALIASES 8
#define DIGI_OPT_ID 0x80
#define DIGI_OPT_PREEMPT 0x40

#define MODE_AUTO 0
#define MODE_GARMIN 1
#define MODE_KISS 2
#define MODE_LACROSSE 3
#define MODE_FLOW 4
#define MODE_TEXT 5
#define MODE_DAVIS 6

#define HBAUD_1200 0
#define HBAUD_300 1
#define HBAUD_PSK31 2

#define TEMP_CELSIUS 0
#define TEMP_FAHRENHEIT 1

#define SW_DCD_THRESHOLD 20

#define ACCESS_LIST_SIZE 6

// 8 bytes
struct digi_config
{
	unsigned char alias[6];
	unsigned char n;
	unsigned char opt;
};

// Tracker configuration data structure

#pragma pack(1)
// 191 bytes
struct tracker_config {
	unsigned char tx_delay;
	unsigned short tx_interval;
	unsigned char options;
	unsigned char options2;
	char fromcall[7];
	unsigned char pathsize;
	char path[21];
	unsigned char aprs_table;
	unsigned char aprs_symbol;
	unsigned short tx_slow_interval;
	unsigned short timeslot;
	unsigned char sb_low_speed;
	unsigned char sb_high_speed;
	unsigned char txlevel;				// Audio drive level
	unsigned char sb_minangle;
	unsigned char sb_mininterval;
	  signed char temp_offset;
	unsigned char comment_len;
	unsigned char cs_speed;
	unsigned short  cs_altitude;
	unsigned char cs_adc2;
	unsigned char cs_adc1;
	unsigned char cs_adc0;
	unsigned char cs_mask;				// Config select enable mask
	unsigned char cs_invert;			// Config select test invert bitmap
	unsigned char power_delay;			// Time in seconds to wait for radio powerup
	unsigned char status_tm_interval;// How often to send the status text and telemetry
	unsigned char options3;
	unsigned char debounce;				// Debouce time for counter
	unsigned char quiet_time;			// Channel clear wait time
	unsigned char lv_inhibit;			// Low voltage inhibit
	char comment[64];						// Freeform text
	
	unsigned char digi_enable;			// Bitmap to turn on and off digi aliases
	unsigned char options4;				// KISS mode
	unsigned char dupe_time;			// Duplicate TTL for digipeating
	
	unsigned char msg_retries;			// Number of times to retry messages
	unsigned char msg_timeout;			// Timeout in seconds between retries
	unsigned char options5;
	unsigned char authtype;				// Was intended to indicate authentication method
	char latitude[8];						// Fixed position latitude
	char longitude[9];					// Fixed position longitude
	unsigned char porta;					// Serial port A config
	unsigned char portb;					// Serial port B config
	unsigned char maxrange;				// Waypoint max range
	unsigned char amode;					// Port A mode
	unsigned char bmode;					// Port B mode
	unsigned char hbaud;					// On-air baud rate
	
	unsigned char vane_adjust;			// 1-wire wind vane adjustment
	unsigned char nice;					// Factor to reduce transmit rate by	

	unsigned char channel;				// Secondary (high nybble) and primary channel
	unsigned char secrate;				// Secondary frequency TX rate
	
	unsigned char options6;
	unsigned char options7;
	
	unsigned char reserved[30];
};

// 5 + (191 * 2) + (8 * 8) + (6 * 6) = 487 bytes
typedef struct {
	unsigned char product_id;
	unsigned char product_version;
	unsigned short product_build;
	unsigned char global_options;
	struct tracker_config tconfig[2];
	struct digi_config digi[DIGI_ALIASES];
	char access_list[ACCESS_LIST_SIZE][6];
	unsigned long secret[4];			// One-time password generator key
	unsigned short otp_sequence;			// Password sequence number
} device_config;

extern const struct tracker_config *config;

#pragma pack(0)

#pragma CONST_SEG FLASH_CONFIG
extern const device_config globalcfg;
#pragma CONST_SEG DEFAULT
