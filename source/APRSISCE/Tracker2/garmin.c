/* Garmin protocol routines

   Notes on Garmin binary interface protocol:
   
   Physical layer is 9600-N-8-1.   
   Packets have the format: <DLE> <PID> <Length> <Payload> <Checksum> <DLE> <ETX>
   All fields are single bytes, except for the variable-length payload.
   Commands should use a minimum of 2 bytes in payload for compatibility.
   DLE characters in the packet are doubled.

*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "garmin.h"
#include "hardware.h"
#include "utility.h"
#include "math.h"
#include "tracker2_main.h"
#include "tracker2_config.h"
#include "events.h"
#include "status.h"
#include "geo.h"
#include "command.h"
#include "packet.h"

#define GARMIN_BUFFER_SIZE 100
#define PVT_TIMEOUT 11
#define FMI_TIMEOUT 4
#define PRODUCT_RQST_RETRY 3

#define STATE_DLE_WAIT 0
#define STATE_PID 1
#define STATE_LENGTH 2
#define STATE_PAYLOAD 3
#define STATE_CHECKSUM 4
#define STATE_END_DLE 5
#define STATE_ETX 6
#define STATE_PROCESS 7

const char user_subclass[] = {0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const char protocol_array[] = {'P', 0, 0, 'L', 1, 0, 'A', 10, 0, 'A', 100, 0, 'D', 109, 0};

// Wrapper to send byte to appropriate port(s)
void garmin_outc(char c)
{
	if (config->amode == MODE_GARMIN) sci_outc(c);
	if (config->bmode == MODE_GARMIN) sci_outc_b(c);
}

// Exit Garmin mode
void garmin_quit(void)
{
	if (config->amode == MODE_GARMIN) 
	{
		strcpy(scratch, "AMODE A");
		process_command(scratch, flags.current_config, 1);		
	}
	if (config->bmode == MODE_GARMIN) 
	{
		strcpy(scratch, "BMODE A");
		process_command(scratch, flags.current_config, 1);
	}
}

// Garmin D109 Waypoint Structure
typedef struct {
	unsigned char dtyp;				// data packet type (0x01 for D109)
	unsigned char wpt_class;		// class
	unsigned char dspl_color;		// display & color (see below)
	unsigned char attr;				// attributes (0x70 for D109)
	unsigned short smbl;			// waypoint symbol
	unsigned char subclass[18];		// subclass
	signed long lat;				// position
	signed long lon;
	float alt;						// altitude in meters
	float dpth;						// depth in meters
	float dist;						// proximity distance in meters
	char state[2];					// state
	char cc[2];						// country code
	unsigned long ete;				// outbound link ete in seconds
	char ident;						// Followed by comment and 4 null fields
} D109_Wpt_Type;

// Garmin D800 Position/Velocity/Time Structure (reverse order)
typedef struct {
	unsigned long wn_days;			// week number days
	signed int leap_scnds;			// difference between GPS and UTC (seconds)
	float msl_hght;					// height of WGS84 ellipsoid above MSL(meters)
	float up;						// velocity up (meters/second)
	float north;					// velocity north (meters/second)
	float east;						// velocity east (meters/second)
	double lon;
	double lat;						// radians
	double tow;						// time of week (seconds)
	unsigned int fix;				// type of position fix (unusable = 0, invalid = 1, 2D = 2, 3D = 3, 2D_diff = 4, 3D_diff = 5
	float epv;						// epe, but vertical only (meters)
	float eph;						// epe, but horizontal only (meters)
	float epe;						// estimated position error, 2 sigma (meters)
	float alt;						// altitude above WGS 84 ellipsoid (meters)
} D800_Pvt_Data_Type;

typedef struct {
	unsigned char tag;
	unsigned int data;
} Protocol_Data_Type;

void process_garmin(unsigned char pid, char *payload, unsigned char length);

volatile unsigned char pending_ack, pvt_timer, fmi_timer, gflags;
unsigned char state = STATE_DLE_WAIT;
unsigned char wpt_protocol = WPT_UNKNOWN;
unsigned long fmi_msg_id = 0xffffff;

static unsigned char pid, length, calc_checksum;
static char payload[GARMIN_BUFFER_SIZE];

// Initialize Garmin protocol interface
// On initial startup, sends product request.  Subsequent calls start PVT data.
void garmin_init(void)
{
	if (config->amode == MODE_GARMIN) set_sci1_rate_9600;
	if (config->bmode == MODE_GARMIN) set_sci2_rate_9600;
	pending_ack = 0;
	gflags &= ~GF_FMI_ON;
	fmi_timer = FMI_TIMEOUT;
	pvt_timer = PVT_TIMEOUT;
	state = STATE_DLE_WAIT;
	// Ask for waypoint type if not known
	if (wpt_protocol == WPT_UNKNOWN)
	{
		garmin_send_packet(Pid_Product_Rqst, "\x00", 2);
		pvt_timer = PRODUCT_RQST_RETRY;
	}
	// Otherwise request PVT start
	else garmin_send_packet(Pid_Command_Data, "\x31", 2);
}

// Handle Garmin protocol events
void garmin_handler(void)
{
	unsigned char acked_pid[2];
	if (pending_ack)
	{
		acked_pid[0] = pending_ack;
		acked_pid[1] = 0;
		garmin_send_packet(Pid_Ack_Byte, (char *)acked_pid, 2);
		pending_ack = 0;
	}

	if (state == STATE_PROCESS)
	{
		// Process the payload
		process_garmin(pid, payload, length);
		state = STATE_DLE_WAIT;
	}

	// Alternate between FMI start and queue check messages
	if (state == STATE_DLE_WAIT && fmi_timer == 0)
	{
		if (gflags & GF_FMI_MSG)
		{
			garmin_send_packet(Pid_FMI, "\x00\x00", 2); // Enable fleet management interface
			garmin_send_packet(Pid_FMI, "\x37\x01\x04APRS", 8); // Define APRS waypoint category
		}
		else
		{
			// Kick the queue in case we missed a message
			garmin_send_packet(Pid_FMI, "\x25\x00\xff\xff\xff\xff", 6);
		}
		fmi_timer = FMI_TIMEOUT;
		gflags ^= GF_FMI_MSG;
	}
	// If we haven't heard from the GPS lately, reset PVT mode
	if (!pvt_timer)
	{
		garmin_init();
	}
	// If the time block changed and we're using D607, erase old waypoints
	if (wpt_protocol == WPT_D607 && (gflags & GF_EXPIRE_COHORT))
	{
		scratch[0] = 0x35;								// 0x0135 = Delete waypoint by category
		scratch[1] = 0x01;
		scratch[2] = 1 << (gflags & 0x03);
		scratch[3] = 0;
		garmin_send_packet(Pid_FMI, scratch, 4);
		gflags &= ~GF_EXPIRE_COHORT;
	}
}

// Once per second handler
void garmin_timer(void)
{
	if (pvt_timer) pvt_timer--;
	else RaiseEvent(EVENT_GARMIN);
	if (fmi_timer) fmi_timer--;
	else RaiseEvent(EVENT_GARMIN);
}

// Process a single received byte - called from SCI ISR
unsigned char garmin_rx_byte(unsigned char s)
{
	static unsigned char esc, remaining;
	static char *p;
	// Keep bytes queued if we're busy processing
	if (state == STATE_PROCESS)
	{
		return 1;
	}
	
	// Handle DLE stuffing (DLEs in payload get doubled on transmission)
	if (state != STATE_DLE_WAIT && state != STATE_END_DLE)
	{
		if (s == 0x10 && !esc)
		{
			esc = 1;
			return 0;
		}
	}
	if (esc)
	{
		// Check for framing error
		if (s != 0x10)
		{
			state = STATE_PID;
			calc_checksum = 0;
		}
	}
	esc = 0;
	
	// Packet checksum - start at zero and subtract each byte after DLE
	calc_checksum -= s;

	// Link level protocol state machine	
	switch (state)
	{
	case STATE_DLE_WAIT:
		// Waiting for start of packet
		if (s == 0x10) state = STATE_PID;
		// Use pid to count Control-C for escape
/*		if (s == 3) pid++;
		else pid = 0;
		if (pid > 3)
		{
			garmin_quit();
		}*/
		calc_checksum = 0;
		esc = 0;
		break;
	case STATE_PID:
		// Receive Packet ID - if it's a DLE, stay here
		pid = s;
		if (s != 0x10) state = STATE_LENGTH;
		break;
	case STATE_LENGTH:
		// Receive paload length
		length = remaining = s;
		p = payload;
		// TODO: limit rx size to buffer size
		if (!length) state = STATE_CHECKSUM;
		else state = STATE_PAYLOAD;
		if (length > GARMIN_BUFFER_SIZE) length = GARMIN_BUFFER_SIZE;
		break;
	case STATE_PAYLOAD:
		// Receiving payload - add to buffer, decrement remaining counter
		if (p < (payload + GARMIN_BUFFER_SIZE))
		{
			*p = s;
			p++;
		}
		*p = 0; // Null-terminate
		remaining--;
		if (!remaining) state = STATE_CHECKSUM;
		break;
	case STATE_CHECKSUM:
		// Verify that the checksum matches what we got
		if (calc_checksum) pid = 0;
		
		state = STATE_END_DLE;
		break;
	case STATE_END_DLE:
		// Next byte should be ending DLE
		if (s == 0x10) state = STATE_ETX;
		break;
	case STATE_ETX:
		// Last byte is ETX
		if (s == 0x03)
		{
			state = STATE_DLE_WAIT;
			// Exit if we didn't get a valid checksum
			if (!pid) break;
			// Ignore ACK and NAK bytes
			if (pid != Pid_Ack_Byte && pid != Pid_Nak_Byte)
			{
				if (pid != Pid_Pvt_Data)
				{
					// Queue ACK for transmission in main loop (excluding PVT)
					pending_ack = pid;
				}
				// Main loop calls message processor
				RaiseEvent(EVENT_GARMIN);
				state = STATE_PROCESS;
			}
		}
		break;
	case STATE_PROCESS:
		// Waiting for main loop to process incoming data
		break;
	default:
		// We shouldn't be here!  Back to start
		state = STATE_DLE_WAIT;
		break;
	}
	return 0;
}

