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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <asm/types.h>

#include "pmd.h"
#include "usb-1024LS.h"

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
  __u8 input, pin = 0; 
  int temp;
  int ch;
  int nInterfaces;

  nInterfaces = PMD_Find(MCC_VID, USB1024LS_PID, &fd);
  if ( nInterfaces < 0 ) {
    nInterfaces = PMD_Find(MCC_VID, USB1024HLS_PID, &fd);
  }

  if ( nInterfaces <= 0 ) {
    fprintf(stderr, "USB 1024LS and USB 1024HLS not found.\n");
    exit(1);
  } else 
    printf("USB USB 1024LS Device is found!\n");

  usbDConfigPort_USB1024LS(fd, DIO_PORTA, DIO_DIR_OUT);
  usbDConfigPort_USB1024LS(fd, DIO_PORTB, DIO_DIR_IN);
  usbDConfigPort_USB1024LS(fd, DIO_PORTC_LOW, DIO_DIR_OUT);
  usbDConfigPort_USB1024LS(fd, DIO_PORTC_HI, DIO_DIR_IN);
  
  while(1) {
    printf("\nUSB 1024 Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink \n");
    printf("Hit 's' to set user id\n");
    printf("Hit 'g' to get user id\n");
    printf("Hit 'c' to test counter \n");
    printf("Hit 'd' to test digital I/O \n");
    printf("Hit 't' to test digital bit I/O\n");
    printf("Hit 'e' to exit\n");
    

    while((ch = getchar()) == '\0' || ch == '\n');
    
    switch(ch) {
    case 'b': /* test to see if led blinks */
      usbBlink_USB1024LS(fd);
      break;
    case 's':
      printf("enter a user id :");
      scanf("%d",&temp);
      usbSetID_USB1024LS(fd, temp);
      printf("User ID is set to %d\n", usbGetID_USB1024LS(fd));      
      break;
    case 'g':
      printf("User ID = %d\n", usbGetID_USB1024LS(fd));      
      break;
    case 'c':
      printf("connect pin 21 and 20\n");
      usbInitCounter_USB1024LS(fd);
      sleep(1);
      usbDOut_USB1024LS(fd,DIO_PORTA,(__u8) 0x0);
      flag = fcntl(fileno(stdin), F_GETFL);
      fcntl(0, F_SETFL, flag | O_NONBLOCK);
      do {
        usbDOut_USB1024LS(fd,DIO_PORTA,0x1);
        usbDOut_USB1024LS(fd,DIO_PORTA,0x0);
	printf("Counter = %d\n",usbReadCounter_USB1024LS(fd));
      } while (!isalpha(getchar()));
      fcntl(fileno(stdin), F_SETFL, flag);
      break;
    case 'd':
      printf("\nTesting Digital I/O....\n");
      printf("connect pins 21 through 28 <=> 32 through 39 and pins 1-4 <==> 5-8\n");
      do {
        printf("Enter a byte number [0-0xff] : " );
        scanf("%x", &temp);
        usbDOut_USB1024LS(fd, DIO_PORTA, (__u8)temp);
        usbDOut_USB1024LS(fd, DIO_PORTA, (__u8)temp);
        input = usbDIn_USB1024LS(fd, DIO_PORTB);
        input = usbDIn_USB1024LS(fd, DIO_PORTB);
        printf("The number you entered = %#x\n\n",input);
        printf("Enter a nibble [0-0xf] : " );
        scanf("%x", &temp);
        usbDOut_USB1024LS(fd, DIO_PORTC_LOW, (__u8)temp);
	input = usbDIn_USB1024LS(fd, DIO_PORTC_HI);
        printf("The number you entered = %#x\n",input);
      } while (toContinue());
      break;
    case 't':
      //reset the pin values
      usbDOut_USB1024LS(fd,DIO_PORTA,0x0);
      printf("\nTesting Bit  I/O....\n");
      printf("Enter a bit value for output (0 | 1) : ");
      scanf("%d", &temp);
      input = (__u8) temp;
      printf("Select the Pin in port A [0-7] :");
      scanf("%d", &temp);
      pin = (__u8) temp;
      usbDBitOut_USB1024LS(fd, DIO_PORTA, pin, input);
      printf("The number you entered 2^%d = %d \n",
	     temp,usbDIn_USB1024LS(fd, DIO_PORTB));
      break;
    case 'e':
      close(fd);
      return 0;
    default:
      break;
    }
  }
}
  



