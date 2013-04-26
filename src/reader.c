#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/time.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>


extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

int main(int argc, char **argv){

	int infile;
	unsigned char buffer[1024];
	int count;
    	struct timeval t1, t2;
	int elapsedTime;
	int index;
	int protocount=0;
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

	gettimeofday(&t1, NULL);

        printf("Opening %s\n", device_name);
	infile = open(device_name,O_RDONLY);

	setvbuf(stdout, (char*)NULL, _IONBF, 0);


	while(1) {
		count = read(infile,&buffer,1023);
		if (count > 0) {

			gettimeofday(&t2, NULL);

    			elapsedTime = (int) ((t2.tv_sec - t1.tv_sec) * 1000.0);      // sec to ms
			elapsedTime += (int) ((t2.tv_usec - t1.tv_usec) / 1000.0);   // us to ms

			t1.tv_sec = t2.tv_sec;
			t1.tv_usec = t2.tv_usec;

			if(elapsedTime > 2) {
				if (elapsedTime <1000)  {
					printf("] [%d]\n",protocount);
				        protocount=0;	
					printf("<%2.2d>\n", elapsedTime);
//					printf ("[",count);
					printf ("[");
				} else {
					printf("] [%d]\n",protocount);
				        protocount=0;	
					printf("\n=======================\n");
					printf("[");
				}
			}


			for (index = 0; index < count; index++) {
				protocount++;
				printf ("%2.2x",(unsigned char) buffer[index]);
			}
		} else {
	 	  usleep(500);
		}
	}
        return(0);
}
