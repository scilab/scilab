/*
 *
 *  Copyright (c) 2004-2005 Mike Erickson
 *                           Warren Jasper <wjasper@tx.ncsu.edu>
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

/*
 * your kernel needs to be configured with /dev/usb/hiddev support
 * I think most distros are already
 * mikee@signalscape.com
 *
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
#include "usb-1608FS.h"

/* Test Program */
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

  int fd[7];
  int flag;
  signed short svalue;
  __u8 channel, gain;
  int temp, i;
  int ch;
  int nInterfaces = 0;
  __u8 gainArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  signed short in_data[1024];
  int count;
  int options;
  float freq;
  char serial[9];

  nInterfaces = PMD_Find(MCC_VID, USB1608FS_PID, fd);

  if ( nInterfaces <= 0 ) {
    fprintf(stderr, "USB 1608FS not found.\n");
    exit(1);
  } else
    printf("USB 1608FS Device is found! Number of Interfaces = %d\n",
	   nInterfaces);

  /* config mask 0x01 means all inputs */
  usbDConfigPort_USB1608FS(fd[0], DIO_DIR_OUT);
  usbDOut_USB1608FS(fd[0], 0);

  while(1) {
    printf("\nUSB 1608FS Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink \n");
    printf("Hit 'c' to test counter\n");
    printf("Hit 'e' to exit\n");
    printf("Hit 'g' to get serial number\n");
    printf("Hit 'I' to test analog input scan\n");    
    printf("Hit 'i' to test analog input\n");
    printf("Hit 'r' to reset\n");
    printf("Hit 's' to get status\n");

    while((ch = getchar()) == '\0' ||
      ch == '\n');

    switch(ch) {
      case 'b': /* test to see if led blinks */
        usbBlink_USB1608FS(fd[0]);
        break;
      case 'c':
        printf("connect pin 38 and 21\n");
        usbInitCounter_USB1608FS(fd[0]);
        sleep(1);
        flag = fcntl(fileno(stdin), F_GETFL);
        fcntl(0, F_SETFL, flag | O_NONBLOCK);
        do {
          usbDOut_USB1608FS(fd[0], 1);
	  usleep(200000);
          usbDOut_USB1608FS(fd[0], 0);
	  printf("Counter = %d\n",usbReadCounter_USB1608FS(fd[0]));
        } while (!isalpha(getchar()));
        fcntl(fileno(stdin), F_SETFL, flag);
        break;
      case 'g':
        usbReadCode_USB1608FS(fd[0], 0x200000, 8, serial);
        serial[8] = '\0';
        printf("Serial Number = %s\n", serial);
        break;
      case 'I':
        printf("Enter desired frequency [Hz]: ");
        scanf("%f", &freq);
        printf("Enter number of samples [1-1024]: ");
        scanf("%d", &count);
	printf("\t\t1. +/- 10.V\n");
        printf("\t\t2. +/- 5.V\n");
        printf("\t\t3. +/- 2.5V\n");
        printf("\t\t4. +/- 2.V\n");
        printf("\t\t5. +/- 1.25V\n");
        printf("\t\t6. +/- 1.0V\n");
        printf("\t\t7. +/- 0.625V\n");
        printf("\t\t8. +/- 0.3125V\n");
        printf("Select gain: [1-8]\n");
        scanf("%d", &temp);
        switch(temp) {
          case 1: gain = BP_10_00V;
            break;
          case 2: gain = BP_5_00V;
            break;
          case 3: gain = BP_2_50V;
            break;
          case 4: gain = BP_2_00V;
            break;
          case 5: gain = BP_1_25V;
            break;
          case 6: gain = BP_1_00V;
            break;
          case 7: gain = BP_0_625V;
            break;
          case 8: gain = BP_0_3125V;
            break;
          default:
            break;
	}
	// Load the gain queue
	gainArray[0] = gain;
	usbAInLoadQueue_USB1608FS(fd[0], gainArray);

	// configure options
	//options = AIN_EXECUTION | AIN_DEBUG_MODE;
	options = AIN_EXECUTION ;
	for ( i = 0; i < 1024; i++ ) {  // load data with known value
	  in_data[i] = 0xbeef;
	}
        usbAInScan_USB1608FS(fd, 0, 0, count, &freq, options, in_data);
	printf("Actual frequency = %f\n", freq);
	for ( i = 0; i < count; i++ ) {
	  printf("data[%d] = %#hx  %.2fV\n", i, in_data[i], volts_USB1608FS(gain, in_data[i]));
	}
	break;
      case 'i':
        printf("Connect pin 1 - pin 23\n");
        printf("Select channel [0-7]: ");
        scanf("%d", &temp);
        if ( temp < 0 || temp > 3 ) break;
        channel = (__u8) temp;
        printf("\t\t1. +/- 10.V\n");
        printf("\t\t2. +/- 5.V\n");
        printf("\t\t3. +/- 2.5V\n");
        printf("\t\t4. +/- 2.V\n");
        printf("\t\t5. +/- 1.25V\n");
        printf("\t\t6. +/- 1.0V\n");
        printf("\t\t7. +/- 0.625V\n");
        printf("\t\t8. +/- 0.3125V\n");
        printf("Select gain: [1-8]\n");
        scanf("%d", &temp);
        switch(temp) {
          case 1: gain = BP_10_00V;
            break;
          case 2: gain = BP_5_00V;
            break;
          case 3: gain = BP_2_50V;
            break;
          case 4: gain = BP_2_00V;
            break;
          case 5: gain = BP_1_25V;
            break;
          case 6: gain = BP_1_00V;
            break;
          case 7: gain = BP_0_625V;
            break;
          case 8: gain = BP_0_3125V;
            break;
          default:
            break;
	}
        flag = fcntl(fileno(stdin), F_GETFL);
        fcntl(0, F_SETFL, flag | O_NONBLOCK);
        do {
          usbDOut_USB1608FS(fd[0], 0);
	  sleep(1);
	  svalue = usbAIn_USB1608FS(fd[0], channel, gain);
	  printf("Channel: %d: value = %#hx, %.2fV\n",
		 channel, svalue, volts_USB1608FS(gain, svalue));
          usbDOut_USB1608FS(fd[0], 0x2);
	  sleep(1);
	  svalue = usbAIn_USB1608FS(fd[0], channel, gain);
	  printf("Channel: %d: value = %#hx, %.2fV\n",
		 channel, svalue, volts_USB1608FS(gain, svalue));
	} while (!isalpha(getchar()));
	fcntl(fileno(stdin), F_SETFL, flag);
	break;
      case 's':
        printf("Status = %#x\n", usbGetStatus_USB1608FS(fd[0]));
	break;
      case 'r':
        usbReset_USB1608FS(fd[0]);
        return 0;
	break;
      case 'e':
        close(fd[0]);
	close(fd[1]);
	close(fd[2]);
	close(fd[3]);
	close(fd[4]);
        close(fd[5]);
        close(fd[6]);
        return 0;
	break;
      default:
        break;
    }
  }
}
