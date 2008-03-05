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
#include "usb-1096HFS.h"

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
  char serial[9];

  nInterfaces = PMD_Find(USB1096HFS_VID, USB1096HFS_PID, &fd);

  if ( nInterfaces <= 0 ) {
    fprintf(stderr, "USB 1096HFS not found.\n");
    exit(1);
  } else 
    printf("USB 1096HFS Device is found!\n");

  usbDConfigPort_USB1096HFS(fd, DIO_PORT0A, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT0B, DIO_DIR_IN);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT0C_LOW, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT0C_HI, DIO_DIR_IN);

  usbDConfigPort_USB1096HFS(fd, DIO_PORT1A, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT1B, DIO_DIR_IN);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT1C_LOW, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT1C_HI, DIO_DIR_IN);

  usbDConfigPort_USB1096HFS(fd, DIO_PORT2A, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT2B, DIO_DIR_IN);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT2C_LOW, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT2C_HI, DIO_DIR_IN);

  usbDConfigPort_USB1096HFS(fd, DIO_PORT3A, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT3B, DIO_DIR_IN);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT3C_LOW, DIO_DIR_OUT);
  usbDConfigPort_USB1096HFS(fd, DIO_PORT3C_HI, DIO_DIR_IN);

  while(1) {
    printf("\nUSB 1096HFS Testing\n");
    printf("----------------\n");
    printf("Hit 'b' to blink LED\n");
    printf("Hit 'c' to test counter \n");
    printf("Hit 'd' to test digital I/O \n");
    printf("Hit 'e' to exit\n");
    printf("Hit 'g' to get serial number\n");
    printf("Hit 's' to get status\n");
    printf("Hit 'r' to reset\n");
    printf("Hit 't' to test digital bit I/O\n");
    
    while((ch = getchar()) == '\0' || ch == '\n');
    
    switch(ch) {
    case 'b': /* test to see if led blinks */
      usbBlink_USB1096HFS(fd);
      break;
    case 'c':
      printf("connect pin P1A0 and CTR\n");
      usbInitCounter_USB1096HFS(fd);
      sleep(1);
      usbDOut_USB1096HFS(fd, DIO_PORT0A, 0x0);
      flag = fcntl(fileno(stdin), F_GETFL);
      fcntl(0, F_SETFL, flag | O_NONBLOCK);
      do {
        usbDOut_USB1096HFS(fd, DIO_PORT0A, 0x1);
        usbDOut_USB1096HFS(fd, DIO_PORT0A, 0x0);
	printf("Counter = %d\n",usbReadCounter_USB1096HFS(fd));
      } while (!isalpha(getchar()));
      fcntl(fileno(stdin), F_SETFL, flag);
      break;
    case 'd':
      printf("\nTesting Digital I/O....\n");
      printf("connect pins 21 through 28 <=> 32 through 39 and pins 1-4 <==> 5-8\n");
      do {
        printf("Enter a byte number [0-0xff] : " );
        scanf("%x", &temp);
        usbDOut_USB1096HFS(fd, DIO_PORT0A, (__u8)temp);
        input = usbDIn_USB1096HFS(fd, DIO_PORT0B);
        printf("The number you entered = %#x\n\n",input);
        printf("Enter a nibble [0-0xf] : " );
        scanf("%x", &temp);
        usbDOut_USB1096HFS(fd, DIO_PORT0C_LOW, (__u8)temp);
	input = usbDIn_USB1096HFS(fd, DIO_PORT0C_HI);
        printf("The number you entered = %#x\n",input);
      } while (toContinue());
      break;
    case 'g':
      usbReadCode_USB1096HFS(fd, 0x200000, 8, serial);
      serial[8] = '\0';
      printf("Serial Number = %s\n", serial);
      break;
    case 't':
      //reset the pin values
      usbDOut_USB1096HFS(fd,DIO_PORT0A,0x0);
      printf("\nTesting Bit  I/O....\n");
      printf("Enter a bit value for output (0 | 1) : ");
      scanf("%d", &temp);
      input = (__u8) temp;
      printf("Select the Pin in port A [0-7] :");
      scanf("%d", &temp);
      pin = (__u8) temp;
      usbDBitOut_USB1096HFS(fd, DIO_PORT0A, pin, input);
      printf("The number you entered 2^%d = %d \n",
	     temp,usbDIn_USB1096HFS(fd, DIO_PORT0B));
      break;
    case 's':
      printf("Status = %#x\n", usbGetStatus_USB1096HFS(fd));
      break;
    case 'r':
      usbReset_USB1096HFS(fd);
      return 0;
      break;
    case 'e':
      close(fd);
      return 0;
    default:
      break;
    }
  }
}
  



