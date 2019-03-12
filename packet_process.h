#pragma once
#include <stdio.h>
#include <stdint.h>
#include "msg_queue.h"
#include <semaphore.h>
#include <stdbool.h>

//variables to keep track of packet statistics (how many packets of each type were dropped and how many in total were received for each type)
int boost_dropped, fast_dropped, coast_dropped, drogue_dropped, main_dropped, landed_dropped,
    boost_received, fast_received, coast_received, drouge_received, main_received, landed_received, total_received;

//packet variables to save received packet information to
struct packet_header pheader;
struct sat_info_t psat_info;
struct sensor_packet psensor;
struct calibration_packet pcal;
struct configuration_packet pconfig;
struct GPSLocation_packet pGPSLoc;
struct GPSSatData_packet pGPSSatData;
struct companion_packet pcompanion;
//shared buffer where messages (packets) are read from and written to
struct buffer buffer;

//functions
void *processPacket();
//void initProcess();
void *receive();
uint8_t bufferWrite(struct msg *message, struct buffer *buff);
void bufferRead(struct msg *message, struct buffer *buff);
void copy8bits(char *temp_uint8, struct msg *dmsg, int i);
void copy16bits(char *temp_uint16, struct msg *dmsg, int i);
void copy32bits(char *temp_uint32, struct msg *dmsg, int i);