// Process received packet
void process_garmin(unsigned char pid, char *payload, unsigned char length)
{

	D800_Pvt_Data_Type *D800;
	float Vx, Vy;
	signed int itemp;
	unsigned long utemp;
	unsigned char c, i;
	Protocol_Data_Type *pdt;
	union {
		unsigned char c[4];
		float f;
	} fc;

	switch (pid)
	{
	case Pid_Ack_Byte:
		// Got ACK, do nothing
		break;

	case Pid_Nak_Byte:
		// Got NAK, should resend
		break;

	case Pid_Command_Data:
		// GPS sent us a command
		if (*payload == Cmnd_Turn_Off_Pwr)
		{
			// Got power off command - reset tracker
			system_reset();
		}
/*
		if (*payload == Cmnd_Transfer_Wpt && flags.fram_installed)
		{
			// Send stored waypoints
			garmin_send_packet(Pid_Records, "\x01\x00", 2);
			garmin_send_packet(Pid_Wpt_Data, wpt_buffer, len);
			garmin_send_packet(Pid_Xfer_Cmplt, "\x07\x00", 2);
		}*/
		break;

	case Pid_Xfer_Cmplt:
		// GPS is done sending stuff
		break;

	case Pid_Protocol_Array:
		// Process protocol capabilities array
		pdt = (Protocol_Data_Type *)payload;
		wpt_protocol = WPT_UNSUPPORTED;
		for (; length >= 3; length -= 3, pdt++)
		{
			if (pdt->tag == 'D')
			{
				switch (pdt->data)
				{
				case 0x6700:
					wpt_protocol = WPT_D103;
					break;
				case 0x6800:
					wpt_protocol = WPT_D104;
					break;
				case 0x6b00:
					wpt_protocol = WPT_D107;
					break;
				case 0x6c00:
					wpt_protocol = WPT_D108;
					break;
				case 0x6d00:
					wpt_protocol = WPT_D109;
					break;
				case 0x6e00:
					wpt_protocol = WPT_D110;
					break;
				case 0x5f02:
					wpt_protocol = WPT_D607;
					break;				
				}
			}
		}
		break;

	case Pid_Pvt_Data:
		// Position / Velocity / Time packet (sent automatically when enabled)
		if (length != 64)
		{
			// Only known PVT format is 64 bytes
			signal_invalid_gps_fix;
			break;
		}
		// Reverse byte order of payload (Garmin is little-endian)
		for (i = 0; i < (length >> 1); i++)
		{
			c = payload[i];
			payload[i] = payload[length-i-1];
			payload[length-i-1] = c;
		}
		
		// Clear Garmin re-init timer only if we already know waypoint type
		if (wpt_protocol != WPT_UNKNOWN) pvt_timer = PVT_TIMEOUT;
		D800 = (D800_Pvt_Data_Type *)payload;
		if (D800->fix < 2)
		{
			signal_invalid_gps_fix;
			break;			
		}

		// Convert lat/lon from radians to semicircles
		fixdata.latitude = D800->lat * ssradian;
		fixdata.longitude = D800->lon * ssradian;
		semicircledecode(fixdata.latitude, fixdata.longitude, fixdata.aprs_position, &(fixdata.aprs_position[8]), 
		   &(fixdata.aprs_position[7]), &(fixdata.aprs_position[16]));
		
		// Velocity is in meters/second north, east and up
		Vy = D800->north;
		Vx = D800->east;
		reset_watchdog;
		// Save 'up' velocity as rate-of-climb
		fc.f = D800->up;
		fixdata.neg_climb = 0;
		if (fc.f < 0)
		{
			fixdata.neg_climb = 1;
			fc.f = 0-fc.f;
		}
		fixdata.climb = fc.f * 196.850394; // Meters/sec to feet/min
		
		// Get heading from arctangent velocity components
		if (Vy == 0.0 && Vx == 0.0) itemp = 0;
		else itemp = atan2deg(Vy, Vx);
		fixdata.velocity = (long)itemp << 15;
		fixdata.aprs_course[0] = '0' + itemp / 100;
		fixdata.aprs_course[1] = '0' + (itemp % 100) / 10;
		fixdata.aprs_course[2] = '0' + (itemp % 10);
		
		// Get speed from square root of sum of squares, convert to knots
		fc.f = sqrt(Vx*Vx + Vy*Vy);
		reset_watchdog;
		itemp = fc.f * 1.94384449;
		fixdata.aprs_speed[0] = '0' + itemp / 100;
		fixdata.aprs_speed[1] = '0' + (itemp % 100) / 10;
		fixdata.aprs_speed[2] = '0' + (itemp % 10);
		fixdata.velocity |= (long)(fc.f * 50) & 0x7fff; // In OT units
		
		// Get altitude
		fc.f = D800->alt + D800->msl_hght;
		fixdata.neg_altitude = 0;
		if (fc.f < 0)
		{
			fixdata.neg_altitude = 1;
			fc.f = 0-fc.f;
		}
		fixdata.altitude = (unsigned long)(fc.f * 100);
		alt_feet(fixdata.altitude, fixdata.aprs_altitude);
		
		// Get time of day
		utemp = (unsigned long) D800->tow - D800->leap_scnds;
		if (utemp & 0xff000000) utemp = 0; // Will be off for a bit at midnight once a week
		itemp = gflags;
		gflags = (gflags & 0xfc) | ((utemp >> 12) & 0x03);			// Todo: Make shift configurable to select time scale
		if (gflags != itemp)													// Time block changed? Flag a delete event
		{
			gflags |= GF_EXPIRE_COHORT;
			RaiseEvent(EVENT_GARMIN);
		}
		utemp %= 86400;
		itemp = utemp / 3600;
		fixdata.aprs_ddhhmmss[2] = '0' + (itemp / 10);
		fixdata.aprs_ddhhmmss[3] = '0' + (itemp % 10);
		itemp = (utemp % 3600) / 60;
		fixdata.aprs_ddhhmmss[4] = '0' + (itemp / 10);
		fixdata.aprs_ddhhmmss[5] = '0' + (itemp % 10);
		itemp = utemp % 60;
		fixdata.aprs_ddhhmmss[6] = '0' + (itemp / 10);
		fixdata.aprs_ddhhmmss[7] = '0' + (itemp % 10);
		
		// Save estimated position error
		fixdata.hdop = (unsigned char)D800->eph;
		fixdata.satsinview = GPS_EPE_FLAG;
		
		// Set validity flags
		fixdata.position_valid = 1;
		fixdata.time_valid = 1;
		fixdata.velocity_valid = 1;
		fixdata.quality_valid = 1;
		fixdata.altitude_valid = 1;
		RaiseEvent(EVENT_POSITION);
		break;

	case Pid_Wpt_Data:
		// Received a waypoint
		break;
		
	case Pid_Product_Rqst:
		// We've been asked for identification (presumably from a PC host)
		garmin_send_packet(Pid_Product_Data, "\x01\xad\x01\x00Tracker2", 13);
		// TODO: Report correct waypoint type
		garmin_send_packet(Pid_Protocol_Array, (char *)protocol_array, sizeof(protocol_array));
		wpt_protocol = WPT_D109;
		break;
		
	case Pid_FMI:
		fmi_timer = FMI_TIMEOUT;
		gflags |= GF_FMI_ON;
		// Fleet management interface packet (probably a text message)
		if (payload[0] == 0x24 && payload[1] == 0)
		{
			fmi_msg_id = (*(unsigned long *)&payload[6]);
			// '--' indicates a command message
			if (payload[10] == '-' && payload[11] == '-')
			{
				*cmdbuf = 0;
				process_command(&payload[12], flags.current_config, 0);
				garmin_ack_fmi_message();
				// If response is returned in cmdbuf, send it back as a message
				if (*cmdbuf)
				{
					garmin_send_open_text("", cmdbuf);
					*cmdbuf = 0;
				}
			}
			// Text message from client - make sure we're not already sending one
			else if (msg.retries == 0)
			{
				// '-' indicates address prefix
				if (payload[10] == '-')
				{
					c = 11;
					// Scan for end of destination and convert to upper case
					do
					{
						if (payload[c] == 0) break;
						payload[c] = toupper(payload[c]);
						c++;
					} while (payload[c] != ' ');
					if (payload[c] != 0)
					{
						cmd_send(&payload[11], CMD_SEND, 0);
//						exec_command(CMD_SEND, 0, &payload[11]);
					}				
				}
				else
				{
					cmd_send(&payload[10], CMD_REPLY, 0);
//					exec_command(CMD_SEND, 0, &payload[10]);
				}
				// Messages are only ACKed to client when they've been sent (or timed out)
			}
		}
		
		if (payload[0] == 0x01 && payload[1] == 0x02)
		{
			// ACK an ETA packet
			garmin_send_packet(Pid_FMI, &payload[2], 4);
		}
		break;

	default:
		// Unsupported packet type
		break;
	}		
}

