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
#include "python2.7/Python.h"

//void initProcess() {
int main(){
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

    //create threads
	pthread_t processThread, receiveThread;
	sem_init(&keySem, 1, 1);
	sem_init(&cntSem, 1, 0);
	buffer.writeIndex = 0;
	buffer.readIndex = 0;
    pthread_create(&receiveThread, NULL, receive, NULL);
	pthread_create(&processThread, NULL, processPacket, NULL);

	pthread_join(processThread, NULL);
	pthread_join(receiveThread, NULL);

	return 0;
}

/*reader thread
This function gets implemented by the processThread
It calls bufferRead and then processes the data read by the bufferRead function based and determines packet values
The function pends/waits on the cntSem semaphore. Basically this function will execute but wait for the cntSem semaphore to signal (this happens in bufferWrite function)
*/
void *processPacket(){
	struct msg dmsg;
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
            printf("pheader serial: %" PRId16 "\n", pheader.serial);

            //parse message to find the header tick data and copy this data into the header variable
            copy16bits(temp_uint16, &dmsg, 4);
            sscanf(temp_uint16, "%" SCNx16, &pheader.tick);
            printf("pheader tick: %" PRId16 "\n", pheader.tick);

            //parse message to find the header type data and copy this data into the header variable
            copy8bits(temp_uint8, &dmsg, 8);
            sscanf(temp_uint8, "%" SCNx8, &pheader.type);
            printf("pheader.type: %" PRId8 "\n", pheader.type);

            //check the header type against known header types to determine which packet was received
            //this allows us to know how to process the packet as different packet types have different structures
            if(pheader.type == 5) {//check if the received packet is the GPS Location packet (type 0x05)
                //compare received packet time stamp (header tick value) with last GPSLocation packet timestamp
                //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
                //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
                //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped GPS location packets
                if(pheader.tick > pGPSLoc.header.tick) {
                    pGPSLoc.header = pheader;

                    copy8bits(temp_uint8, &dmsg, 10);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.flags);
                    printf("pGPSLoc.flags: %" PRId8 "\n", pGPSLoc.flags);

                    copy16bits(temp_uint16, &dmsg, 12);
                    sscanf(temp_uint16, "%" SCNx16, &pGPSLoc.altitude_low);
                    printf("pGPSLoc.altitude_low: %" PRId16 "\n", pGPSLoc.altitude_low);

                    copy32bits(temp_uint32, &dmsg, 16);
                    sscanf(temp_uint32, "%" SCNx32, &pGPSLoc.latitude);
                    printf("pGPSLoc.latitude: %" PRId32 "\n", pGPSLoc.latitude);

                    copy32bits(temp_uint32, &dmsg, 24);
                    sscanf(temp_uint32, "%" SCNx32, &pGPSLoc.longitude);
                    printf("pGPSLoc.longitude: %" PRId32 "\n", pGPSLoc.longitude);

                    copy8bits(temp_uint8, &dmsg, 32);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.year);
                    printf("pGPSLoc.year: %" PRId8 "\n", pGPSLoc.year);

                    copy8bits(temp_uint8, &dmsg, 34);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.month);
                    printf("pGPSLoc.month: %" PRId8 "\n", pGPSLoc.month);

                    copy8bits(temp_uint8, &dmsg, 36);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.day);
                    printf("pGPSLoc.day: %" PRId8 "\n", pGPSLoc.day);

                    copy8bits(temp_uint8, &dmsg, 38);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.hour);
                    printf("pGPSLoc.hour: %" PRId8 "\n", pGPSLoc.hour);

                    copy8bits(temp_uint8, &dmsg, 40);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.minute);
                    printf("pGPSLoc.minute: %" PRId8 "\n", pGPSLoc.minute);

                    copy8bits(temp_uint8, &dmsg, 42);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.second);
                    printf("pGPSLoc.second: %" PRId8 "\n", pGPSLoc.second);

                    copy8bits(temp_uint8, &dmsg, 44);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.pdop);
                    printf("pGPSLoc.pdop: %" PRId8 "\n", pGPSLoc.pdop);

                    copy8bits(temp_uint8, &dmsg, 46);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.hdop);
                    printf("pGPSLoc.hdop: %" PRId8 "\n", pGPSLoc.hdop);

                    copy8bits(temp_uint8, &dmsg, 48);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.vdop);
                    printf("pGPSLoc.vdop: %" PRId8 "\n", pGPSLoc.vdop);

                    copy8bits(temp_uint8, &dmsg, 50);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.mode);
                    printf("pGPSLoc.mode: %" PRId8 "\n", pGPSLoc.mode);

                    copy16bits(temp_uint16, &dmsg, 52);
                    sscanf(temp_uint16, "%" SCNx16, &pGPSLoc.ground_speed);
                    printf("pGPSLoc.ground_speed: %" PRId16 "\n", pGPSLoc.ground_speed);

                    copy16bits(temp_uint16, &dmsg, 56);
                    sscanf(temp_uint16, "%" SCNx16, &pGPSLoc.climb_rate);
                    printf("pGPSLoc.climb_rate: %" PRId16 "\n", pGPSLoc.climb_rate);

                    copy8bits(temp_uint8, &dmsg, 60);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.course);
                    printf("pGPSLoc.course: %" PRId8 "\n", pGPSLoc.course);

                    copy8bits(temp_uint8, &dmsg, 62);
                    sscanf(temp_uint8, "%" SCNx8, &pGPSLoc.altitude_high);
                    printf("pGPSLoc.altitude_high: %" PRId8 "\n", pGPSLoc.altitude_high);
                    }
                    total_received++;
                }

            //check if the received packet is the sensor packet (type 0x0A)
            else if(pheader.type == 10){
                //compare received packet time stamp (header tick value) with last snesor packet timestamp
                //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
                //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
                //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped sensor packets
                printf("packet is type 0x0A \n");
                if(pheader.tick > psensor.header.tick) {
                    psensor.header = pheader;

                    copy8bits(temp_uint8, &dmsg, 10);
                    sscanf(temp_uint8, "%" SCNx8, &psensor.state);
                    printf("psensor.temp: %" PRId8 "\n", psensor.state);

                    if(psensor.state == 0) {
                        boost_received++;
                     }
                     else if(psensor.state == 1) {
                        fast_received++;
                     }
                     else if(psensor.state == 2) {
                        coast_received++;
                     }
                     else if(psensor.state == 3) {
                        drouge_received++;
                     }
                     else if(psensor.state == 4) {
                        main_received++;
                     }
                     else if(psensor.state == 5) {
                        landed_received++;
                     }

                    copy16bits(temp_uint16, &dmsg, 12);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.accelmeter);
                    printf("psensor.accelmeter: %" PRId16 "\n", psensor.accelmeter);

                    copy32bits(temp_uint32, &dmsg, 16);
                    sscanf(temp_uint32, "%" SCNx32, &psensor.pres);
                    printf("psensor.pres: %" PRId32 "\n", psensor.pres);


                    copy16bits(temp_uint16, &dmsg, 24);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.temp);
                    printf("psensor.temp: %" PRId16 "\n", psensor.temp);

                    copy16bits(temp_uint16, &dmsg, 28);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.acceleration);
                    printf("psensor.acceleration: %" PRId16 "\n", psensor.acceleration);

                    copy16bits(temp_uint16, &dmsg, 32);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.speed);
                    printf("psensor.speed: %" PRId16 "\n", psensor.speed);

                    copy16bits(temp_uint16, &dmsg, 36);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.height);
                    printf("psensor.height: %" PRId16 "\n", psensor.height);

                    copy16bits(temp_uint16, &dmsg, 40);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.v_batt);
                    printf("psensor.v_batt: %" PRId16 "\n", psensor.v_batt);

                    copy16bits(temp_uint16, &dmsg, 44);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.sense_d);
                    printf("psensor.sense_d: %" PRId16 "\n", psensor.sense_d);

                    copy16bits(temp_uint16, &dmsg, 48);
                    sscanf(temp_uint16, "%" SCNx16, &psensor.sense_m);
                    printf("psensor.sense_m: %" PRId16 "\n", psensor.sense_m);

                    tempIndex = 52;
                    for(int i = 0; i < 6; i++) {
                        copy8bits(temp_uint8, &dmsg, tempIndex);
                        sscanf(temp_uint8, "%" SCNx8, &psensor.pad[i]);
                        printf("psensor.pad[%d]: %" PRId8 "\n", i, psensor.pad[i]);
                        tempIndex+=2;
                        }
                    }
                    else{
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
                    total_received++;

                    printf("boost r: %d \n", boost_received);
                    printf("fast r: %d \n", fast_received);
                    printf("coast r: %d \n", coast_received);
                    printf("drouge r: %d \n", drouge_received);
                    printf("main r: %d \n", main_received);
                    printf("landed r: %d \n", landed_received);
                    printf("total r: %d \n", total_received);
                    printf("boost d: %d \n", boost_dropped);
                    printf("fast d: %d \n", fast_dropped);
                    printf("coast d: %d \n", coast_dropped);
                    printf("drouge d: %d \n", drogue_dropped);
                    printf("main d: %d \n", main_dropped);
                    printf("landed d: %d \n", landed_dropped);
                }

            //check if the received packet is the calibration packet (type 0x0B)
            else if(pheader.type == 11){
            //compare received packet time stamp (header tick value) with last calibration packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped calibration packets
                int tempIndex2 = 36; //needed for the struct variables that are arrays
                printf("packet is type 0x0B \n");
                if(pheader.tick > pcal.header.tick) {
                        pcal.header = pheader;
                        tempIndex = 10;
                        for(int i = 0; i < 14; i++) {
                            if(i < 3) {
                                copy8bits(temp_uint8, &dmsg, tempIndex);
                                sscanf(temp_uint8, "%" SCNx8, &pcal.startpad[i]);
                                printf("pcal.startpad[%d]: %" PRId8 "\n", i, pcal.startpad[i]);
                                tempIndex+=2;
                            }
                            copy8bits(temp_uint8, &dmsg, tempIndex2);
                            sscanf(temp_uint8, "%" SCNx8, &pcal.endpad[i]);
                            printf("pcal.endpad[%d]: %" PRId8 "\n", i, pcal.endpad[i]);
                            tempIndex2+=2;
                        }

                        copy32bits(temp_uint32, &dmsg, 16);
                        sscanf(temp_uint32, "%" SCNx32, &pcal.ground_pres);
                        printf("pcal.ground_pres: %" PRId32 "\n", pcal.ground_pres);

                        copy16bits(temp_uint16, &dmsg, 24);
                        sscanf(temp_uint16, "%" SCNx16, &pcal.ground_accel);
                        printf("pcal.ground_accel: %" PRId16 "\n", pcal.ground_accel);

                        copy16bits(temp_uint16, &dmsg, 28);
                        sscanf(temp_uint16, "%" SCNx16, &pcal.accel_plus_g);
                        printf("pcal.accel_plus_g: %" PRId16 "\n", pcal.accel_plus_g);

                        copy16bits(temp_uint16, &dmsg, 32);
                        sscanf(temp_uint16, "%" SCNx16, &pcal.accel_minus_g);
                        printf("pcal.accel_minus_g: %" PRId16 "\n", pcal.accel_minus_g);

                    }
                total_received++;
            }

            //check if the received packet is the configuration packet (type 0x04)
            else if(pheader.type == 4){
            //compare received packet time stamp (header tick value) with last configuration packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped configuration packets
                printf("packet is type 0x04 \n");
                if(pheader.tick > pconfig.header.tick) {
                        pconfig.header = pheader;

                        copy8bits(temp_uint8, &dmsg, 10);
                        sscanf(temp_uint8, "%" SCNx8, &pconfig.device);
                        printf("pconfig.device: %" PRId8 "\n", pconfig.device);

                        copy16bits(temp_uint16, &dmsg, 12);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.flight);
                        printf("pconfig.flight: %" PRId16 "\n", pconfig.flight);

                        copy8bits(temp_uint8, &dmsg, 16);
                        sscanf(temp_uint8, "%" SCNx8, &pconfig.config_major);
                        printf("pconfig.config_major: %" PRId8 "\n", pconfig.config_major);

                        copy8bits(temp_uint8, &dmsg, 18);
                        sscanf(temp_uint8, "%" SCNx8, &pconfig.config_minor);
                        printf("pconfig.config_minor: %" PRId8 "\n", pconfig.config_minor);

                        copy16bits(temp_uint16, &dmsg, 20);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.apogee_delay);
                        printf("pconfig.apogee_delay: %" PRId16 "\n", pconfig.apogee_delay);

                        copy16bits(temp_uint16, &dmsg, 24);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.main_deploy);
                        printf("pconfig.main_deploy: %" PRId16 "\n", pconfig.main_deploy);

                        copy16bits(temp_uint16, &dmsg, 28);
                        sscanf(temp_uint16, "%" SCNx16, &pconfig.flight_log_max);
                        printf("pconfig.flight_log_max: %" PRId16 "\n", pconfig.flight_log_max);

                        tempIndex = 32;
                        for(int i = 0; i < 16; i++) {
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            if(i < 8){
                                sscanf(temp_uint8, "%" SCNx8, &pconfig.callsign[i]);
                                printf("pconfig.callsign[%d]: %" SCNx8 "\n", i, pconfig.callsign[i]);
                                }
                            else if (i >= 8){
                                sscanf(temp_uint8, "%" SCNx8, &pconfig.version[i-8]);
                                printf("pconfig.version[%d]: %" SCNx8 "\n", i-8, pconfig.version[i-8]);
                                }
                            tempIndex+=2;
                            }
                    }
                total_received++;
            }

            //check if the received packet is the GPS satellite data packet (type 0x06)
            else if(pheader.type == 6){
            //compare received packet time stamp (header tick value) with last GPS Satellite packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped GPS satellite packets
                printf("packet is type 0x06 \n");
                //check time stamp for last GPSSatData_packet
                if(pheader.tick > pGPSSatData.header.tick) {
                        pGPSSatData.header = pheader;

                        copy8bits(temp_uint8, &dmsg, 10);
                        sscanf(temp_uint8, "%" SCNx8, &pGPSSatData.channels);
                        printf("pGPSSatData.channels: %" PRId8 "\n", pGPSSatData.channels);

                        tempIndex = 12;
                        for(int i = 0; i < 12; i++) {
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            sscanf(temp_uint8, "%" SCNx8, &psat_info.svid);

                            tempIndex+=2;
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            sscanf(temp_uint8, "%" SCNx8, &psat_info.c_n_1);

                            pGPSSatData.sats[i] = psat_info;
                            printf("pGPSSatData.sats[%d].svid: %" PRId8 "\n", i, pGPSSatData.sats[i].svid);
                            printf("pGPSSatData.sats[%d].c_n_1: %" PRId8 "\n", i, pGPSSatData.sats[i].c_n_1);
                            tempIndex+=2;
                        }

                        for(int i = 0; i < 2; i++) {
                            copy8bits(temp_uint8, &dmsg, tempIndex);
                            sscanf(temp_uint8, "%" SCNx8, &pGPSSatData.unused[i]);
                            printf("pGPSSatData.unused[%d]: %" PRId8 "\n", i, pGPSSatData.unused[i]);
                            tempIndex+=2;
                            }
                    }
                total_received++;
            }

            //check if the received packet is the companion packet (type 0x07)
            else if(pheader.type == 7){
            //compare received packet time stamp (header tick value) with last companion packet timestamp
            //if the received packet has been received in the correct order then the time stamp will be greater than the last received packet's time stamp
            //and processing will continue by parsing the rest of the packet and assigning the data to the respected variable
            //if this is not the case then the packet will be dropped and the counter will increase to keep track of the number of dropped companion packets
                printf("packet is type 0x07 \n");
                if(pheader.tick > pcompanion.header.tick) {
                        pcompanion.header = pheader;

                        copy8bits(temp_uint8, &dmsg, 10);
                        sscanf(temp_uint8, "%" SCNx8, &pcompanion.board_id);
                        printf("pcompanion.board_id: %" PRId8 "\n", pcompanion.board_id);

                        copy8bits(temp_uint8, &dmsg, 12);
                        sscanf(temp_uint8, "%" SCNx8, &pcompanion.update_period);
                        printf("pcompanion.update_period: %" PRId8 "\n", pcompanion.update_period);

                        copy8bits(temp_uint8, &dmsg, 14);
                        sscanf(temp_uint8, "%" SCNx8, &pcompanion.channels);
                        printf("pcompanion.channels: %" PRId8 "\n", pcompanion.channels);

                        tempIndex = 16;
                        for(int i = 0; i < 12; i++) {
                            copy16bits(temp_uint16, &dmsg, tempIndex);
                            sscanf(temp_uint16, "%" SCNx16, &pcompanion.companion_data[i]);
                            printf("pcompanion.companion_data[%d]: %" PRId16 "\n", i, pcompanion.companion_data[i]);
                            tempIndex+=4;
                            }
                    }
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
    struct msg newmsg;
	//char txt[] = "4f01080b05765e00701f1a1bbeb8d7b60b070605140c00060000000000000000"; //packet 0x05
	//char txt1[] = "4f01f80b0592ef116a341a1dbcb847b3ab08070614bc0a051aa1683d11111111"; //packet 0x05 with greater timestamp
	//char txt2[] = "4f01080b0592ef116a341a1dbcb847b3ab08070614bc0a051aa1683d11111111"; //packet 0x05 with lesser timestamp

	char txt[] =  "4f01010b0a005e00701f1a1bbeb8d7b60b070605140c00060000000000000000"; //packet 0x0A
	char txt1[] = "4f01020b0a01ef116a341a1dbcb847b3ab08070614bc0a051aa1683d12345670"; //packet 0x0A with greater timestamp
	char txt2[] = "4f01030b0a02ef116a341a1dbcb847b3ab08070614bc0a051aa1683d12345670"; //packet 0x0A with lesser timestamp
	char txt3[] = "4f01040b0a03ef116a341a1dbcb847b3ab08070614bc0a051aa1683d12345670"; //packet 0x0A with lesser timestamp
	char txt4[] = "4f01050b0a04ef116a341a1dbcb847b3ab08070614bc0a051aa1683d12345670"; //packet 0x0A with lesser timestamp
	char txt5[] = "4f01060b0a05ef116a341a1dbcb847b3ab08070614bc0a051aa1683d12345670"; //packet 0x0A with lesser timestamp

	//char txt[] = "4f01080b0bb584ee26ac29fb4e23ddb62a010704142c0326060f00a0b0ac2306"; //packet 0x0B
	//char txt1[] = "4f01f80b0b72ef61aa34ea8dac482763a9a80746149c0a156aa9653db8aa5f21"; //packet 0x0B with greater timestamp
	//char txt2[] = "4f01080b0b72ef61aa34ea8dac482763a9a80746149c0a156aa9653db8aa5f21"; //packet 0x0B with lesser timestamp

	//char txt[] = "4f01080b07b583ea26cc35faae232db72a0fa744841c7326969f90aabfac2306"; //packet 0x07
	//char txt1[] = "4f01f80b0772ef713a3aec8dac4e2723a9a8674624922a156aa9653d285a5f21"; //packet 0x07 with greater timestamp
	//char txt2[] = "4f01080b0772ef61aa34ea8dac482763a9a80746149c0a156aa9653db8aa5f21"; //packet 0x07 with lesser timestamp

	//char txt[] =  "4f01080b04b583ea26cd33f3ae4325b77a2fa44464ac7a2a9a9fa0aebfac2366"; //packet 0x04
	//char txt1[] = "4f01f80b0455e4733f3ae182acae27a3c9ac6746c4c22a156aa9653d285a5fcc"; //packet 0x04 with greater timestamp
	//char txt2[] = "4f01080b0472ef61aa34ea8dac482763a9a80746149c0a156aa9653db8aa5f21"; //packet 0x04 with lesser timestamp

	//char txt[] =  "4f01080b06b583ea26c03533aec324b79a2fa24b64ac7a3a1a9fb73ebf8c0316"; //packet 0x06
	//char txt1[] = "4f01f80b0636e2733a32e782a3a829a9c9ac6726c5c82a1566a9653d280a50c2"; //packet 0x06 with greater timestamp
	//char txt2[] = "4f01080b0672ef61aa34ea8dac482763a9a80746149c0a156aa9653db8aa5f21"; //packet 0x06 with lesser timestamp


	int counter = 0;
	for(;;){
        sleep(2);
        if(counter == 0) {
        memcpy(newmsg.text, txt, 64);
        }
        else if(counter == 1) {
        memcpy(newmsg.text, txt1, 64);
        }
        else if(counter == 2) {
        memcpy(newmsg.text, txt2, 64);
        }
        else if(counter == 3) {
        memcpy(newmsg.text, txt3, 64);
        }
        else if(counter == 4) {
        memcpy(newmsg.text, txt4, 64);
        }
        else if(counter == 5) {
        memcpy(newmsg.text, txt5, 64);
        }
        else {
        memcpy(newmsg.text, txt, 64);
        }
        bufferWrite(&newmsg, &buffer);
        counter++;
		}
	return(NULL);
}

