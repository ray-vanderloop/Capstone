#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "msg_queue.h"
#include "packet_process.h"
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <ctype.h>

//flight stage lights and radio good light GPIO pin output assignment
#define boostLED 2
#define fastLED 3
#define coastLED 25
#define drogueLED 5
#define mainLED 8
#define landedLED 9
#define radioLED 27
#define onLED 6
#define mFailLED 0
#define dFailLED 7

//void initProcess() {
int main(){
	
	if(wiringPiSetup() == -1) {
		printf("WiringPi setup failed \n");
	}
	else{
		pinMode(onLED, OUTPUT);
		pinMode(boostLED, OUTPUT);
		pinMode(fastLED, OUTPUT);
		pinMode(coastLED, OUTPUT); 
		pinMode(drogueLED, OUTPUT);
		pinMode(mainLED, OUTPUT);
		pinMode(dFailLED, OUTPUT);
		pinMode(mFailLED, OUTPUT);
		pinMode(landedLED, OUTPUT);
		pinMode(radioLED, OUTPUT);

		digitalWrite(boostLED, LOW);
		digitalWrite(radioLED, LOW);
		digitalWrite(fastLED, LOW);
		digitalWrite(coastLED, LOW);
		digitalWrite(drogueLED, LOW);
		digitalWrite(mainLED, LOW);
		digitalWrite(landedLED, LOW);
		digitalWrite(onLED, LOW);
		digitalWrite(mFailLED, LOW);
		digitalWrite(dFailLED, LOW);

		digitalWrite(onLED, HIGH); //turn on on LED 

//initiate counter variables to 0
	boost_dropped = 0;
	fast_dropped = 0;
	coast_dropped = 0;
	drogue_dropped = 0;
	main_dropped = 0;
	landed_dropped = 0;
	boost_received = 0;
	fast_received = 0;
	coast_received = 0;
	drouge_received = 0;
	main_received = 0;
	landed_received = 0;
	total_received = 0;

	//create threads and initiate semaphores
	pthread_t processThread, receiveThread;


		sem_init(&keySem, 1, 1);
		sem_init(&cntSem, 1, 0);
		buffer.writeIndex = 0;
		buffer.readIndex = 0;
    		pthread_create(&receiveThread, NULL, receive, NULL);
		pthread_create(&processThread, NULL, processPacket, NULL);

		pthread_join(processThread, NULL);
		pthread_join(receiveThread, NULL); 
		digitalWrite(boostLED, LOW);
		digitalWrite(radioLED, LOW);
		digitalWrite(fastLED, LOW);
		digitalWrite(coastLED, LOW);
		digitalWrite(drogueLED, LOW);
		digitalWrite(mainLED, LOW);
		digitalWrite(landedLED, LOW);
		digitalWrite(onLED, LOW);
		digitalWrite(mFailLED, LOW);
		digitalWrite(dFailLED, LOW);
	}
	return 0;
}

