#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "sign_proto.h"
#include "proto.h"

int sign_proto_attention(int fd) {

	int retval;
	unsigned char buffer [1];

	buffer[0] = ATTENTION;

	retval = write(	fd, buffer, 1);
    return retval;
}

int sign_proto_write_buffer(int fd, int slot, char *data_buffer, int length, int speed , int action) {
	int retval;
    int index;

	unsigned char buffer [WRITE_BUFFER_LENGTH];
	unsigned char complete_buffer[WRITE_SOURCE_MAX_LENGTH];
	unsigned char *data_buffer_start;

	int slot_address;

        memset(buffer, 0,WRITE_BUFFER_LENGTH);
        memset(complete_buffer, 0,WRITE_SOURCE_MAX_LENGTH);
	memcpy(complete_buffer, data_buffer, length);
	data_buffer_start = buffer + WRITE_BUFFER_OFFSET;


	buffer[0] = COMMAND_START;
	buffer[1] = COMMAND_WRITE_BUFFER;

	slot_address = slot*SLOT_SIZE + SLOT_OFFSET;

	// First packet
	buffer[2] = (slot_address >> 8) & 0xFF;
	buffer[3] = (slot_address) & 0xFF;
       
	// Mode & Style
        buffer[4] = speed;
        buffer[6] = action;
	buffer[7] = length;

	memcpy(buffer+8, complete_buffer+ WRITE_SOURCE_1ST_OFFSET,WRITE_SOURCE_1ST_LENGTH);
	buffer[WRITE_BUFFER_CHECKSUM_OFFSET] = sign_proto_checksum(buffer,WRITE_BUFFER_LENGTH-1); 
	retval = write(	fd,buffer, WRITE_BUFFER_LENGTH);
	 
  	usleep(COMMAND_STANDOFF);

        // Second Line
	buffer[3] += 0x40;
	memcpy(data_buffer_start,complete_buffer + WRITE_SOURCE_2ND_OFFSET, WRITE_SOURCE_2ND_LENGTH);
	buffer[WRITE_BUFFER_CHECKSUM_OFFSET] = sign_proto_checksum(buffer,WRITE_BUFFER_LENGTH-1); 
	retval = write(	fd,buffer, WRITE_BUFFER_LENGTH);

  	usleep(COMMAND_STANDOFF);

        // Third Line
	buffer[3] += 0x40;
	memcpy(data_buffer_start,complete_buffer+ WRITE_SOURCE_3RD_OFFSET, WRITE_SOURCE_3RD_LENGTH);
	buffer[WRITE_BUFFER_CHECKSUM_OFFSET] = sign_proto_checksum(buffer,WRITE_BUFFER_LENGTH-1); 
	retval = write(	fd,buffer, WRITE_BUFFER_LENGTH);

  	usleep(COMMAND_STANDOFF);

        // Fourth Line (note the last 2 bytes are not copied).
	buffer[3] += 0x40;
	memcpy(data_buffer_start,complete_buffer+WRITE_SOURCE_3RD_OFFSET, WRITE_SOURCE_4TH_LENGTH);
	buffer[WRITE_BUFFER_CHECKSUM_OFFSET] = sign_proto_checksum(buffer,WRITE_BUFFER_LENGTH-1); 
	retval = write(	fd,buffer, WRITE_BUFFER_LENGTH);
	

    return retval;
}

int sign_proto_activate(int fd, int num_lines) {
	int retval;
	unsigned char buffer [3];

	buffer[0] = COMMAND_START;
	buffer[1] = COMMAND_ACTIVATE;

	// Assumed right shift fills high bits with 0
	buffer[2] = 0xff >> 8-num_lines;

	retval = write(	fd, buffer, 3);

    return retval;
}

int sign_proto_checksum(unsigned char *buffer, int length) {
	unsigned int sum = 0;
	int i;

	for(i=1; i< length; i++) {
             sum+=buffer[i];
	}

	return(sum%256);
}