/*
This function writes the message passed in the parameter (message) to the buffer (buff)
buff is a shared resource between processThread and receiveThread and therefore must be protected
*/
uint8_t bufferWrite(struct msg *message, struct buffer *buff) {
	sem_wait(&keySem); //wait on keySem semaphore. This is needed to synchronize access to buff
    (*buff).msgBuffer[buff->writeIndex] = *message; //write message to the buffer at index specified by writeIndex
	printf("Write data: %s \n", (*buff).msgBuffer[buff->writeIndex].text);
	buff->writeIndex++; //move to the next buffer location
	//if the writeIndex is greater than the buffer size, set the writeIndex to the start of the buffer
	if(buff->writeIndex > 20) {
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
buff is a shared resource between processThread and receiveThread and therefore must be protected
*/
void bufferRead(struct msg *message, struct buffer *buff) {
	sem_wait(&keySem); //wait on keySem semaphore. This is needed to synchronize access to buff
    *message = (*buff).msgBuffer[buff->readIndex]; //read the buffer value at location specified by readIndex, and put the value read into message
	printf("read data: %s \n", (*buff).msgBuffer[buff->readIndex].text);
	buff->readIndex++;
	//if the readIndex is greater than the buffer size, set the readIndex to the start of the buffer
	if(buff->readIndex > 20){
		buff->readIndex = 0;
	}
	sem_post(&keySem); //release access to buff
}

/*
This function takes a buffer (temp_uint8) and copies 2 bites of the message (dmsg), starting at the specified index (i)
This function is needed because the message is a string and needs to be parsed before it can be converted to uint8_t type
*/
void copy8bits(char *temp_uint8, struct msg *dmsg, int i) {
    temp_uint8[0] = dmsg->text[i];
    temp_uint8[1] = dmsg->text[i+1];
    temp_uint8[2] = '\0'; //this is nessisary otherwise the array size will grow for no reason
}

/*
This function takes a buffer (temp_uint16) and copies 4 bites of the message (dmsg), starting at the specified index (i)
This function is needed because the message is a string and needs to be parsed before it can be converted to uint16_t type
*/
void copy16bits(char *temp_uint16, struct msg *dmsg, int i) {
    temp_uint16[0] = dmsg->text[i];
    temp_uint16[1] = dmsg->text[i+1];
    temp_uint16[2] = dmsg->text[i+2];
    temp_uint16[3] = dmsg->text[i+3];
    temp_uint16[4] = '\0'; //this is nessisary otherwise the array size will grow for no reason
}

/*
This function takes a buffer (temp_uint32) and copies 8 bites of the message (dmsg), starting at the specified index (i)
This function is needed because the message is a string and needs to be parsed before it can be converted to uint32_t type
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
    temp_uint32[8] ='\0'; //this is nessisary otherwise the array size will grow for no reason
}