/*reader thread
This function gets implemented by the processThread
It calls bufferRead and then processes the data read by the bufferRead function based and determines packet values
The function pends/waits on the cntSem semaphore. Basically this function will execute but wait for the cntSem semaphore to signal (this happens in bufferWrite function)
*/
void *processPacket(){
    struct msg dmsg;
    int drogueState = 0;
    int mainState = 0;
    char temp_uint8[3];
    char deviceSerial[] = "4f01"; //THIS IS TEMP. CHANGE TO USER INPUT
    char temp_uint16[5];
    char temp_uint32[9];
    int tempIndex;
	for(;;) {
		sem_wait(&cntSem); //wait for cntSem to signal that something has been written into the buffer
		bufferRead(&dmsg, &buffer);

		//Compare first 4 values of message received from buffer with user entered flight computer device serial number
		//if these values are the same then we know we received the packet from the correct flight computer and packet processing begins
		if(!strncmp(dmsg.text, deviceSerial,4)){
            //parse hexadecimal message (packet - this is a char array and somewhat acts like a string) to find the header serial data
            //and copy this data into the header variable
            copy16bits(temp_uint16, &dmsg, 0);
            sscanf(temp_uint16, "%" SCNx16, &pheader.serial);
            //printf("Device serial number: %" PRId16 "\n", pheader.serial);

            //parse message to find the header tick data and copy this data into the header variable
            copy16bits(temp_uint16, &dmsg, 4);
            sscanf(temp_uint16, "%" SCNx16, &pheader.tick);
            //printf("Device clock value: %" PRId16 "\n", pheader.tick);

            //parse message to find the header type data and copy this data into the header variable
            copy8bits(temp_uint8, &dmsg, 8);
            sscanf(temp_uint8, "%" SCNx8, &pheader.type);
            //printf("Packet type: %" PRId8 "\n", pheader.type);

            //check the header type against known header types to determine which packet was received
            //this allows us to know how to process the packet as different packet types have different structures
            if(pheader.type == 5) {//check if the received packet is the GPS Location packet (type 0x05)
                //compare received packet time stamp (header tick value) with last GPSLocation packet timestamp
                //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
                //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
                //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped GPS location packets
                if(pheader.tick > pGPSLoc.header.tick) {
                    pGPSLoc.header = pheader;
printf("Device serial number: %" PRId16 "\n", pheader.serial);
		    printf("GPS Location Packet (Type 0x05) \n");
		    printf("Device Clock Value: %" PRId16 "\n", pheader.tick);
                    copy8bits(temp_uint8, &dmsg, 10);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.flags);
                    printf("GPS Flags: %" PRId8 "\n", pGPSLoc.flags);

                    copy16bits(temp_uint16, &dmsg, 12);
                    sscanf(temp_uint16, "%" SCNx16, &pGPSLoc.altitude_low);
                    printf("Low Altitude: %" PRId16 "\n", pGPSLoc.altitude_low);

                    copy32bits(temp_uint32, &dmsg, 16);
                    sscanf(temp_uint32, "%" SCNx32, &pGPSLoc.latitude);
                    printf("Latitude: %" PRId32 "\n", pGPSLoc.latitude);

                    copy32bits(temp_uint32, &dmsg, 24);
                    sscanf(temp_uint32, "%" SCNx32, &pGPSLoc.longitude);
                    printf("Longitude: %" PRId32 "\n", pGPSLoc.longitude);

                    copy8bits(temp_uint8, &dmsg, 32);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.year);
                    printf("Year: %" PRId8 "\n", pGPSLoc.year);

                    copy8bits(temp_uint8, &dmsg, 34);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.month);
                    printf("Month: %" PRId8 "\n", pGPSLoc.month);

                    copy8bits(temp_uint8, &dmsg, 36);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.day);
                    printf("Day: %" PRId8 "\n", pGPSLoc.day);

                    copy8bits(temp_uint8, &dmsg, 38);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.hour);
                    printf("Hour: %" PRId8 "\n", pGPSLoc.hour);

                    copy8bits(temp_uint8, &dmsg, 40);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.minute);
                    printf("Minute: %" PRId8 "\n", pGPSLoc.minute);

                    copy8bits(temp_uint8, &dmsg, 42);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.second);
                    printf("Second: %" PRId8 "\n", pGPSLoc.second);

                    copy8bits(temp_uint8, &dmsg, 44);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.pdop);
                    printf("Pdop: %" PRId8 "\n", pGPSLoc.pdop);

                    copy8bits(temp_uint8, &dmsg, 46);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.hdop);
                    printf("Hdop: %" PRId8 "\n", pGPSLoc.hdop);

                    copy8bits(temp_uint8, &dmsg, 48);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.vdop);
                    printf("Vdop: %" PRId8 "\n", pGPSLoc.vdop);

                    copy8bits(temp_uint8, &dmsg, 50);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.mode);
                    printf("Mode: %" PRId8 "\n", pGPSLoc.mode);

                    copy16bits(temp_uint16, &dmsg, 52);
                    sscanf(temp_uint16, "%" SCNx16, &pGPSLoc.ground_speed);
                    printf("Ground Speed: %" PRId16 "\n", pGPSLoc.ground_speed);

                    copy16bits(temp_uint16, &dmsg, 56);
                    sscanf(temp_uint16, "%" SCNx16, &pGPSLoc.climb_rate);
                    printf("Climb Rate: %" PRId16 "\n", pGPSLoc.climb_rate);

                    copy8bits(temp_uint8, &dmsg, 60);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.course);
                    printf("Course: %" PRId8 "\n", pGPSLoc.course);

                    copy8bits(temp_uint8, &dmsg, 62);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.altitude_high);
                    printf("High Altitude: %" PRId8 "\n\n", pGPSLoc.altitude_high);
                    }
