#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/time.h>


#define SERIAL_FILE "testfile"
//#define SERIAL_FILE "/Users/mtippett/byte-reader/data"

int main(void){

	int infile;
	unsigned char buffer[1024];
	int count;
    	struct timeval t1, t2;
	int elapsedTime;
	int index;
	int protocount=0;

	
	gettimeofday(&t1, NULL);

        printf("Opening %s\n", SERIAL_FILE);
	infile = open(SERIAL_FILE,O_RDONLY);

	setvbuf(stdout, (char*)NULL, _IONBF, 0);


	while(1) {
		count = read(infile,&buffer,1023);
		if (count > 0) {

			gettimeofday(&t2, NULL);

    			elapsedTime = (int) ((t2.tv_sec - t1.tv_sec) * 1000.0);      // sec to ms
			elapsedTime += (int) ((t2.tv_usec - t1.tv_usec) / 1000.0);   // us to ms

			t1.tv_sec = t2.tv_sec;
			t1.tv_usec = t2.tv_usec;

			if(elapsedTime > 5) {
				if (elapsedTime <1000)  {
					printf("] [%d]\n",protocount);
				        protocount=0;	
//					printf("<%2.2d>\n", elapsedTime);
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
	 	  usleep(1);
		}
	}
        return(0);
}
