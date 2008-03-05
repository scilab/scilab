/*
 *
 *  Copyright (c) 2005  Warren Jasper <wjasper@tx.ncsu.edu>
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
#include "usb-1616FS.h"

#define FS_DELAY 2000

/* configures digital port */
void usbDConfigPort_USB1616FS(int fd, __u8 direction)
{
  PMD_SendOutputReport(fd, DCONFIG, &direction, sizeof(direction), FS_DELAY);
}

/* configures digital bit */
void usbDConfigBit_USB1616FS(int fd, __u8 bit_num, __u8 direction)
{
  __u8 cmd[2];

  cmd[0] = bit_num;     // the bit to configure 0-7
  cmd[1] = direction;   // 0 = output, 1 = input

  PMD_SendOutputReport(fd, DCONFIG_BIT, cmd, sizeof(cmd), FS_DELAY);
}

/* reads digital port  */
__u8 usbDIn_USB1616FS(int fd)
{
  __u8 value;
  
  PMD_SendOutputReport(fd, DIN, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, DIN, &value, sizeof(value), FS_DELAY);
  return value;
}

/* reads digital bit  */
__u8 usbDInBit_USB1616FS(int fd, __u8 bit_num)
{
  __u8 value;

  PMD_SendOutputReport(fd, DBIT_IN, &bit_num, sizeof(bit_num), FS_DELAY);
  PMD_GetInputReport(fd, DBIT_IN, &value, sizeof(value), FS_DELAY);
  return value;
}

/* writes digital port */
void usbDOut_USB1616FS(int fd, __u8 value)
{
  PMD_SendOutputReport(fd, DOUT, &value, sizeof(value), FS_DELAY);
}

/* writes digital bit  */
void usbDOutBit_USB1616FS(int fd, __u8 bit_num, __u8 value)
{
  __u8 cmd[2];

  cmd[0] = bit_num & 0x7;  // the bit to write (0 - 7)
  cmd[1] = value & 0x1;    // the value to write to the bit (0 or 1);

  PMD_SendOutputReport(fd, DBIT_OUT, cmd, sizeof(cmd), FS_DELAY);
  return;
}

/* reads from analog in */
__s16 usbAIn_USB1616FS(int fd, __u8 channel, __u8 range)
{
  __u8 cmd[2];
  __u16 data;
  __s16 value;

  cmd[0] = channel;
  cmd[1] = range;
  
  if (channel > 15) {
    printf("usbAIN: channel out of range for differential mode.\n");
    return -1;
  }

  if (range > 7) {
    printf("usbAIN: range setting too large.\n");
    return -1;
  }

  PMD_SendOutputReport(fd, AIN, cmd, sizeof(cmd), FS_DELAY);
  PMD_GetInputReport(fd, AIN, (__u8 *) &data, sizeof(data), FS_DELAY);
  
  if ( data >= 0x8000 ) {
    value = (data - 0x8000);
  } else {
    value = (0x8000 - data);
    value *= (-1);
  }

  return value;
}

void usbAInStop_USB1616FS(int fd)
{
   PMD_SendOutputReport(fd, AIN_STOP, 0, 0, FS_DELAY);
}

