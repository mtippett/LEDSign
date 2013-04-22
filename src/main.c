//
//  main.c
//  LEDSign
//
//  Created by Tippett, Matthew on 4/18/13.
//  Copyright (c) 2013 Tippett, Matthew. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>



#include "sign_proto.h"

int main(void) {
    int testfile;
    struct termios term;
    
    printf("Opening file\n");
    // testfile = open("testfile", O_RDWR |O_APPEND|O_CREAT, 0066);
    testfile = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    //testfile = 1;
    
    tcgetattr(testfile, &term);
    cfsetspeed(&term,B38400);
    tcsetattr(testfile,TCSANOW,&term);
    
    printf("Attention\n");

    sign_proto_attention(testfile);
    usleep(ATTENTION_DELAY);

    printf("Write buffer\n");
  
    usleep(COMMAND_STANDOFF);
    sign_proto_write_buffer(testfile, 0, "\\CRM 1", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);
    sign_proto_write_buffer(testfile, 1, "\\CGM 2", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);
/*    sign_proto_write_buffer(testfile, 2, "MNOPQR", 6, SPEED_1, ACTION_SNOW);
    usleep(COMMAND_STANDOFF);
    sign_proto_write_buffer(testfile, 3, "STUVWX", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);
*/
    usleep(COMMAND_STANDOFF);
    usleep(COMMAND_STANDOFF);
    printf("Activate\n");
    sign_proto_activate(testfile,2);
    printf("Close\n");
    close(testfile);
    return 0;
    
}

