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
#include "usb-1208FS.h"

#define FS_DELAY 1000

enum Mode {Differential, SingleEnded};

/* configures digital port */
void usbDConfigPort_USB1208FS(int fd, __u8 port, __u8 direction)
{
  __u8 cmd[2];

  cmd[0] = port;
  cmd[1] = direction;

  PMD_SendOutputReport(fd, DCONFIG, cmd, sizeof(cmd), FS_DELAY);
}

/* reads digital port  */
void usbDIn_USB1208FS(int fd, __u8 port, __u8* din_value)
{
  __u8 value[2];

  PMD_SendOutputReport(fd, DIN, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, DIN, value, sizeof(value), FS_DELAY);

  /* don't return values off the stack*/
  if (port == DIO_PORTA) {
    *din_value = value[0];
  } else {
    *din_value = value[1];
  }
  return;
}

/* writes digital port */
void usbDOut_USB1208FS(int fd, __u8 port, __u8 value)
{
  __u8 cmd[2];

  cmd[0] = port;
  cmd[1] = value;

  PMD_SendOutputReport(fd, DOUT, cmd, sizeof(cmd), FS_DELAY);
}

/* writes to analog out */
void usbAOut_USB1208FS(int fd, __u8 channel, __u16 value)
{
  __u8 cmd[3];
  value <<= 0x4;

  cmd[0] = channel;                         // 0 or 1
  cmd[1] = (__u8) (value & 0xf0);           // low byte
  cmd[2] = (__u8) ((value >> 0x8) & 0xff);  // high byte

  PMD_SendOutputReport(fd, AOUT, cmd, sizeof(cmd), FS_DELAY);
}

