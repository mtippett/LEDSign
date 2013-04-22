//
//  main.c
//  LEDSign
//
//  Created by Tippett, Matthew on 4/18/13.
//  Copyright (c) 2013 Tippett, Matthew. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>



#include "sign_proto.h"

int main(void) {
    int testfile = -1;
    char message = "abcdef";
    char nullstring = { 0x00, 0x00, 0x00, 0x00 };
    
    printf("Opening file\n");
    testfile = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (testfile == -1) {
		printf("Open Failed\n");
		exit(1);
	}

    printf("handle is %d\n", testfile);

    printf("write byte\n");
    write(testfile, nullstring ,1);
    printf("read byte\n");
    read(testfile, message, 1);
    printf("close\n");
    close(testfile);
    printf("\n");

}