else{
	printf("GPS location packet received with timestamp %" PRId16 ", last GPS location packet timestamp was %" PRId16 ". Packet has been dropped \n\n",pheader.tick, pGPSLoc.header.tick);
}
                    total_received++;
                }

            //check if the received packet is the sensor packet (type 0x0A)
            else if(pheader.type == 10){
                //compare received packet time stamp (header tick value) with last snesor packet timestamp
                //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
                //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
                //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped sensor packets
                if(pheader.tick > psensor.header.tick) {
                    psensor.header = pheader;
		    printf("Device serial number: %" PRId16 "\n", pheader.serial);
		    printf("Sensor Packet (Type 0x0A) \n");
		    printf("Device Clock Value: %" PRId16 "\n", pheader.tick);

                    copy8bits(temp_uint8, &dmsg, 10);
                    sscanf(temp_uint8, "%" SCNx8, &psensor.state);
                    printf("Flight Stage: %" PRId8 "\n", psensor.state);

                    if(psensor.state == 0) {
			if(digitalRead(boostLED) == 0) {
				digitalWrite(boostLED, HIGH); //turn on boost flight stage LED if it is off
			}
			printf("turn on boost LED \n");
			boost_received++;
                     }
                     else if(psensor.state == 1) {
			if(digitalRead(fastLED) == 0) {
              			digitalWrite(fastLED, HIGH); //turn on fast flight stage LED if its off
			}
			printf("turn on fast LED \n");
                        fast_received++;
                     }
                     else if(psensor.state == 2) {
			if(digitalRead(coastLED) == 0) {
              			digitalWrite(coastLED, HIGH); //turn on coast flight stage LED if its off
			}
			printf("turn on coast LED \n");
                        coast_received++;
                     }
                     else if(psensor.state == 3) {
			if(digitalRead(drogueLED) == 0) {
              			digitalWrite(drogueLED, HIGH); //turn on drogue flight stage LED if its off
			}
			printf("turn on drogue LED \n");
                        drouge_received++;
                     }
                     else if(psensor.state == 4) {
			if(digitalRead(mainLED) == 0) {
              			digitalWrite(mainLED, HIGH); //turn on main flight stage LED if its off
			printf("turn on main LED \n");
			if(drogueState == 0) {
				digitalWrite(dFailLED, HIGH); //turn on drouge fail light
				printf("turn on drouge failed light \n");
			}
			}
                        main_received++;
                     }
                     else if(psensor.state == 5) {
			if(digitalRead(landedLED) == 0) {
              			digitalWrite(landedLED, HIGH); //turn on landed flight stage LED if its off
			}
			printf("turn on landed LED \n");
			if(mainState == 0) {
				digitalWrite(mFailLED, HIGH); //turn on main fail light
				printf("turn on main failed light \n");
			}
                        landed_received++;
                     }

                    copy16bits(temp_uint16, &dmsg, 12);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.accelmeter);
                    printf("Accelerometer Data: %" PRId16 "\n", psensor.accelmeter);

                    copy32bits(temp_uint32, &dmsg, 16);
                    sscanf(temp_uint32, "%" SCNx32, &psensor.pres);
                    printf("Pressure Sensor Data: %" PRId32 "\n", psensor.pres);

                    copy16bits(temp_uint16, &dmsg, 24);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.temp);
                    printf("Temperature Sensor Data: %" PRId16 "\n", psensor.temp);

                    copy16bits(temp_uint16, &dmsg, 28);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.acceleration);
                    printf("Acceleration: %" PRId16 "\n", psensor.acceleration);

                    copy16bits(temp_uint16, &dmsg, 32);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.speed);
                    printf("Speed: %" PRId16 "\n", psensor.speed);

                    copy16bits(temp_uint16, &dmsg, 36);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.height);
                    printf("Height: %" PRId16 "\n", psensor.height);

                    copy16bits(temp_uint16, &dmsg, 40);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.v_batt);
                    printf("Battery Voltage: %" PRId16 "\n", psensor.v_batt);

                    copy16bits(temp_uint16, &dmsg, 44);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.sense_d);
                    printf("Drogue Continuity Sense: %" PRId16 "\n", psensor.sense_d);
		    if((psensor.state == 3) && (psensor.sense_d > 600)) {
			drogueState++;
			}

                    copy16bits(temp_uint16, &dmsg, 48);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.sense_m);
                    printf("Main Continuity Sense: %" PRId16 "\n", psensor.sense_m);
		    if((psensor.state == 4) && (psensor.sense_m > 600)) {
			mainState++;
			}

                    tempIndex = 52;
                    for(int i = 0; i < 6; i++) {
                        copy8bits(temp_uint8, &dmsg, tempIndex);
                        sscanf(temp_uint8, "%" SCNx8, &psensor.pad[i]);
			if(i == 0){
                        	printf("Padding: %" PRId8, psensor.pad[i]);
			}
			else {
				printf("%" PRId8, psensor.pad[i]);
			}		
			
                        tempIndex+=2;
                        }
			printf("\n\n");
                    }
                    else{
			printf("Sensor packet received with timestamp %" PRId16 ", last GPS location packet timestamp was %" PRId16 ". Packet has been dropped \n\n",pheader.tick, psensor.header.tick);
                        copy8bits(temp_uint8, &dmsg, 10);
                        char temp;
                        sscanf(temp_uint8, "%" SCNx8, &temp);
                        if(temp == 0) {
                            boost_dropped++;
                        }
                        else if(temp == 1) {
                            fast_dropped++;
                        }
                        else if(temp == 2) {
                            coast_dropped++;
                        }
                        else if(temp == 3) {
                            drogue_dropped++;
                        }
                        else if(temp == 4) {
                            main_dropped++;
                        }
                        else if(temp == 5) {
                            landed_dropped++;
                        }
                    }
		printf("Flight Stage Packet Statistics: \n");
		printf("Boost Flight Stage Received: %d \n", boost_received);
                printf("Fast Flight Stage Received: %d \n", fast_received);
                printf("Coast Flight Stage Received: %d \n", coast_received);
                printf("Drogue Flight Stage Received: %d \n", drouge_received);
                printf("Main Flight Stage Received: %d \n", main_received);
                printf("Landed Flight Stage Received: %d \n", landed_received);                
                /*printf("Boost Flight Stage Dropped: %d \n", boost_dropped);
                printf("Fast Flight Stage Dropped: %d \n", fast_dropped);
                printf("Coast Flight Stage Dropped: %d \n", coast_dropped);
                printf("Drogue Flight Stage Dropped: %d \n", drogue_dropped);
                printf("Main Flight Stage Dropped: %d \n", main_dropped);
                printf("Landed Flight Stage Dropped: %d \n\n", landed_dropped);*/
                total_received++;
                }

            //check if the received packet is the calibration packet (type 0x0B)
            else if(pheader.type == 11){
            //compare received packet time stamp (header tick value) with last calibration packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped calibration packets
                int tempIndex2 = 36; //needed for the struct variables that are arrays
                if(pheader.tick > pcal.header.tick) {
                        pcal.header = pheader;
			printf("Device serial number: %" PRId16 "\n", pheader.serial);
			printf("Calibration Packet (Type 0x0B) \n");
		        printf("Device Clock Value: %" PRId16 "\n", pheader.tick);

                        tempIndex = 10;
                        for(int i = 0; i < 14; i++) {
                            if(i < 3) {
                                copy8bits(temp_uint8, &dmsg, tempIndex);
                                sscanf(temp_uint8, "%" SCNx8, &pcal.startpad[i]);
                                tempIndex+=2;
                            }

                            copy8bits(temp_uint8, &dmsg, tempIndex2);
                            sscanf(temp_uint8, "%" SCNx8, &pcal.endpad[i]);
                            //printf("pcal.endpad[%d]: %" PRId8 "\n", i, pcal.endpad[i]);
                            tempIndex2+=2;
                        }

                        copy32bits(temp_uint32, &dmsg, 16);
                        sscanf(temp_uint32, "%" SCNx32, &pcal.ground_pres);
                        printf("Barometer Reading on Ground: %" PRId32 "\n", pcal.ground_pres);

                        copy16bits(temp_uint16, &dmsg, 24);
                        sscanf(temp_uint16, "%" SCNx16, &pcal.ground_accel);
                        printf("Average Ground Acceleration: %" PRId16 "\n", pcal.ground_accel);

                        copy16bits(temp_uint16, &dmsg, 28);
                        sscanf(temp_uint16, "%" SCNx16, &pcal.accel_plus_g);
                        printf("Acceleration Calibration (+G): %" PRId16 "\n", pcal.accel_plus_g);

                        copy16bits(temp_uint16, &dmsg, 32);
                        sscanf(temp_uint16, "%" SCNx16, &pcal.accel_minus_g);
                        printf("Acceleration Calibration (-G): %" PRId16 "\n", pcal.accel_minus_g);


 			for(int i = 0; i < 3; i++) {
				if(i == 0) {
					printf("Startpad: %" PRId8, pcal.startpad[i]);
				}
				else{
					printf("%" PRId8, pcal.startpad[i]);
				}
                    	}
			printf("\n");

 			for(int i = 0; i < 14; i++) {
				if(i == 0) {
					printf("Endpad: %" PRId8, pcal.endpad[i]);
				}
				else{
					printf("%" PRId8, pcal.endpad[i]);
				}
                    }
printf("\n\n");
                total_received++;
            }
else{
	printf("Calibration packet received with timestamp %" PRId16 ", last GPS location packet timestamp was %" PRId16 ". Packet has been dropped \n\n",pheader.tick, pcal.header.tick);
}
	}

            //check if the received packet is the configuration packet (type 0x04)
            else if(pheader.type == 4){
            //compare received packet time stamp (header tick value) with last configuration packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped configuration packets
                if(pheader.tick > pconfig.header.tick) {
                        pconfig.header = pheader;
printf("Device serial number: %" PRId16 "\n", pheader.serial);
			printf("Configuration Packet (Type 0x04) \n");
		        printf("Device Clock Value: %" PRId16 "\n", pheader.tick);

                        copy8bits(temp_uint8, &dmsg, 10);
                        sscanf(temp_uint8, "%" SCNx8, &pconfig.device);
                        printf("Device Type: %" PRId8 "\n", pconfig.device);

                        copy16bits(temp_uint16, &dmsg, 12);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.flight);
                        printf("Flight Number: %" PRId16 "\n", pconfig.flight);

                        copy8bits(temp_uint8, &dmsg, 16);
                        sscanf(temp_uint8, "%" SCNx8, &pconfig.config_major);
                        printf("Configuration Major Version: %" PRId8 "\n", pconfig.config_major);

                        copy8bits(temp_uint8, &dmsg, 18);
                        sscanf(temp_uint8, "%" SCNx8, &pconfig.config_minor);
                        printf("Configuration Minor Version: %" PRId8 "\n", pconfig.config_minor);

                        copy16bits(temp_uint16, &dmsg, 20);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.apogee_delay);
                        printf("Apogee Deploy Delay: %" PRId16 "\n", pconfig.apogee_delay);

                        copy16bits(temp_uint16, &dmsg, 24);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.main_deploy);
                        printf("Main Deploy Altitude: %" PRId16 "\n", pconfig.main_deploy);

                        copy16bits(temp_uint16, &dmsg, 28);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.flight_log_max);
                        printf("Maximum Flight Log Size: %" PRId16 "\n", pconfig.flight_log_max);

                        tempIndex = 32;
                        for(int i = 0; i < 16; i++) {
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            if(i < 8){
                                sscanf(temp_uint8, "%" SCNx8, &pconfig.callsign[i]);
                                //printf("pconfig.callsign[%d]: %" SCNx8 "\n", i, pconfig.callsign[i]);

                                }
                            else if (i >= 8){
                                sscanf(temp_uint8, "%" SCNx8, &pconfig.version[i-8]);
                                //printf("pconfig.version[%d]: %" SCNx8 "\n", i-8, pconfig.version[i-8]);
                                }
                            tempIndex+=2;
                            }

			for(int i = 0; i < 8; i++){
				if(i == 0){
					printf("Radio Operator Callsign: %" SCNx8, pconfig.callsign[i]);
				}
				else {
				printf("%" SCNx8, pconfig.callsign[i]);
				}
			}
			printf("\n");

			for(int i = 0; i < 8; i++){
				if(i == 0){
					printf("Software Version ID: %" SCNx8, pconfig.version[i]);
				}
				else {
					printf("%" SCNx8, pconfig.version[i]);
				}
			}
			printf("\n\n");
                    }
		else{
			printf("Configuration packet received with timestamp %" PRId16 ", last GPS location packet timestamp was %" PRId16 ". Packet has been dropped \n\n",pheader.tick, pconfig.header.tick);
		}
                total_received++;
            }

            //check if the received packet is the GPS satellite data packet (type 0x06)
            else if(pheader.type == 6){
            //compare received packet time stamp (header tick value) with last GPS Satellite packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped GPS satellite packets
                //check time stamp for last GPSSatData_packet
                if(pheader.tick > pGPSSatData.header.tick) {
                        pGPSSatData.header = pheader;
			printf("Device serial number: %" PRId16 "\n", pheader.serial);
			printf("GPS Satellite Data Packet (Type 0x06) \n");
		        printf("Device Clock Value: %" PRId16 "\n", pheader.tick);

                        copy8bits(temp_uint8, &dmsg, 10);
                        sscanf(temp_uint8, "%" SCNx8, &pGPSSatData.channels);
                        printf("Number of Reported Satellites: %" PRId8 "\n", pGPSSatData.channels);

                        tempIndex = 12;
                        for(int i = 0; i < 12; i++) {
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            sscanf(temp_uint8, "%" SCNx8, &psat_info.svid);

                            tempIndex+=2;
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            sscanf(temp_uint8, "%" SCNx8, &psat_info.c_n_1);

                            pGPSSatData.sats[i] = psat_info;
                            //printf("pGPSSatData.sats[%d].svid: %" PRId8 "\n", i, pGPSSatData.sats[i].svid);
                            //printf("pGPSSatData.sats[%d].c_n_1: %" PRId8 "\n", i, pGPSSatData.sats[i].c_n_1);
			      printf("Channel %d Satellite Number: %" PRId8 " and Signal Quality: %" PRId8 "\n", i, pGPSSatData.sats[i].svid, pGPSSatData.sats[i].c_n_1);
                            tempIndex+=2;
                        }
			printf("\n");
                        for(int i = 0; i < 2; i++) {
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            sscanf(temp_uint8, "%" SCNx8, &pGPSSatData.unused[i]);
                           // printf("pGPSSatData.unused[%d]: %" PRId8 "\n", i, pGPSSatData.unused[i]);
                            tempIndex+=2;
                            }
                    }
		else {
			printf("GPS satellite data packet received with timestamp %" PRId16 ", last GPS location packet timestamp was %" PRId16 ". Packet has been dropped \n\n",pheader.tick, pGPSSatData.header.tick);
		}
                total_received++;
            }

            //check if the received packet is the companion packet (type 0x07)
            else if(pheader.type == 7){
            //compare received packet time stamp (header tick value) with last companion packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped companion packets
                if(pheader.tick > pcompanion.header.tick) {
                        pcompanion.header = pheader;
			printf("Device serial number: %" PRId16 "\n", pheader.serial);
			printf("Companion Data Packet (Type 0x07) \n");
		        printf("Device Clock Value: %" PRId16 "\n", pheader.tick);

                        copy8bits(temp_uint8, &dmsg, 10);
                        sscanf(temp_uint8, "%" SCNx8, &pcompanion.board_id);
                        printf("Companion Board ID: %" PRId8 "\n", pcompanion.board_id);

                        copy8bits(temp_uint8, &dmsg, 12);
                        sscanf(temp_uint8, "%" SCNx8, &pcompanion.update_period);
                        printf("Packet Update Period: %" PRId8 "\n", pcompanion.update_period);

                        copy8bits(temp_uint8, &dmsg, 14);
                        sscanf(temp_uint8, "%" SCNx8, &pcompanion.channels);
                        printf("Number of Data Channels: %" PRId8 "\n", pcompanion.channels);

                        tempIndex = 16;
                        for(int i = 0; i < 12; i++) {
                            copy16bits(temp_uint16, &dmsg, tempIndex);
                            sscanf(temp_uint16, "%" SCNx16, &pcompanion.companion_data[i]);
                            printf("Channel %d Data: %" PRId16 "\n", i, pcompanion.companion_data[i]);
                            tempIndex+=4;
                            }
                    }

		else{
			printf("Companion data packet received with timestamp %" PRId16 ", last GPS location packet timestamp was %" PRId16 ". Packet has been dropped \n",pheader.tick, pcompanion.header.tick);
		}
		printf("\n");
                total_received++;
            }
        }
    }
	return(NULL);
}


