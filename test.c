#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // para usleep()
#include <getopt.h>
#include <math.h>

#include "arduino-serial-lib.h"

float calculateSD(float data[]);

void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int fd = -1;
	int baudrate = 9600;  // default
	//declaracion variables
	int contador=0;	

	char *temperatura="t";
	char *humedad="h";
	
	char buffer[3];
	float artemp[12];
	float arhum[12];
	float mt;
	float mh;
	fd = serialport_init("/dev/ttyACM0", baudrate);

	if( fd==-1 )
	{
		error("couldn't open port");
		return -1;
	}
	serialport_flush(fd);

	while(1){
		
		write(fd, temperatura, 1);
		usleep(5000000);

		read(fd, buffer, 1);

		artemp[contador]=buffer[0];

		printf("Temperatura : %d\n", buffer[0]);
		
		write(fd, humedad, 1);
		usleep(5000000);
		
		read(fd, buffer, 1);
		arhum[contador]=buffer[0];
		printf("Humedad : %d\n", buffer[0]);

		if(contador==11){	
			mt=calculateSD(artemp);
			mh=calculateSD(arhum);

			printf("media temperatura : %f y su desviacion es: %.3f\n" ,mt, pow(mt,2));
			printf("media humedad: %f y su desviacion es:  %.3f\n" ,mh, pow(mh,2));
			contador=-1;
		}
		contador++;	
	}

	
	close( fd );
	return 0;	
}

/* Ejemplo para calcular desviacion estandar y media */
float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i = 0; i < 10; ++i)
    {
        sum += data[i];
    }

    mean = sum/10;

    for(i = 0; i < 10; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / 10);
}

