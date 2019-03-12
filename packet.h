#pragma once
#include <stdio.h>
#include <stdint.h>
//all packets are 32 bytes long - 5 byte header followed by 27 bytes of data
//more information about packet types and their fields can be found here: https://altusmetrum.org/AltOS/doc/telemetry.pdf

/*
The first 5 bytes of every packet is reserved for the header information
The data included in the packet header facilitates how to process the packets
*/
struct packet_header {
	uint16_t serial; //device serial number - packet bytes[0-1]
	uint16_t tick; //device time in 100ths of a second (when the packet was transmitted) - packet bytes[2-3]
	uint8_t type; //packet type - packet byte[4]
};

/*
The sensor data packet type (type = 0x0A) contains data from the Telemetrum's sensors.
These packets are transmitted from the Telemetrum once/second when on the ground and during the descent and landing flight stages and 10/second during the ascent flight stage.
*/
struct sensor_packet {
	struct packet_header header; //packet header - packet bytes[0-4]
	uint8_t state; //flight state - packet byte[5]:
                    //flight state Boost = 00
                    //flight state Fast = 01
                    //flight state Coast = 02
                    //flight state Drogue = 03
                    //flight state Main = 04
                    //flight state Landed = 05
	int16_t accelmeter; //accelerometer data - packet bytes[6-7]
	int32_t pres; //pressure sensor data (Pa * 10) - packet bytes[8-11]
	int16_t temp; //temperature sensor data (celcius * 100) - packet bytes[12-13]
	int16_t acceleration; //in m/s^2 * 16 - packet bytes[14-15]
	int16_t speed; //in m/s * 16 - packet bytes[16-17]
	int16_t height; //in m - packet bytes[18-19]
	int16_t v_batt; //battery voltage - packet bytes[20-21]
	int16_t sense_d; //drogue continuity sense - packet bytes[22-23]
	int16_t sense_m; //main contonuity sense - packet bytes[24-25]
	uint8_t pad[6]; //6 pad bytes - packet bytes[26-31]
};

/*
The configuration packet type (type = 0x0B) contains data the telemetrum uses to calibrate it's sensors
These packets are transmitted from the Telemetrum once/second during all flight stages.
*/
struct calibration_packet {
	struct packet_header header; //packet header - packet bytes[0-4]
	uint8_t startpad[3];    //3 pad bytes - packet bytes[5-7]
	int32_t ground_pres;   //average barometer reading (pressure) on pad/ground - packet bytes[8-11]
	int16_t ground_accel;  //average accelerometer reading on pad/ground - packet bytes[12-13]
	int16_t accel_plus_g;  //accel calibration at +1g - packet bytes[14-15]
	int16_t accel_minus_g; //accel calibration at -1g - packet bytes[16-17]
	uint8_t endpad[14];    //14 pad bytes - packet bytes[18-32]
};

/*
The configuration packet type (type = 0x04) contains information of software installed on the Telemetrum and any user specified configuration data.
These packets are transmitted from the Telemetrum once/second during all flight stages.
*/
struct configuration_packet {
	struct packet_header header; //packet header - packet bytes[0-4]
	uint8_t device;				//device type - packet byte[5]
	uint16_t flight;		    //flight number - packet bytes[6-7]
	uint8_t config_major;      //Config major version - packet byte[8]
	uint8_t config_minor;      //Config minor version - packet bytes[9]
	uint16_t apogee_delay;     //Apogee deploy delay in seconds - packet bytes[10-11]
	uint16_t main_deploy;      //Main deploy altitude in meters - packet bytes[12-13]
	uint16_t flight_log_max;    //Maximum flight log size in kB - packet bytes[14-15]
	char callsign[8];  //Radio operator identity (callsign) - packet bytes[16-23]
	char version[8];    //Software version identifier - packet bytes[24-32]
};

