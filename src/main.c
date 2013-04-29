//
//  main.c
//  LEDSign
//
//  Created by Tippett, Matthew on 4/18/13.
//  Copyright (c) 2013 Tippett, Matthew. All rights reserved.
//

#define _GNU_SOURCE 500
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <getopt.h>


extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;
extern int getdate_err;

struct tm *date = NULL;

struct tm *getdate(const char *string);



#include "sign_proto.h"

char *make_test_string(const char base, char *my_string);

int main(int argc, char **argv) {
    int testfile;
    struct termios term;
    char *device_name = NULL;
    char *my_string;
    char *messages[6];
    int message_count = 0;    

    int c;
    int digit_optind = 0;

    my_string = (char *) malloc(WRITE_SOURCE_MAX_LENGTH);


    while (1) {
               int this_option_optind = optind ? optind : 1;
               int option_index = 0;
               static struct option long_options[] = {
                   {"device", required_argument, NULL, 'd'},
                   {"time", optional_argument, NULL, 't'},
                   {0, 0, 0, 0}
               };

               c = getopt_long(argc, argv, "t:d:0:1:2:3:4:5:6:",
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
                   printf("option %c,%s\n", c, optarg);
                   break;

               case 'd':
                   printf("option device with option %s\n", optarg);
	           device_name = malloc(strlen(optarg));
		   strcpy(device_name, optarg);
                   break;

               case 't':
		   if (optarg != NULL) {
			date = getdate(optarg);
			if (date == NULL) {
				printf("Invalid date string \"%s\" code %d\n",optarg,getdate_err);
				exit(1);
			}	
		   } else {
       			time_t current_time;

	        	current_time = time(NULL);
	        	date = localtime(&current_time);
	           }

                   printf("Time f %s\n", optarg);
                   break;

               case '?':
                   break;

               default:
                   printf("?? getopt returned character code 0%o ??\n", c);
               }
           }

    if (optind < argc) {
        while (optind < argc) {
		messages[message_count] = argv[optind];
		message_count++;
		optind++;
	}
        printf("message_count %d\n", message_count);
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

    for( c= 0; c < message_count; c++ ) {
    	printf("Write buffer %d with %s\n", c, messages[c]);
        sign_proto_write_buffer(testfile, c, messages[c], strlen(messages[c]), SPEED_5, ACTION_SCROLL);
        usleep(COMMAND_STANDOFF);
	
    }

    printf("Activate\n");
    sign_proto_activate(testfile,c);
    usleep(COMMAND_STANDOFF);

    if (date != NULL) {
	printf("Set Time\n");
	sign_proto_set_time(testfile, date);
    	usleep(COMMAND_STANDOFF);

    }	

    printf("Close\n");
    close(testfile);
    return 0;
    
}

char *make_test_string(const char base, char *my_string) {
	int i;

	for(i = 0; i < 8; i++) {
//	for(i = 0; i < WRITE_SOURCE_MAX_LENGTH; i++) {
		if (!(i%10)) {
			my_string[i]=base;
		} else {
			my_string[i]='A'+i/10;
		}
	}

//	memcpy(my_string,"\\DH:\\DM:\\DS ", 12);

	return my_string;
}

