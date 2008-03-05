/*
 *
 *  Copyright (c) 2004-2005  Warren Jasper <wjasper@tx.ncsu.edu>
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
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/hiddev.h>

#include "pmd.h"
#include "usb-1024LS.h"

#define LS_DELAY (20000)

static __u8 PortC = 0;

/* configures digital port */
void usbDConfigPort_USB1024LS(int fd, __u8 port, __u8 direction)
{
  __u8 cmd[3];

  cmd[0] = DCONFIG;
  cmd[1] = port;
  cmd[2] = direction;
  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* reads digital port  */
__u8 usbDIn_USB1024LS(int fd, __u8 port)
{
  __u8 cmd[2];
  __u8 value;
  
  cmd[0] = DIN;
  cmd[1] = port;
  
  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, &value, sizeof(value), LS_DELAY);
  if (port == DIO_PORTC_HI)  value >>= 4;
  if (port == DIO_PORTC_LOW) value &= 0xf;

  return value;
}

/* writes digital port */
void usbDOut_USB1024LS(int fd, __u8 port, __u8 value) 
{
  __u8 cmd[3];
  
  cmd[0] = DOUT;
  cmd[1] = port;
  cmd[2] = value;

  if (port == DIO_PORTC_LOW) {
    PortC &= (0xf0);
    PortC |= (value & 0xf);
    cmd[2] = PortC;
  }

  if (port == DIO_PORTC_HI) {
    PortC &= (0x0f);
    PortC |= (value << 0x4);
    cmd[2] = PortC;
  }

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* reads digital port bit */
__u8 usbDBitIn_USB1024LS(int fd, __u8 port, __u8 bit) 
{
  __u8 cmd[3];
  __u8 value;

  cmd[0] = DBIT_IN;
  cmd[1] = port;
  cmd[2] = bit;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, &value, sizeof(value), LS_DELAY);

  return value;
}

/* writes digital port bit */
void usbDBitOut_USB1024LS(int fd, __u8 port, __u8 bit, __u8 value)
{
  __u8 cmd[4];
  
  cmd[0] = DBIT_OUT;
  cmd[1] = port;
  cmd[2] = bit;
  cmd[3] = value;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* Initialize the counter */
void usbInitCounter_USB1024LS(int fd)
{
  __u8 cmd[1];
  
  cmd[0] = CINIT;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

__u32 usbReadCounter_USB1024LS(int fd)
{
  __u8 cmd[1];
  __u32 value;

  cmd[0] = CIN;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, (__u8  *) &value, sizeof(value), LS_DELAY);

  return value;
}

void usbReadMemory_USB1024LS(int fd, __u16 address, __u8 *data, __u8 count)
{
  __u8 cmd[4];
 
  cmd[0] = MEM_READ;
  cmd[1] = (__u8) (address & 0xff);  // low byte
  cmd[2] = (__u8) (address >> 0x8);  // high byte
  cmd[3] = count;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, data, count, LS_DELAY);
}

/* blinks the LED of USB device */
int usbBlink_USB1024LS(int fd)
{
  __u8 cmd[1];

  cmd[0] = BLINK_LED;

  return PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* resets the USB device */
int usbReset_USB1024LS(int fd)
{
  __u8 cmd[1];

  cmd[0] = RESET;

  return PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

__u8 usbGetID_USB1024LS(int fd)
{
  __u8 cmd[1];
  __u8 data;

  cmd[0] = GET_ID;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), 2*LS_DELAY);
  PMD_GetInputReport(fd, 0, &data, 1, 2*LS_DELAY);
  sleep(1);  // wait a long time here;

  return data;
}

void usbSetID_USB1024LS(int fd, __u8 id)
{
  __u8 cmd[2];

  cmd[0] = SET_ID;
  cmd[1] = id;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  sleep(1);    // wait a long time here;
}
