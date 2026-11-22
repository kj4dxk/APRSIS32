#define PORT_A_WX 0x01
#define PORT_B_WX 0x02
#define PORT_A_NMEA 0x04
#define PORT_B_NMEA 0x08
#define PORT_A_MASK (PORT_A_WX | PORT_A_NMEA)
#define PORT_B_MASK (PORT_B_WX | PORT_B_NMEA)

#define GPS_EPE_FLAG		31
#define GPS_INVALID_FLAG	255

#define CANARY_VALUE (unsigned char)0xe6

#define SCRATCH_SIZE 118

typedef struct {
	signed long latitude;
	signed long longitude;
	unsigned long altitude;			//	In 1/100 meters, from -10000m
	unsigned long velocity;
	char aprs_position[17];
	char aprs_altitude[6];
	char aprs_course[3];
	char aprs_speed[3];
	char aprs_ddhhmmss[8];
	char aprs_dao[2]; 
	unsigned char hdop;
	unsigned gotfirstfix	: 1;
	unsigned fixtype		: 2;
	unsigned satsinview		: 5;
	unsigned velocity_valid : 1;
	unsigned altitude_valid : 1;
	unsigned position_valid	: 1;
	unsigned time_valid		: 1;
	unsigned date_valid		: 1;
	unsigned quality_valid	: 1;
	unsigned neg_altitude	: 1;
	unsigned fix_expired	: 1;
	unsigned int climb;				// Vertical speed, feet/minute
	unsigned neg_climb		: 1;	// Rate of climb is negative
	unsigned long maxaltitude;		// Max altitude attained	
} GPS_FIX;

void select_profile(unsigned char newprofile);
void refresh_profile(void);
char ddtoc(char *val);
unsigned int tdtoi(char *val);

#pragma DATA_SEG __SHORT_SEG _DATA_ZEROPAGE

extern unsigned int second, gps_fix_age;
extern unsigned int cd_stat, tx_counter;
extern unsigned char portinfo;
extern unsigned int clock_ticks, tx_interval, tx_counter, counter, gps_fix_age;

#pragma DATA_SEG ZEROPAGE144

extern char ctemp[];

#pragma DATA_SEG ZEROPAGE160

extern unsigned char nice_count;
extern GPS_FIX fixdata;

#pragma DATA_SEG DEFAULT

extern unsigned int lastdigi;

char ParseGPS(char gpsdata);
long semicircles(char *degstr, char hemi);
void nmea_init(void);

extern char scratch[SCRATCH_SIZE];
extern volatile unsigned char canary;
extern unsigned int otp_sequence;