/*writer thread
This function takes a message (newmsg) and passes it to bufferWrite
This function gets implemented by the receiverThread
*/
void *receive(){
	int serial = serialOpen("/dev/serial0", 9600); //9600 is the baud rate, this needs to match the baud rate of what you're communicating with
  	struct msg newmsg;
	char text[65];
	int i = 0;
	time_t lastReceivedTime, time_difference;
	struct timespec getCurrentTime;
	if(serial == -1) {
		printf("Error: Couldn't open port \n");
	}
	else {
		for(;;){
        		if((serialDataAvail(serial) > 0) || (i == 64)) {
				if(digitalRead(radioLED) == 0) { 
					digitalWrite(radioLED, HIGH); //turn on radio good LED if it was previously off
				}
				if(i < (sizeof(text) - 1)){
       					text[i] = tolower(serialGetchar(serial));
					i++;
				}
				else {
					clock_gettime(CLOCK_REALTIME, &getCurrentTime);
					lastReceivedTime = getCurrentTime.tv_sec;
					text[i] = '\0';
					memcpy(newmsg.text, text, 65);
      		  			bufferWrite(&newmsg, &buffer);
					i = 0;
        			}
			} 
			else {
				clock_gettime(CLOCK_REALTIME, &getCurrentTime);
				time_difference = getCurrentTime.tv_sec - lastReceivedTime;
				if(time_difference >= 5) {
					digitalWrite(radioLED, LOW); //turn off radio good LED as packets have not been received in the last 5 seconds
					//printf("turn radio LED off \n");
				}
				//printf("sleep \n");
				sleep(1);
			}
		} 
	} 
	serialClose(serial);
	return(NULL);
}