int usbAOutScan_USB1208FS(int fd[], __u8 lowchannel, __u8 highchannel,
			  __u32 count, float *frequency, __u16 data[])
{
  int num_samples;
  int i;
  __u32 preload;
  __u8 byte;
  
  struct arg {
    __u8  lowchannel;
    __u8  highchannel;
    __u8  count[4];
    __u8  prescale;
    __u8  preload[2];
    __u8  options;
  } arg;

  if ( highchannel > 1 ) {
    printf("usbAOutScan: highchannel out of range.\n");
    return -1;
  }

  if ( lowchannel > 1 ) {
    printf("usbAOutScan: lowchannel out of range.\n");
    return -1;
  }

  if ( lowchannel > highchannel ) {
    printf("usbAOutScan: lowchannel greater than highchannel.\n");
    return -1;
  }

  num_samples = count*(highchannel - lowchannel + 1);

  arg.lowchannel = lowchannel;
  arg.highchannel = highchannel;
  arg.count[0] = (__u8) count & 0xff;           // low byte
  arg.count[1] = (__u8) (count >> 8) & 0xff;
  arg.count[2] = (__u8) (count >> 16) & 0xff;
  arg.count[3] = (__u8) (count >> 24) & 0xff;   // high byte
  arg.options = 1;                              // single execution

  for ( arg.prescale = 0; arg.prescale <= 8; arg.prescale++ ) {
    preload = 10e6/((*frequency) * (1<<arg.prescale));
    if ( preload <= 0xffff ) {
      arg.preload[0] = (__u8) preload & 0xff;          // low byte
      arg.preload[1] = (__u8) (preload >> 8) & 0xff;   // high byte
      break;
    }
  }

  if ( arg.prescale == 9 || preload == 0) {
    printf("usbAOutScan_USB1208FS: frequency out of range.\n");
    return -1;
  }
  
  *frequency = 10e6/((1<<arg.prescale)*preload);

  /* shift over all data 4 bits */
  for (i = 0; i < num_samples; i++) {
    data[i] <<= 4;
  }

  PMD_SendOutputReport(fd[0], AOUT_SCAN, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  
  i = 0;
  while( num_samples > 0) {
    PMD_GetInputReport(fd[0], AOUT_SCAN, &byte, 1, FS_DELAY);  // blocking on interrupt?
    if ( num_samples > 32 ) {
      PMD_SendOutputReport(fd[1], 0,(__u8*) &data[i], 64, FS_DELAY);
      num_samples -= 32;
      i += 32;
    } else {
      PMD_SendOutputReport(fd[1], 0,(__u8*) &data[i], num_samples*2, FS_DELAY);
      num_samples = 0;
    }
  }
  return 0;
}

void usbAOutStop_USB1208FS(int fd)
{
  PMD_SendOutputReport(fd, AOUT_STOP, 0, 0, FS_DELAY);
}

/* reads from analog in */
signed short usbAIn_USB1208FS(int fd, __u8 channel, __u8 range)
{
  enum Mode mode;

  __u8 cmd[2];
  __s16 value;
  __u16 uvalue;

  cmd[0] = channel;
  cmd[1] = range;
  
  if ( (range == SE_10_00V) && (channel > 7)) {
    mode = SingleEnded;
  } else {
    mode = Differential;
  }

  if ((channel > 7) && (mode == Differential)) {
    printf("usbAIN: channel out of range for differential mode.\n");
    return -1;
  }

  if (((channel > 15) || (channel < 8)) && (mode == SingleEnded)) {
    printf("usbAIN: channel out of range for single ended mode.\n");
    return -1;
  }

  PMD_SendOutputReport(fd, AIN, cmd, sizeof(cmd), FS_DELAY);
  PMD_GetInputReport(fd, AIN, (__u8 *) &value, sizeof(value), FS_DELAY);

  if ( mode == Differential ) {
    /* the data is a 2's compliment signed 12 bit number */
    PMD_GetInputReport(fd, AIN, (__u8 *) &value, sizeof(value), FS_DELAY);
    value /= (1<<4);
  } else {
    /* the data is a  2's compliment signed 11 bit number */
    PMD_GetInputReport(fd, AIN, (__u8 *) &uvalue, sizeof(value), FS_DELAY);
    if (uvalue > 0x7ff0) {
      uvalue = 0;
    } else if ( uvalue > 0x7fe0 ) {
      uvalue = 0xfff;
    } else {
      uvalue >>= 3;
      uvalue &= 0xfff;
    }
    value = uvalue - 0x800;
  }

  return value;
}

void usbAInStop_USB1208FS(int fd)
{
   PMD_SendOutputReport(fd, AIN_STOP, 0, 0, FS_DELAY);
}

int usbAInScan_USB1208FS(int fd[], __u8 lowchannel, __u8 highchannel, __u32 count,
			  float *frequency, __u8 options, __s16 sdata[])
{
  int num_samples;
  int i, j, k;
  int scan_index = 0;
  __u32 preload;

  struct {
    __s16 value[31];
    __u16 cnt;
  } data;

  struct hiddev_event ev;
  
  struct arg {
    __u8  lowchannel;
    __u8  highchannel;
    __u8  count[4];
    __u8  prescale;
    __u8  preload[2];
    __u8  options;
  } arg;

  if ( highchannel > 7 ) {
    printf("usbAInScan: highchannel out of range.\n");
    return -1;
  }
  if ( lowchannel > 7 ) {
    printf("usbAInScan: lowchannel out of range.\n");
    return -1;
  }
  if (highchannel >= lowchannel) {
    num_samples = count*(highchannel - lowchannel + 1);
  } else {
    num_samples = count*((8-highchannel) + lowchannel + 1);
  }

  arg.lowchannel = lowchannel;
  arg.highchannel = highchannel;
  arg.count[0] = (__u8) count & 0xff;           // low byte
  arg.count[1] = (__u8) (count >> 8) & 0xff;
  arg.count[2] = (__u8) (count >> 16) & 0xff;
  arg.count[3] = (__u8) (count >> 24) & 0xff;   // high byte
  arg.options = options;                        

  for ( arg.prescale = 0; arg.prescale <= 8; arg.prescale++ ) {
    preload = 10e6/((*frequency) * (1<<arg.prescale));
    if ( preload <= 0xffff ) {
      arg.preload[0] = (__u8) preload & 0xff;          // low byte
      arg.preload[1] = (__u8) (preload >> 8) & 0xff;   // high byte
      break;
    }
  }

  *frequency = 10.e6/(preload*(1<<arg.prescale));

  //  printf("AInScan: actual frequency = %f\n", *frequency);

  if ( arg.prescale == 9 || preload == 0) {
    printf("usbAInScan_USB1208FS: frequency out of range.\n");
    return -1;
  }
  count = num_samples;  // store value of samples.
  PMD_SendOutputReport(fd[0], AIN_SCAN, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  i = 0;

  while ( num_samples > 0 ) {         
    for ( j = 1; j <= 3; j++ ) {         // cycle through the ADC pipes
      //read(fd[j], &ev, sizeof(ev));    // wait for hid event.
      do {                               // lets poll for now.  Events are not reliable.
	PMD_GetInputReport(fd[j], 0, (__u8 *) &data, 64, 100);
      } while ( data.cnt != scan_index); // keep reading until we are in sync.
      scan_index++;

      if ( num_samples > 31 ) {
	for ( k = 0; k < 31;  k++ ) {
          sdata[i+k] = data.value[k];
	}
        num_samples -= 31;
	i += 31;
      } else {   // only copy in a partial scan
	for ( k = 0; k < num_samples;  k++ ) {
          sdata[i+k] = data.value[k];
	}
        num_samples -= 31;
	i += 31;
        break;
      }
      printf("Scan count = %d\tnumber samples left = %d\n", data.cnt, num_samples);
    }
  }

  // Differential mode: data in 2's compliment signed 12 bit 
  for ( i = 0; i < count; i++ ) {
    sdata[i] /= (1<<4);
  }
  return count;
}

void usbALoadQueue_USB1208FS(int fd, __u8 num, __u8 gains[])
{
  __u8 arg[2*num+1];
  int i;

  num = (num <= 8) ? num : 8;
  
  for ( i = 1; i <= 2*num; i++ ) {
    arg[i] = gains[i] & 0x7;
  }
  arg[0] = num;
  PMD_SendOutputReport(fd, ALOAD_QUEUE, arg, 2*num+1, FS_DELAY);
}

/* Initialize the counter */
void usbInitCounter_USB1208FS(int fd)
{
  PMD_SendOutputReport(fd, CINIT, 0, 0, FS_DELAY);
}

__u32 usbReadCounter_USB1208FS(int fd)
{
  __u32 value;

  PMD_SendOutputReport(fd, CIN, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, CIN, (__u8  *) &value, sizeof(value), FS_DELAY);

  return value;
}


/* blinks the LED of USB device */
int usbBlink_USB1208FS(int fd)
{
  return PMD_SendOutputReport(fd, BLINK_LED, 0, 0, FS_DELAY);
}

int usbReset_USB1208FS(int fd)
{
  return PMD_SendOutputReport(fd, RESET, 0, 0, FS_DELAY);
}

void usbSetTrigger_USB1208FS(int fd, __u8 type)
{
  PMD_SendOutputReport(fd, SET_TRIGGER, &type, sizeof(type), FS_DELAY);
}

void usbSetSync_USB1208FS(int fd, __u8 type)
{
  PMD_SendOutputReport(fd, SET_SYNC, &type, sizeof(type), FS_DELAY);
}

__u16 usbGetStatus_USB1208FS(int fd)
{
  __u16 status;

  PMD_SendOutputReport(fd, GET_STATUS, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, GET_STATUS, (__u8 *) &status, sizeof(status), FS_DELAY);
  return status;
}

void usbReadMemory_USB1208FS( int fd, __u16 address, __u8 count, __u8* memory)
{
  // Addresses 0x000 - 0x07F are reserved for firmware data
  // Addresses 0x080 - 0x3FF are available for use as calibraion or user data
  struct arg {
    __u16 address;
    __u8 type;
    __u8 count;
  } arg;

  if ( count > 62 ) count = 62;
  arg.type = 0;
  arg.address = address;
  arg.count = count;

  PMD_SendOutputReport(fd, MEM_READ, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  PMD_GetInputReport(fd, MEM_READ, memory, count, FS_DELAY);
}

int usbWriteMemory_USB1208FS(int fd, __u16 address, __u8 count, __u8* data)
{
  // Locations 0x00-0x7F are reserved for firmware and my not be written.
  int i;
  struct arg {
    __u16 address;
    __u8 count;
    __u8 data[count];
  } arg;

  if ( address <=0x7f ) return -1;
  if ( count > 59 ) count = 59;
  
  arg.address = address;
  arg.count = count;
  for ( i = 0; i < count; i++ ) {
    arg.data[i] = data[i];
  }
  PMD_SendOutputReport(fd, MEM_WRITE, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  return 0;
}

void usbGetAll_USB1208FS(int fd, __u8 data[])
{
  PMD_SendOutputReport(fd, GET_ALL, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, GET_ALL,  data, 18, FS_DELAY);
}

/* converts signed short value to volts for Single Ended Mode */
float volts_SE( const signed short num )
{
  float volt;
  volt = num * 10.0 / 0x7ff;
  return volt;
}

/* converts signed short value to volts for Differential Mode */     
float volts_FS( const int gain, const signed short num )
{
  float volt;

  switch( gain ) {
    case BP_20_00V:
      volt = num * 20.0 / 0x7ff;
      break;
    case BP_10_00V:
      volt = num * 10.0 / 0x7ff;
      break;
    case BP_5_00V:
      volt = num * 5.0 / 0x7ff;
      break;
    case BP_4_00V:
      volt = num * 4.0 / 0x7ff;
      break;
    case BP_2_50V:
      volt = num * 2.5 / 0x7ff;
      break;
    case BP_2_00V:
      volt = num * 2.0 / 0x7ff;
      break;
    case BP_1_25V:
      volt = num * 1.25 / 0x7ff;
      break;
    case BP_1_00V:
      volt = num * 1.0 / 0x7ff;
      break;
  }
  return volt;
}