/*
The GPS location packet type (type = 0x05) contains information from the GPS receiver (position, time, speed, and precision estimates).
These packets are transmitted from the Telemetrum once/second during all flight stages.
*/
struct GPSLocation_packet {
	struct packet_header header; //packet header - packet bytes[0-4]
	uint8_t flags;          //GPS flags and number of satellites- packet byte[5]
							//Bits 0-3, name: nsats - number of satellites used to compute poisition (number may be lower than actual value)
							//Bit 4, name: valid - GPS solution is valid
							//Bit 5, name: running - GPS receiver is operational
							//Bit 6, name: date_valid - Reported date is valid
							//Bit 7, name: course_valid - ground speed, course, and climb rates are valid
	uint16_t altitude_low;  // GPS reported altitude (m) - packet bytes[6-7]
	int32_t latitude;       //Latitude (degrees * 10?) - packet bytes[8-11]
	int32_t longitude;      //Longitude (degrees * 10?) - packet bytes[12-15]
	uint8_t year;           //Year - 2000 - packet byte[16]
	uint8_t month;          //Month (numerical value 1-12) - packet byte[17]
	uint8_t day;            //Day (numerical value 1-31) - packet byte[18]
	uint8_t hour;           //Hour (numerical value 0-23) - packet byte[19]
	uint8_t minute;         //Minute (numerical value 0-59) - packet byte[20]
	uint8_t second;         //Second (numerical value 0-59) - packet byte[21]
	uint8_t pdop;           // (m * 5) - packet byte[22]
	uint8_t hdop;           // (m * 5) - packet byte[23]
	uint8_t vdop;           // (m * 5) - packet byte[24]
	uint8_t mode;           //GPS Mode - packet byte[25]
							//Mode types:
							//Mode: N, name: not valid - All data is invalid
							//Mode: A, name: autonomous mode - data is derived from satellite data
							//Mode: E, name: estimated - data is estimated using dead reckoning from last known data
							//Mode: M, name: manual - data was entered manually
							//Mode: S, name: simulated -GPS receiver testing mode
	uint16_t ground_speed;  //ground speed in cm/s - packet bytes[26-27]
	int16_t climb_rate;     //climb rate in cm/s - packet bytes[28-29]
	uint8_t course;         //course (degrees / 2) - packet byte[30]
	int8_t altitude_high;  /* 31 high byte of altitude */
};

/*GPS per-satellite data:*/
struct sat_info_t {
	uint8_t svid; //svid - space vehicle (satellite) identifier
	uint8_t c_n_1; //C/N1 signal quality indicator
};


/*
The GPS Satellite Data packet type (type = 0x06) contains information of satellite identifiers and signal quality.
This data is in the form of a C/N1 number for up to 12 satellites
These packets are transmitted from the Telemetrum once/second during all flight stages.
*/
struct GPSSatData_packet {
	struct packet_header header; //packet header - packet bytes[0-4]
	uint8_t channels; //Number of reported satellites - packet byte[5]
	struct sat_info_t sats[12];   //Channels  - packet bytes[6-29]
	uint8_t unused[2];  //Unused bytes  - packet bytes[30-32]
};


/*
The Companion Data packet type (type = 0x07) contains telemetry information to be included in the downlink and provides 12 16-bit data values.
These packets are transmitted only if a companion board is attached to the Telemetrum.
These packets are transmitted from the companion board specifies the transmission rate.
On the ground and during the descent flight stages this rate is limited to once per second and during teh ascent flight stage this rate is limited to 10/second.
*/
struct companion_packet {
	struct packet_header header; //packet header - packet bytes[0-4]
	uint8_t board_id;			//ID of companion board attached to Telemetrum - packet byte[5]
	uint8_t update_period;  //How often telemetry is sent (in 1/100ths of a second) - packet byte[6]
	uint8_t channels;       //Number of data channels supplied (how many values are in the companion_data array) - packet byte[7]
	uint16_t companion_data[12];      //Companion data, up to 12 channels (array size) of 16-bit data - packet bytes[8-32]
};