/*
This function writes the message passed in the parameter (message) to the buffer (buff)
buff is a shared resource between processThread and receiveThread and therefore must be protected
*/
uint8_t bufferWrite(struct msg *message, struct buffer *buff) {
	sem_wait(&keySem); //wait on keySem semaphore. This is needed to synchronize access to buff
    	(*buff).msgBuffer[buff->writeIndex] = *message; //write message to the buffer at index specified by writeIndex
	//printf("Write data: %s \n", (*buff).msgBuffer[buff->writeIndex].text);
	buff->writeIndex++; //move to the next buffer location
	//if the writeIndex is greater than the buffer size, set the writeIndex to the start of the buffer
	if(buff->writeIndex > 99) {
		buff->writeIndex = 0;
	}
	//the write and read index point to the same spot the function will return 1 to show an error. They should never be the same value.
	if(buff->writeIndex == buff->readIndex){
		return 1;
	}
	sem_post(&keySem); //release access to buff
	sem_post(&cntSem); //increase cntSem semaphore to notify processPacket() that the event took place (something was written to the buffer)
	return 0;
}

/*
This function reads from the buffer (buff) and puts the result in the message passed as the parameter (message)
buff is a shared resource between processThread and receiveThread and therefore must be protected by a semaphore
*/
void bufferRead(struct msg *message, struct buffer *buff) {
	sem_wait(&keySem); //wait on keySem semaphore. This is needed to synchronize access to the buffer.
    	*message = (*buff).msgBuffer[buff->readIndex]; //read the buffer value at location specified by readIndex, and put the value read into message
	//printf("read data[%d]: %s \n", buff->readIndex, (*buff).msgBuffer[buff->readIndex].text);
	buff->readIndex++;
	//if the readIndex is greater than the buffer size, set the readIndex to the start of the buffer
	if(buff->readIndex > 99){
		buff->readIndex = 0;
	}
	sem_post(&keySem); //release access to buff
}