// Send a Garmin protocol packet with the specified PID and payload.
// Garmin says even empty packets should have a 2-byte payload.
void garmin_send_packet(unsigned char pid, char *payload, unsigned char length)
{
	unsigned char checksum;
	garmin_outc(0x10); // DLE
	garmin_outc(pid);
	garmin_outc(length);
	checksum = 0-(signed char)pid-length;
	for (; length; length--, payload++)
	{
		garmin_outc(*payload);
		// Double DLEs in payload
		if (*payload == 0x10) garmin_outc(0x10);
		checksum -= *payload;
	}
	garmin_outc(checksum);
	garmin_outc(0x10); // DLE
	garmin_outc(0x03); // ETX
}

// Send FMI text message
void garmin_send_open_text(char *msgfrom, char *msgtext)
{
	state = STATE_DLE_WAIT;
	payload[0] = 0x21;
	payload[1] = 0x00;
	payload[2] = 0x00;
	payload[3] = 0x00;
	payload[4] = 0x00;
	payload[5] = 0x00;
	sprintf(&payload[6], "%s: %s", msgfrom, msgtext);
	garmin_send_packet(Pid_FMI, payload, sizeof(payload));
}

// Send ack for FMI text message
void garmin_ack_fmi_message(void)
{
	state = STATE_DLE_WAIT;
	payload[0] = 0x25;
	payload[1] = 0x00;
	(*(unsigned long *)&payload[2]) = fmi_msg_id;
	garmin_send_packet(Pid_FMI, payload, 6);
}