#ifndef PROTO_H
#define PROTO_H

#define ATTENTION_DELAY 100000
/* Have seen standoff of 20-30 ms */
#define COMMAND_STANDOFF 10000 

/* Send ATTENTION, wait ATTENTION_DELAY */
#define ATTENTION 0x00

/* [0] Send PREAMBLE */
#define COMMAND_START 0x02

/* [1] COMMANDS */
/* WRITE ONLY COMMANDS */
#define COMMAND_WRITE_BUFFER  0x31
#define COMMAND_ACTIVATE 0x33

/* READ COMMAND */
#define COMMAND_FONT 0x39
/* eg:(02390039) */

/* [3] Offset is the next byte */
/*** NOTE : Alternate is model is confirm by replacing a certain piece data */
/*  [2:3] Address */
/* Message slots are  */
/* 0x0000-0x05FF Initialization Data (pushed with initial screen) */
/* 0x0600-0x06FA Message Slot 1 */
/* 0x0700-0x07FA Message Slot 2 */
/* 0x0800-0x08FA Message Slot 3 */
/* 0x0900-0x09FA Message Slot 4 */
/* 0x0A00-0x0AFA Message Slot 5 */
/* 0x0B00-0x0BFA Message Slot 6 */
/*                   */
/* 0x0e00-????? Extra Data */

/* Slot is */
/* <speed><00><action><length to print><data><checksum> */
#define SLOT_OFFSET 0x0600
#define SLOT_SIZE   0x0100

#define WRITE_BUFFER_OFFSET 0x04
#define WRITE_BUFFER_LENGTH 69
#define WRITE_BUFFER_CHECKSUM_OFFSET 68

#define WRITE_SOURCE_MAX_LENGTH 250

#define WRITE_SOURCE_1ST_OFFSET (0)
#define WRITE_SOURCE_2ND_OFFSET (60)
#define WRITE_SOURCE_3RD_OFFSET (60+64)
#define WRITE_SOURCE_4TH_OFFSET (60+64+64)

#define WRITE_SOURCE_1ST_LENGTH 60
#define WRITE_SOURCE_2ND_LENGTH 64
#define WRITE_SOURCE_3RD_LENGTH 64
#define WRITE_SOURCE_4TH_LENGTH 62

/* Speed */
#define SPEED_1 0x31
#define SPEED_2 0x32
#define SPEED_3 0x33
#define SPEED_4 0x34
#define SPEED_5 0x35

/* Action */
#define ACTION_HOLD   0x41
#define ACTION_SCROLL 0x42
#define ACTION_SNOW   0x43
#define ACTION_SPARKLE 0x44
#define ACTION_FLASH 0x45


#endif