/*
This function takes a buffer (temp_uint8) and copies 2 bits of the message (dmsg), starting at the specified index (i)
This function is needed because the message is a string and needs to be parsed before it can be converted to non-char type
*/
void copy8bits(char *temp_uint8, struct msg *dmsg, int i) {
    temp_uint8[0] = dmsg->text[i];
    temp_uint8[1] = dmsg->text[i+1];
    temp_uint8[2] = '\0'; //this is nessisary otherwise the array size will grow for no reason
}

/*
This function takes a buffer (temp_uint16) and copies 4 bits of the message (dmsg), starting at the specified index (i)
This function is needed because the message is a string and needs to be parsed before it can be converted to non-char type
*/
void copy16bits(char *temp_uint16, struct msg *dmsg, int i) {
    temp_uint16[0] = dmsg->text[i];
    temp_uint16[1] = dmsg->text[i+1];
    temp_uint16[2] = dmsg->text[i+2];
    temp_uint16[3] = dmsg->text[i+3];
    temp_uint16[4] = '\0'; //this is nessisary otherwise the array size will grow for no reason
}

/*
This function takes a buffer (temp_uint32) and copies 8 bits of the message (dmsg), starting at the specified index (i)
This function is needed because the message is a string and needs to be parsed before it can be converted to non-char type
*/
void copy32bits(char *temp_uint32, struct msg *dmsg, int i) {
    temp_uint32[0] = dmsg->text[i];
    temp_uint32[1] = dmsg->text[i+1];
    temp_uint32[2] = dmsg->text[i+2];
    temp_uint32[3] = dmsg->text[i+3];
    temp_uint32[4] = dmsg->text[i+4];
    temp_uint32[5] = dmsg->text[i+5];
    temp_uint32[6] = dmsg->text[i+6];
    temp_uint32[7] = dmsg->text[i+7];
    temp_uint32[8] = '\0'; //this is nessisary otherwise the array size will grow for no reason
}


