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

char *make_test_string(const char base, char *my_string);

int main(int argc, char **argv) {
    int testfile;
    struct termios term;
    char *device_name = NULL;
    char *my_string;
    

    int c;
    int digit_optind = 0;

    my_string = (char *) malloc(WRITE_SOURCE_MAX_LENGTH);


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

    printf("Write buffer 1\n");
    sign_proto_write_buffer(testfile, 0, make_test_string('a',my_string), 250, SPEED_5, ACTION_SCROLL);
    usleep(COMMAND_STANDOFF);
/*

    printf("Write buffer 2\n");
    sign_proto_write_buffer(testfile, 1, make_test_string('b',my_string), 250, SPEED_5, ACTION_SCROLL);
    usleep(COMMAND_STANDOFF);

    printf("Write buffer 3\n");
    sign_proto_write_buffer(testfile, 2, make_test_string('c',my_string), 250, SPEED_5, ACTION_SCROLL);
    usleep(COMMAND_STANDOFF);

    printf("Write buffer 4\n");
    sign_proto_write_buffer(testfile, 3, make_test_string('d',my_string), 250, SPEED_5, ACTION_SCROLL);
    usleep(COMMAND_STANDOFF);

    printf("Write buffer 5\n");
    sign_proto_write_buffer(testfile, 4, make_test_string('e',my_string), 250, SPEED_5, ACTION_SCROLL);
    usleep(COMMAND_STANDOFF);

    printf("Write buffer 6\n");
    sign_proto_write_buffer(testfile, 5, make_test_string('f',my_string), 250, SPEED_5, ACTION_SCROLL);
    usleep(COMMAND_STANDOFF);
*/

    printf("Set Time\n");
    sign_proto_set_time(testfile);
    usleep(COMMAND_STANDOFF);

    printf("Activate\n");
    sign_proto_activate(testfile,1);
    usleep(COMMAND_STANDOFF);

    printf("Close\n");
    close(testfile);
    return 0;
    
}

char *make_test_string(const char base, char *my_string) {
	int i;

	for(i = 0; i < WRITE_SOURCE_MAX_LENGTH; i++) {
		if (!(i%10)) {
			my_string[i]=base;
		} else {
			my_string[i]='0'+i/10;
		}
	}

	memcpy(my_string,"\\DH:\\DM:\\DS ", 12);

	return my_string;
}

