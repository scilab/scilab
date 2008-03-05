/*
 *
 *  Copyright (c) 2004-2005 Warren Jasper <wjasper@tx.ncsu.edu>
 *                          Mike Erickson <merickson@nc.rr.com>
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
#include "minilab-1008.h"

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
  
  int fd = -1;
  int flag;
  signed short svalue;
  int temp, i, j;
  int ch;
  int nInterfaces;
  int rate;
  __s16 sdata[1024];
  __u16 value;
  __u16 count;
  __u8 gains[8];
  __u8 options;
  __u8 input, pin = 0, channel, gain;

  nInterfaces = PMD_Find(MCC_VID, MINILAB1008_PID, &fd);

  if ( fd <= 0 ) {
    fprintf(stderr, "miniLAB 1008 not found.\n");
    exit(1);
  } else 
    printf("miniLAB 1008 Device is found! Number of Interfaces = %d\n",
	   nInterfaces);

  /* config mask 0x01 means all inputs */
  usbDConfigPort_miniLAB1008(fd, DIO_PORTB, DIO_DIR_IN);
  usbDConfigPort_miniLAB1008(fd, DIO_PORTA, DIO_DIR_OUT);
  usbDConfigPort_miniLAB1008(fd, DIO_PORTCL, DIO_DIR_IN);
  usbDConfigPort_miniLAB1008(fd, DIO_PORTCH, DIO_DIR_OUT);
  
  /* Note:  I believe the direction setting for the Auxport (DIO0-DIO3)
     is opposite than for PORTA-PORTC.  This is not documented.
  */

  usbDConfigPort_miniLAB1008(fd, DIO_AUXPORT, 0x0f); // all 4 bits to output
  usbDOut_miniLAB1008(fd, DIO_PORTA, 0x0);
  
  while(1) {
    printf("\nminiLAB 1008 Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink \n");
    printf("Hit 's' to set user id\n");
    printf("Hit 'g' to get user id\n");
    printf("Hit 'c' to test counter \n");
    printf("Hit 'd' to test digital I/O \n");
    printf("Hit 't' to test digital bit I/O\n");
    printf("Hit 'o' to test analog output\n");
    printf("Hit 'i' to test analog input\n");
    printf("Hin 'n' to test analog input scan\n");
    printf("Hit 'e' to exit\n");
    

    while((ch = getchar()) == '\0' || ch == '\n');
    
    switch(tolower(ch)) {
    case 'b': /* test to see if led blinks */
      usbBlink_miniLAB1008(fd);
      break;
    case 's':
      printf("enter a user id :");
      scanf("%d",&temp);
      usbSetID_miniLAB1008(fd, temp);
      printf("User ID is set to %d\n", usbGetID_miniLAB1008(fd));      
      break;
    case 'g':
      printf("User ID = %d\n", usbGetID_miniLAB1008(fd));      
      break;
    case 'o': /* test the analog output */
      printf("Testing the analog output...\n");
      printf("Enter channel [0-1] => (pin 13-14):");
      scanf("%d", &temp);
      channel = (__u8) temp;
      for ( j = 0; j < 5; j++ ) {
        for ( value = 0; value < 0x3ff; value++ ) {
	  usbAOut_miniLAB1008(fd, channel, value);
	}
      }
      break;
    case 'c':
      printf("connect DIO0 and CTR\n");
      usbInitCounter_miniLAB1008(fd);
      sleep(1);
      flag = fcntl(fileno(stdin), F_GETFL);
      fcntl(0, F_SETFL, flag | O_NONBLOCK);
      do {
        usbDOut_miniLAB1008(fd, DIO_AUXPORT, 0x1);
	sleep(1);
        usbDOut_miniLAB1008(fd, DIO_AUXPORT, 0x0);
	printf("Counter = %d\n",usbReadCounter_miniLAB1008(fd));
      } while (!isalpha(getchar()));
      fcntl(fileno(stdin), F_SETFL, flag);
      break;
    case 'd':
      printf("\nTesting Digital I/O....\n");
      printf("connect pins 21 through 28 <=> 32 through 39\n");
      do {
        printf("Enter a byte number [0-0xff]: " );
        scanf("%x", &temp);
        usbDOut_miniLAB1008(fd, DIO_PORTA, (__u8)temp);
        usbDIn_miniLAB1008(fd, DIO_PORTB, &input);
        usbDOut_miniLAB1008(fd, DIO_AUXPORT, (__u8)temp);
        printf("The number you entered = %#x\n",input);
      } while (toContinue());
      break;
    case 't':
      //reset the pin values
      usbDOut_miniLAB1008(fd, DIO_PORTA, 0x0);
      printf("\nTesting Bit  I/O....\n");
      printf("Enter a bit value for output (0 | 1) : ");
      scanf("%d", &temp);
      input = (__u8) temp;
      printf("Select the Pin in port A [0-7] :");
      scanf("%d", &temp);
      pin = (__u8) temp;
      usbDBitOut_miniLAB1008(fd, DIO_PORTA, pin, input);
      usbDIn_miniLAB1008(fd, DIO_PORTB, &input);
      printf("The number you entered 2^%d = %d \n",temp, input);
      break;
    case 'n':
      printf("\t\t1. +/- 20.V\n");
      printf("\t\t2. +/- 10.V\n");
      printf("\t\t3. +/- 5.V\n");
      printf("\t\t4. +/- 4.V\n");
      printf("\t\t5. +/- 2.5V\n");
      printf("\t\t6. +/- 2.0V\n");
      printf("\t\t7. +/- 1.25V\n");
      printf("\t\t8. +/- 1.0V\n");
      printf("Select gain: [1-8]\n");
      scanf("%d", &temp);
      switch(temp) {
        case 1: gains[0] = BP_20_00V;
                break;
        case 2: gains[0] = BP_10_00V;
                break;
        case 3: gains[0] = BP_5_00V;
                break;
        case 4: gains[0] = BP_4_00V;
                break;
        case 5: gains[0] = BP_2_50V;
                break;
        case 6: gains[0] = BP_2_00V;
                break;
        case 7: gains[0] = BP_1_25V;
                break;
        case 8: gains[0] = BP_1_00V;
                break;
        default:
                break;
      }
      printf("Select sampling rate [Hz]: ");
      scanf("%d", &rate);
      printf("Select number of samples: ");
      scanf("%hd", &count);
      options = AIN_EXECUTION;
      usbAInScan_miniLAB1008(fd, count, rate, 0, 0, options, sdata, gains);
      for ( i = 0; i < count; i++ ) {
	printf("sdata[%d] = %#hx, %.2fV\n", i, sdata[i], volts_LS(gain, sdata[i]));
      }
      break;
    case 'i':
      printf("Select channel [0-3]: ");
      scanf("%d", &temp);
      if ( temp < 0 || temp > 3 ) break;
      channel = (__u8) temp;
      printf("\t\t1. +/- 20.V\n");
      printf("\t\t2. +/- 10.V\n");
      printf("\t\t3. +/- 5.V\n");
      printf("\t\t4. +/- 4.V\n");
      printf("\t\t5. +/- 2.5V\n");
      printf("\t\t6. +/- 2.0V\n");
      printf("\t\t7. +/- 1.25V\n");
      printf("\t\t8. +/- 1.0V\n");
      printf("Select gain: [1-8]\n");
      scanf("%d", &temp);
      switch(temp) {
        case 1: gain = BP_20_00V;
                break;
        case 2: gain = BP_10_00V;
                break;
        case 3: gain = BP_5_00V;
                break;
        case 4: gain = BP_4_00V;
                break;
        case 5: gain = BP_2_50V;
                break;
        case 6: gain = BP_2_00V;
                break;
        case 7: gain = BP_1_25V;
                break;
        case 8: gain = BP_1_00V;
                break;
        default:
                break;
      }
      flag = fcntl(fileno(stdin), F_GETFL);
      fcntl(0, F_SETFL, flag | O_NONBLOCK);
      do {
	 sleep(1);
	 svalue = usbAIn_miniLAB1008(fd, channel, gain);
	 printf("Channel: %d: value = %#hx, %.2fV\n",
		      channel, svalue, volts_LS(gain, svalue));
      } while (!isalpha(getchar()));
      fcntl(fileno(stdin), F_SETFL, flag);
      break;
    case 'e':
      close(fd);
      return 0;
    default:
      break;
    }
  }
}
  



