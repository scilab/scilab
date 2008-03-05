/*
 *
 *  Copyright (c) 2004-2005  Warren Jasper <wjasper@tx.ncsu.edu>
 *                           Measurement Computing
 *                          
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <asm/types.h>

#include "pmd.h"
#include "usb-temp.h"

float volts_FS(const int gain, const signed short num);


/* Test Program */
float celsius2fahr( float celsius)
{
  return (celsius*9.0/5.0 + 32.);
}

float fahr2celsius( float fahr)
{
  return (fahr - 32.)*5.0/9.0;
}

int toContinue()
{
  int answer;
  answer = 0; //answer = getchar();
  printf("Continue [yY]? ");
  while((answer = getchar()) == '\0' ||
    answer == '\n');
  return ( answer == 'y' || answer == 'Y');
}

int main (int argc, char **argv) {

  int fd[1];
  int ch;
  int numInterfaces = 0;
  __u8 bIReg, bOReg;
  float temperature;
  float temperature_array[8];
  int nchan;
  int flag;
  char type;
  int i;

  numInterfaces = PMD_Find(MCC_VID, USBTEMP_PID, fd);

  if ( numInterfaces <= 0 ) {
    fprintf(stderr, "USB-TEMP not found.\n");
    exit(1);
  } else
    printf("USB TEMP Device is found! Number of interfaces = %d\n",
	   numInterfaces);

  /* config mask 0x01 means all inputs */
  usbDConfigPort_USBTEMP(fd[0], DIO_DIR_OUT);
  usbDOut_USBTEMP(fd[0], 0);

  while(1) {
    printf("\nUSB-TEMP Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink LED\n");
    printf("Hit 'c' to calibrate\n");
    printf("Hit 'd' to test DIO\n");
    printf("Hit 'e' to exit\n");
    printf("Hit 'f' for burnout status\n");
    printf("Hit 'r' to reset\n");
    printf("Hit 'p' read the CJC\n");
    printf("Hit 's' to get status\n");
    printf("Hit 't' to measure temperature (Thermocouple)\n");
    printf("Hit 'x' to measure temperature (Thermocouple) multiple channels\n");
    while((ch = getchar()) == '\0' ||
      ch == '\n');

    switch(ch) {
      case 'b': /* test to see if led blinks */
        usbBlink_USBTEMP(fd[0]);
        break;
      case 'c': /* calibration */
        usbCalibrate_USBTEMP(fd[0]);
        break;
      case 'f':  /* Get status of thermocouple burnout detection */
	 printf("Burnout status = %#x\n", usbGetBurnoutStatus_USBTEMP(fd[0], 0xf));
	 break;
      case 'p':  /* read the CJC */
        usbTin_USBTEMP(fd[0], CJC0, 0, &temperature);
	printf("CJC 0 = %.2f degress Celsius or %.2f degrees Fahrenheit.\n", temperature,
	        celsius2fahr(temperature));
        usbTin_USBTEMP(fd[0], CJC1, 0, &temperature);
	printf("CJC 1 = %.2f degress Celsius or %.2f degrees Fahrenheit.\n", temperature,
	        celsius2fahr(temperature));
        break;
      case 'd': 
        printf("conect DIO0 - DIO4\n");
	printf("conect DIO1 - DIO5\n");
	printf("conect DIO2 - DIO6\n");
	printf("conect DIO3 - DIO7\n");
	usbDConfigBit_USBTEMP(fd[0], 0,  DIO_DIR_OUT);
	usbDConfigBit_USBTEMP(fd[0], 1,  DIO_DIR_OUT);
	usbDConfigBit_USBTEMP(fd[0], 2,  DIO_DIR_OUT);
	usbDConfigBit_USBTEMP(fd[0], 3,  DIO_DIR_OUT);
	usbDConfigBit_USBTEMP(fd[0], 4,  DIO_DIR_IN);
	usbDConfigBit_USBTEMP(fd[0], 5,  DIO_DIR_IN);
	usbDConfigBit_USBTEMP(fd[0], 6,  DIO_DIR_IN);
	usbDConfigBit_USBTEMP(fd[0], 7,  DIO_DIR_IN);
	do {
  	  printf("Enter value [0-f]: ");
	  scanf("%hx", &bIReg);
	  bIReg &= 0xf;
  	  usbDOut_USBTEMP(fd[0], bIReg);
	  usbDIn_USBTEMP(fd[0], &bOReg);
	  printf("value = %#x\n", bOReg);
	} while (toContinue());
	break;
      case 's':
        printf("Status = %#x\n", usbGetStatus_USBTEMP(fd[0]));
	break;
      case 'r':
        usbReset_USBTEMP(fd[0]);
        return 0;
	break;
      case 'e':
        close(fd[0]);
        return 0;
	break;
    case 't':
        printf("Select Channel [0-7]: ");
        scanf("%d", &i);
        usbSetItem_USBTEMP(fd[0], i/2, SENSOR_TYPE, THERMOCOUPLE);
        usbSetItem_USBTEMP(fd[0], i/2, EXCITATION, 0x0);
        printf("Connect thermocouple to channel %d\n", i);
	printf("Select Thermocouple Type [JKSRBETN]: ");
	scanf("%s", &type);
	switch(type) {
	case 'J':
	  bIReg = TYPE_J;
	  printf("Type J Thermocouple Selected: \n");
	  break;
	case 'K':
	  bIReg = TYPE_K;
  	  printf("Type K Thermocouple Selected: \n");
	  break;
	case 'T':
	  bIReg = TYPE_T;
    	  printf("Type T Thermocouple Selected: \n");
	  break;
	case 'E':
	  bIReg = TYPE_E;
    	  printf("Type E Thermocouple Selected: \n");
	  break;
	case 'R':
	  bIReg = TYPE_R;
      	  printf("Type R Thermocouple Selected: \n");
	  break;
	case 'S':
	  bIReg = TYPE_S;
       	  printf("Type S Thermocouple Selected: \n");
	  break;
	case 'B':
	  bIReg = TYPE_B;
       	  printf("Type B Thermocouple Selected: \n");
	  break;
	case 'N':
	  bIReg = TYPE_N;
       	  printf("Type N Thermocouple Selected: \n");
	  break;
        default:
	  printf("Unknown or unsupported thermocopule type.\n");
	  break;
	}
        usbSetItem_USBTEMP(fd[0], i/2, i%2+CH_0_TC, bIReg);
        flag = fcntl(fileno(stdin), F_GETFL);
        fcntl(0, F_SETFL, flag | O_NONBLOCK);
	do {
          usbTin_USBTEMP(fd[0], i, 0, &temperature);
  	  printf("Channel: %d  %.2f degress Celsius or %.2f degrees Fahrenheit.\n",
		 i, temperature, celsius2fahr(temperature));
  	  sleep(1);
	} while (!isalpha(getchar()));
	fcntl(fileno(stdin), F_SETFL, flag);
	break;
    case 'x':
      	printf("Enter number of Channels (1-8): ");
	scanf("%d", &nchan);
	for ( i = 0; i < nchan; i++ ) {
          usbSetItem_USBTEMP(fd[0], i/2, SENSOR_TYPE, THERMOCOUPLE);
          usbSetItem_USBTEMP(fd[0], i/2, EXCITATION, 0x0);

          printf("Connect thermocouple to channel %d\n", i);
	  printf(" Select Thermocouple Type [JKSRBETN]: ");
	  scanf("%s", &type);
	  switch(type) {
	  case 'J':
	    bIReg = TYPE_J;
	    printf("Type J Thermocouple Selected: \n");
	    break;
	  case 'K':
	    bIReg = TYPE_K;
  	    printf("Type K Thermocouple Selected: \n");
	    break;
	  case 'T':
	    bIReg = TYPE_T;
    	    printf("Type T Thermocouple Selected: \n");
	    break;
	  case 'E':
	    bIReg = TYPE_E;
    	    printf("Type E Thermocouple Selected: \n");
	    break;
	  case 'R':
	    bIReg = TYPE_R;
      	    printf("Type R Thermocouple Selected: \n");
	    break;
	  case 'S':
	    bIReg = TYPE_S;
       	    printf("Type S Thermocouple Selected: \n");
	    break;
	  case 'B':
	    bIReg = TYPE_B;
       	    printf("Type B Thermocouple Selected: \n");
	    break;
	  case 'N':
	    bIReg = TYPE_N;
       	    printf("Type N Thermocouple Selected: \n");
	    break;
          default:
	    printf("Unknown or unsupported thermocopule type.\n");
	    break;
	  }
          usbSetItem_USBTEMP(fd[0], i/2, i%2+CH_0_TC, bIReg);
	}
        flag = fcntl(fileno(stdin), F_GETFL);
        fcntl(0, F_SETFL, flag | O_NONBLOCK);
	do {
          usbTinScan_USBTEMP(fd[0], CH0, nchan-1, 0, temperature_array);
	  for ( i = 0; i < nchan; i++ ) {
  	    printf("Channel %d:  %.2f degress Celsius or %.2f degrees Fahrenheit.\n",
		   i, temperature_array[i], celsius2fahr(temperature_array[i]));
	  }
	  printf("\n");
	  sleep(1);
	} while (!isalpha(getchar()));
	fcntl(fileno(stdin), F_SETFL, flag);
	break;
      default:
        break;
    }
  }
}
