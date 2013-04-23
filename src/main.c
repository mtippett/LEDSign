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
#include <getopt.h>
#include <stdlib.h>


extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;



#include "sign_proto.h"

int main(int argc, char **argv) {
    int testfile;
    struct termios term;
    char *device_name = NULL;
    

    int c;
    int digit_optind = 0;

    while (1) {
               int this_option_optind = optind ? optind : 1;
               int option_index = 0;
               static struct option long_options[] = {
                   {"device", required_argument, NULL, 'd'},
                   {0, 0, 0, 0}
               };

               c = getopt_long(argc, argv, "d:0:1:2:3:4:5:6:",
                        long_options, &option_index);
               if (c == -1)
                   break;

               switch (c) {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
                   if (digit_optind != 0 && digit_optind != this_option_optind)
                     printf("digits occur in two different argv-elements.\n");
                   digit_optind = this_option_optind;
                   printf("option %c\n", c);
                   break;

               case 'd':
                   printf("option device with option %s\n", optarg);
	           device_name = malloc(strlen(optarg));
		   strcpy(device_name, optarg);
                   break;

               case '?':
                   break;

               default:
                   printf("?? getopt returned character code 0%o ??\n", c);
               }
           }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }


    if(device_name == NULL) {
	printf("Device name must be specified with -d or --device\n"); 
	exit(-1);
    }
    printf("Opening file %s\n", device_name);
    testfile = open(device_name, O_RDWR | O_NOCTTY | O_CREAT| O_NONBLOCK|O_APPEND);
    
    if(strstr(device_name, "/dev/") == device_name) {
    	printf("Assuming tty semantics\n");
    	tcgetattr(testfile, &term);
	cfsetspeed(&term,B38400);
	tcsetattr(testfile,TCSANOW,&term);
    }
    
    printf("Attention\n");

    sign_proto_attention(testfile);
    usleep(ATTENTION_DELAY);

    printf("Write buffer\n");
  
    sign_proto_write_buffer(testfile, 0, "1", 1, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);

/*    sign_proto_write_buffer(testfile, 1, "\\CYA 2", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);

    sign_proto_write_buffer(testfile, 2, "\\CRA 3", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);

    sign_proto_write_buffer(testfile, 3, "\\CGA 4", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);

    sign_proto_write_buffer(testfile, 4, "\\CYA 5", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);

    sign_proto_write_buffer(testfile, 5, "\\CRA 6", 6, SPEED_1, ACTION_HOLD);
    usleep(COMMAND_STANDOFF);
*/

    printf("Activate\n");
    sign_proto_activate(testfile,1);
    usleep(COMMAND_STANDOFF);

    printf("Close\n");
    close(testfile);
    return 0;
    
}

