#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //int8_t
#include <math.h>
#include <unistd.h> //sleep
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> // serial ports


/* baudrate settings are defined in <asm/termbits.h>, which is included by <termios.h> */
#define BAUDRATE 115200         
/* change this definition for the correct port */
#define DEVICEPORT "/dev/ttyACM0"


/* Function declaration */
int mes_dtw(int (*drg)[3], int (*drg1)[3],int n, int m, int dim);
int distanceBetween(int p1, int p2);
int fimin(int x,int y);
void change_gesture(char gnum);


int main(int argc, char **argv) 
{
	char command[128];
	char response[2560];
	char start[3];
	char dataRequest[7];
	int cFlag = 0;
	int r, c;
	int start_recording =0;
	int rms_array[200][3];
	int edit_mode = 0;
	int edit_gesture = 0;

	uint8_t x=0, y=0, z=0, u=0;
	unsigned int x_int = 0, y_int=0, z_int=0;
	int8_t filter_x=0, filter_y=0, filter_z=0;

/*command for access point communication*/
	start[0] = 0xFF;
	start[1] = 0x07;
	start[2] = 0x03; 

	dataRequest[0] = 0xFF;
	dataRequest[1] = 0x08;
	dataRequest[2] = 0x07; 
	dataRequest[3] = 0x00;
	dataRequest[4] = 0x00; 	
	dataRequest[5] = 0x00;
	dataRequest[6] = 0x00; 
	
	// open port for watch RF Communication
	struct termios  config;
	int wFile;
	wFile = open(DEVICEPORT, O_RDWR | O_NOCTTY | O_NDELAY);
	if(wFile == -1)
	{
		printf( "failed to open port\n" );
		return -1;
	}


	bzero(&config, sizeof(config));
	config.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	config.c_oflag = 0;
	tcflush(wFile, TCIFLUSH);
	tcsetattr(wFile,TCSANOW,&config);

	write(wFile,start,sizeof(start));
	printf("Sending Start\n");

	sleep(5);

	int d = 0;
	int sampleCounter = 0;
	int reqCounter = 0;
	int minus_counter=0;
	int plus_counter=0;
	
while(1){	//infinite loop
d = 0;
sampleCounter = 0;
reqCounter = 0;
minus_counter=0;
plus_counter=0;
	//usleep(25000); //assuming sampling rate is 33Hz (This will request sample at a rate of 40Hz)
	sleep(1);
	strcpy(response,"");

	//printf("Sending Req\n");
	//data request sequence: 0xFF 0x08 0x07 0x00 0x00 0x00 0x00;
	write(wFile,dataRequest,sizeof(dataRequest));
	//printf("\nRequest sent!\n");
	
	read(wFile,response,255);		
        u = response[3];
	x = response[4];
	y = response[5];
	z = response[6];
	

	x_int = (unsigned int) x;
	y_int = (unsigned int) y;
	z_int = (unsigned int) z;

	if(u == 1){
		printf("RES: %u %u %u\n", x, y, z, x_int, y_int, z_int);
			
	}
		
        
}  // end of outer while loop      
  	close(wFile);
	
	return 0;
}  // end of main