int usbAInScan_USB1616FS(int fd[], __u8 lowchannel, __u8 highchannel, __u32 count,
			  float *frequency, __u8 options, __s16 sdata[])
{
  int num_samples;
  int i, j, k;
  int scan_index = 0;
  __u32 preload;

  struct {
    __u16 value[31];
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

  if ( highchannel > 15 ) {
    printf("usbAInScan: highchannel out of range.\n");
    return -1;
  }
  if ( lowchannel > 15 ) {
    printf("usbAInScan: lowchannel out of range.\n");
    return -1;
  }
  if (highchannel >= lowchannel) {
    num_samples = count*(highchannel - lowchannel + 1);
  } else {
    num_samples = count*((15-highchannel) + lowchannel + 1);
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

  printf("AInScan: actual frequency = %f\n", *frequency);

  if ( arg.prescale == 9 || preload == 0) {
    printf("usbAInScan_USB1616FS: frequency out of range.\n");
    return -1;
  }
  count = num_samples;  // store value of samples.
  PMD_SendOutputReport(fd[0], AIN_SCAN, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  i = 0;

  while ( num_samples > 0 ) {         
    for ( j = 1; j <= 6; j++ ) {         // cycle through the ADC pipes
    //read(fd[j], &ev, sizeof(ev));    // wait for hid event.
      do {                               // lets poll for now.  Events are not reliable.
	PMD_GetInputReport(fd[j], 0, (__u8 *) &data, 64, FS_DELAY);
      } while ( data.cnt != scan_index); // keep reading until we are in sync.
      scan_index++;

      if ( num_samples > 31 ) {
	for ( k = 0; k < 31;  k++ ) {
	  if (data.value[k] >= 0x8000) {
	    sdata[i+k] = data.value[k] - 0x8000;
	  } else {
	    sdata[i+k] = 0x8000 - data.value[k];
	    sdata[i+k] *= (-1);
	  }
	}
        num_samples -= 31;
	i += 31;
      } else {   // only copy in a partial scan
	for ( k = 0; k < num_samples;  k++ ) {
	  if (data.value[k] >= 0x8000) {
	    sdata[i+k] = data.value[k] - 0x8000;
	  } else {
	    sdata[i+k] = 0x8000 - data.value[k];
	    sdata[i+k] *= (-1);
	  }
	}
        num_samples -= 31;
	i += num_samples;
        break;
      }
      printf("Scan count = %d\tnumber samples left = %d\n", data.cnt, num_samples);
    }
  }

  return count;
}

void usbAInLoadQueue_USB1616FS(int fd, __u8 gainArray[16])
{
  PMD_SendOutputReport(fd, ALOAD_QUEUE, gainArray, 16, FS_DELAY);
}

/* Initialize the counter */
void usbInitCounter_USB1616FS(int fd)
{
  PMD_SendOutputReport(fd, CINIT, 0, 0, FS_DELAY);
}

__u32 usbReadCounter_USB1616FS(int fd)
{
  __u32 value;

  PMD_SendOutputReport(fd, CIN, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, CIN, (__u8  *) &value, sizeof(value), FS_DELAY);

  return value;
}

/* blinks the LED of USB device */
int usbBlink_USB1616FS(int fd)
{
  return PMD_SendOutputReport(fd, BLINK_LED, 0, 0, FS_DELAY);
}

int usbReset_USB1616FS(int fd)
{
  return PMD_SendOutputReport(fd, RESET, 0, 0, FS_DELAY);
}

void usbSetTrigger_USB1616FS(int fd, __u8 type)
{
  PMD_SendOutputReport(fd, SET_TRIGGER, &type, sizeof(type), FS_DELAY);
}

void usbSetSync_USB1616FS(int fd, __u8 type)
{
  /*
    This command configures the sync signal.  The sync signal may be used to synchronize the
    analog input scan of multiple devices.  When multiple devices are to be used, one device is
    selected as the master and the rest as slaves.  The sync signal of all devices must be wired
    together.  The master will output a pulse every sample, and all the devices will acquire thier
    samples simultaneously.  The may also be used to pace one or more devices from an external
    TTL/CMOS clock signal (max rate = 50kHz)

    This may also be used with an external trigger; the external trigger signal should be brought to
    the master device, and all devices will begin sampling wihen the master is triggered.

    If the device is configured as a slave, it will not acquire data when given an AInScan command
    until it detects a pulse onthe syc input.

    type   0 = master,   1 = slave
  */
  
  PMD_SendOutputReport(fd, SET_SYNC, &type, sizeof(type), FS_DELAY);
}

__u16 usbGetStatus_USB1616FS(int fd)
{
  /*
    Bit  0:   0 = Sync slave,            1 = sync master
    Bit  1:   0 = trigger falling edge,  1 = trigger rising edge
    Bit 16:   1 = program memory update mode
  */

  __u16 status;

  PMD_SendOutputReport(fd, GET_STATUS, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, GET_STATUS, (__u8 *) &status, sizeof(status), FS_DELAY);
  return status;
}

void usbReadMemory_USB1616FS( int fd, __u16 address, __u8 type, __u8 count, __u8* memory)
{
  struct arg {
    __u16 address;
    __u8 type;  // (0 = EEPROM,  1 = SRAM)
    __u8 count; // (62 bytes max)
  } arg;

  if ( count > 62 ) count = 62;
  arg.type = type;
  arg.address = address;
  arg.count = count;

  PMD_SendOutputReport(fd, MEM_READ, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  PMD_GetInputReport(fd, MEM_READ, memory, count, FS_DELAY);
}

int usbWriteMemory_USB1616FS(int fd, __u16 address, __u8 count, __u8 data[])
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

void usbPrepareDownload_USB1616FS(int fd)
{
  /*
    This command puts the device into code update mode.  The unlock code must be correct as a
    further safety device.  Call this once before sending code with usbWriteCode.  If not in
    code update mode, any usbWriteCode will be ignored.  A usbReset command must be issued at
    the end of the code download in order to return the device to operation with the new code.
  */

  __u8 unlock_code = 0xad;

  PMD_SendOutputReport(fd, PREPARE_DOWNLOAD, &unlock_code, sizeof(__u8), FS_DELAY);
}

void usbWriteCode_USB1616FS(int fd, __u32 address, __u8 count, __u8 data[])
{
  /*
    This command send the new program memory image to the device.  The download program
    memory image is stored in external SRAM.  The image will be written to FLASH program
    memory when the UpdateCode command is issued (updates must be enabled with UnlockCode
    first.)  A running checksum will be calculated a the program memory image is sent, and the host
    should compare its own checksum with this value (retrieved with ReadChecksum) prior to
    sending the UpdateCode command.
  */

  struct t_arg {
    __u8 address[3];
    __u8 count;        // 58 bytes max
    __u8 data[58];     // the program data, 58 bytes max
  } arg;

  if (count > 58) return;

  memcpy(&arg.address[0], &address, 3);   // 24 bit address
  arg.count = count;
  memcpy(&arg.data[0], data, count);      // 24 bit address
  PMD_SendOutputReport(fd, WRITE_CODE, (__u8 *) &arg, count+4, FS_DELAY);
}

__u16 usbReadChecksum_USB1616FS(int fd)
{
  __u16 checksum;
  
  PMD_SendOutputReport(fd, READ_CHECKSUM, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, READ_CHECKSUM, (__u8 *)&checksum, sizeof(checksum), FS_DELAY);
  return checksum;
}

void usbWriteSerial_USB1616FS(int fd, __u8 serial[8])
{
  // Note: The new serial number will be programmed but not used until hardware reset.
  
  PMD_SendOutputReport(fd, WRITE_SERIAL, serial, 8, FS_DELAY);
}

void usbUpdateCode_USB1616FS(int fd)
{
  PMD_SendOutputReport(fd, UPDATE_CODE, 0, 0, FS_DELAY);
}

int usbReadCode_USB1616FS(int fd, __u32 address, __u8 count, __u8 data[])
{
  struct t_arg {
    __u8 address[3];
    __u8 count;     // 62 max.
  } arg;

  int bRead;

  if ( count > 62 ) count = 62;  

  memcpy(&arg.address[0], &address, 3);   // 24 bit address
  arg.count = count;
  PMD_SendOutputReport(fd, READ_CODE, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  bRead = PMD_GetInputReport(fd, READ_CODE,  data, count, FS_DELAY);
  return bRead;
}

float volts_USB1616FS( const int gain, const signed short num )
{
  float volt;

  switch( gain ) {
    case BP_10_00V:
      volt = num * 10.0 / 0x7fff;
      break;
    case BP_5_00V:
      volt = num * 5.0 / 0x7fff;
      break;
    case BP_2_50V:
      volt = num * 2.5 / 0x7fff;
      break;
    case BP_2_00V:
      volt = num * 2.0 / 0x7fff;
      break;
    case BP_1_25V:
      volt = num * 1.25 / 0x7fff;
      break;
    case BP_1_00V:
      volt = num * 1.0 / 0x7fff;
      break;
    case BP_0_625V:
      volt = num * 0.625 / 0x7ff;
      break;
    case BP_0_3125V:
      volt = num * 1.0 / 0x7fff;
      break;
  }
  return volt;
}
