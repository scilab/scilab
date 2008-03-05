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
#include "usb-temp.h"

#define FS_DELAY 10000

/* configures digital port */
void usbDConfigPort_USBTEMP(int fd, __u8 direction)
{
  __u8 cmd[1];

  cmd[0] = direction;

  PMD_SendOutputReport(fd, DCONFIG, cmd, sizeof(cmd), FS_DELAY);
}

/* configures digital bit */
void usbDConfigBit_USBTEMP(int fd, __u8 bit_num, __u8 direction)
{
  __u8 cmd[2];

  cmd[0] = bit_num;
  cmd[1] = direction;

  PMD_SendOutputReport(fd, DCONFIG_BIT, cmd, sizeof(cmd), FS_DELAY);
}

/* reads digital port  */
void usbDIn_USBTEMP(int fd, __u8* value)
{
  PMD_SendOutputReport(fd, DIN, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, DIN, value, 1, FS_DELAY);
  PMD_GetInputReport(fd, DIN, value, 1, FS_DELAY);
  return;
}

/* reads digital bit  */
void usbDInBit_USBTEMP(int fd, __u8 bit_num, __u8* value)
{
  __u8 cmd[1];

  cmd[0] = bit_num;

  PMD_SendOutputReport(fd, DBIT_IN, cmd, sizeof(cmd), FS_DELAY);
  PMD_GetInputReport(fd, DBIT_IN, value, sizeof(*value), FS_DELAY);
 
  return;
}

/* writes digital port */
void usbDOut_USBTEMP(int fd, __u8 value)
{
  __u8 cmd[1];

  cmd[0] = value;

  PMD_SendOutputReport(fd, DOUT, cmd, sizeof(cmd), FS_DELAY);
}

/* writes digital bit  */
void usbDOutBit_USBTEMP(int fd, __u8 bit_num, __u8 value)
{
  __u8 cmd[2];

  cmd[0] = bit_num;
  cmd[1] = value;

  PMD_SendOutputReport(fd, DBIT_OUT, cmd, sizeof(cmd), FS_DELAY);
  return;
}

void usbTin_USBTEMP(int fd, __u8 channel, __u8 units, float *value)
{

  /*
    This command reads the value from the specified input channel.  The return
    value is a 32-bit floating point value in the units configured fro the
    channel.  CJC readings will always be in Celsius.
  */
  
  __u8 cmd[2];

  cmd[0] = channel; // 0 - 7
  cmd[1] = units;   // 0 - temperature, 1 - raw measurement

  PMD_SendOutputReport(fd, TIN, cmd, sizeof(cmd), FS_DELAY);
  PMD_GetInputReport(fd, TIN, (__u8*) value, sizeof(float), FS_DELAY);
}

void usbTinScan_USBTEMP(int fd, __u8 start_chan, __u8 end_chan, __u8 units, float value[])
{
  int nchan;
  __u8 cmd[3];

  nchan = (end_chan - start_chan + 1);

  cmd[0] = start_chan;  // the first channel to return 0-7
  cmd[1] = end_chan;    // the last channel to return 0-7
  cmd[2] = units;       // 0 - temperature, 1 - raw measurement

  PMD_SendOutputReport(fd, TIN_SCAN, cmd, sizeof(cmd), FS_DELAY);
  PMD_GetInputReport(fd, TIN_SCAN, (__u8*) value, nchan*sizeof(float), FS_DELAY);
}

/* blinks the LED of USB device */
int usbBlink_USBTEMP(int fd)
{
  return PMD_SendOutputReport(fd, BLINK_LED, 0, 0, FS_DELAY);
}

int usbReset_USBTEMP(int fd)
{
  return PMD_SendOutputReport(fd, RESET, 0, 0, FS_DELAY);
}


__u8 usbGetStatus_USBTEMP(int fd)
{
  __u8 status;

  PMD_SendOutputReport(fd, GET_STATUS, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, GET_STATUS, (__u8 *) &status, sizeof(status), FS_DELAY);
  return status;
}

void usbReadMemory_USBTEMP(int fd, __u16 address, __u8 type, __u8 count, __u8 *memory)
{
  struct t_arg {
    __u16 address;
    __u8 type;     // 0 = main microcontroller  1 = isolated microcontroller
    __u8 count;
  } arg;
  if ( count > 62 && type == 0) count = 62;  // 62 bytes max for main microcontroller
  if ( count > 60 && type == 1) count = 60;  // 60 bytes max for isolated microcontroller
  arg.type = type;
  arg.address = address;
  arg.count = count;

  PMD_SendOutputReport(fd, MEM_READ, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  PMD_GetInputReport(fd, MEM_READ,  memory, count, FS_DELAY);
}

int usbWriteMemory_USBTEMP(int fd, __u16 address, __u8 type, __u8 count, __u8* data)
{
  // Locations 0x00-0xFF are available on the main microcontroller
  int i;
  struct t_arg {
    __u16 address;  // start address for the write (0x00-0xFF)
    __u8 type;      // 0 = main microcontroller  1 = isolated microcontroller
    __u8 count;     // number of bytes to write (59 max)
    __u8 data[count];
  } arg;

  if ( address > 0xff ) return -1;
  if ( count > 59 ) count = 59;
  
  arg.address = address;
  arg.count = count;
  arg.type = type;
  for ( i = 0; i < count; i++ ) {
    arg.data[i] = data[i];
  }
  PMD_SendOutputReport(fd, MEM_WRITE, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  return 0;
}

void usbSetItem_USBTEMP(int fd, __u8 item, __u8 subitem, __u32 value)
{
    int size;
    __u8 cmd[6];
    
  if ( item > 3 ) {
    printf("Error: usbSetItem_USBTEMP  Item = %d too large.\n", item);
  }

  switch (subitem) {
    case SENSOR_TYPE:
    case CONNECTION_TYPE:
    case FILTER_RATE:
    case EXCITATION:
    case CH_0_TC:
    case CH_1_TC:
    case CH_0_GAIN:
    case CH_1_GAIN:
      size = 1;
      break;
    case VREF:
    case I_value_0:
    case I_value_1:
    case I_value_2:
    case V_value_0:
    case V_value_1:
    case V_value_2:
    case CH_0_COEF_0:
    case CH_1_COEF_0:
    case CH_0_COEF_1:
    case CH_1_COEF_1:
    case CH_0_COEF_2:
    case CH_1_COEF_2:
    case CH_0_COEF_3:
    case CH_1_COEF_3:
      size = 4;
      break;
    default:
      printf("Error usbSetItem_USBTEMP: subitem = %#x unknown\n", subitem);
      return;
      break;
  }
    
  cmd[0] = item;
  cmd[1] = subitem;
  memcpy(&cmd[2], &value, size);

  PMD_SendOutputReport(fd, SET_ITEM,  cmd, size+2, FS_DELAY);
}

void usbGetItem_USBTEMP(int fd, __u8 item, __u8 subitem, void *value)
{
    int size;
  __u8 cmd[2];

  cmd[0] = item;
  cmd[1] = subitem;

  switch (subitem) {
    case SENSOR_TYPE:
    case CONNECTION_TYPE:
    case FILTER_RATE:
    case EXCITATION:
    case CH_0_TC:
    case CH_1_TC:
    case CH_0_GAIN:
    case CH_1_GAIN:
      size = 1;
      break;
    case VREF:
    case I_value_0:
    case I_value_1:
    case I_value_2:
    case V_value_0:
    case V_value_1:
    case V_value_2:
    case CH_0_COEF_0:
    case CH_1_COEF_0:
    case CH_0_COEF_1:
    case CH_1_COEF_1:
    case CH_0_COEF_2:
    case CH_1_COEF_2:
    case CH_0_COEF_3:
    case CH_1_COEF_3:
      size = 4;
      break;
    default:
      printf("Error usbGetItem_USBTEMP: subitem = %#x unknown\n", subitem);
      return;
  }

  PMD_SendOutputReport(fd, GET_ITEM,  cmd, sizeof(cmd), FS_DELAY);
  PMD_GetInputReport(fd, GET_ITEM,  value, size, FS_DELAY);
}

void usbCalibrate_USBTEMP(int fd)
{
  /*
    The command instructs the device to perform a calibration on all channels.  Used after
    reconfiguring the channel(s).  This may take up to several seconds, and the completion
    may be determined by polling the status with usbGetStatus.  Temperature readings will
    not be updated while the calibration is ongoing, but DIO operations may be performed.
    The device will not accept usbSetItem or usbMemWrite commands while calibration is
    being performed.
  */
  PMD_SendOutputReport(fd, CALIBRATE, 0, 0, FS_DELAY);
  printf("Calibrating.  Please wait ");
  do {
    sleep(1);
    printf(".");
  } while ((usbGetStatus_USBTEMP(fd) & 0x1) == 1);
  printf("\n");
}

__u8  usbGetBurnoutStatus_USBTEMP(int fd, __u8 mask)
{
    /*
     This command returns the status of burnout detection for thermocouple channels.  The
     return value is a bitmap indicating the burnout detection status for all 8 channels.
     Individual bits will be set if an open circuit has been detected on that channel.  The
     bits will be cleared after the call using the mask that is passed as a parameter. If
     a bit is set, the corresponding bit in the status will be left at its current value.
  */

  __u8 status;
  
  PMD_SendOutputReport(fd, GET_BURNOUT_STATUS, 0, 0, FS_DELAY);
  PMD_GetInputReport(fd, GET_BURNOUT_STATUS,  &status, sizeof(status), FS_DELAY);
  return (status & mask);
}

void usbPrepareDownload_USBTEMP(int fd, __u8 micro)
{
  /*
    This command puts the device into code update mode.  The unlock code must be correct as a
    further safety device.  Call this once before sending code with usbWriteCode.  If not in
    code update mode, any usbWriteCode will be ignored.  A usbReset command must be issued at
    the end of the code download in order to return the device to operation with the new code.
  */

  const __u8 unlock_code = 0xad;

  __u8 cmd[2];

  cmd[0] = unlock_code;
  cmd[1] = micro;            // 0 = main, 1 = isolated
  PMD_SendOutputReport(fd, PREPARE_DOWNLOAD, cmd, sizeof(cmd), FS_DELAY);
}

void usbWriteCode_USBTEMP(int fd, __u32 address, __u8 count, __u8 data[])
{
  /*
    This command writes to the program memory in the device.  This command is not accepted
    unless the device is in update mode.  This command will normally be used when downloading
    a new hex file, so it supports memory ranges that may be found in the hex file.  The
    microcontroller that is being written to is selected with the "Prepare Download" command.

    The address ranges are:

    0x000000 - 0x0075FF:  Microcontroller FLASH program memory
    0x200000 - 0x200007:  ID memory (serial number is stored here on main micro)
    0x300000 - 0x30000F:  CONFIG memory (processor configuration data)
    0xF00000 - 0xF03FFF:  EEPROM memory

    FLASH program memory: The device must receive data in 64-byte segments that begin
    on a 64-byte boundary.  The data is sent in messages containing 32 bytes.  count
    must always equal 32.

    Other memory: Any number of bytes up to the maximum (32) may be sent.
    
  */

  struct t_arg {
    __u8 address[3];
    __u8 count;
    __u8 data[32];
  } arg;

  memcpy(&arg.address[0], &address, 3);   // 24 bit address
  arg.count = count;
  memcpy(&arg.data[0], data, count);      // 24 bit address
  PMD_SendOutputReport(fd, WRITE_CODE, (__u8 *) &arg, count+4, FS_DELAY);
}

void usbWriteSerial_USBTEMP(int fd, __u8 serial[8])
{

  // Note: The new serial number will be programmed but not used until hardware reset.
  
  PMD_SendOutputReport(fd, WRITE_SERIAL, serial, 8, FS_DELAY);
}

int usbReadCode_USBTEMP(int fd, __u32 address, __u8 count, __u8 data[])
{
  struct t_arg {
    __u8 address[3];
    __u8 count;
  } arg;

  int bRead;

  if ( count > 62 ) count = 62;  

  memcpy(&arg.address[0], &address, 3);   // 24 bit address
  arg.count = count;
  PMD_SendOutputReport(fd, READ_CODE, (__u8 *) &arg, sizeof(arg), FS_DELAY);
  bRead = PMD_GetInputReport(fd, READ_CODE,  data, count, FS_DELAY);
  return bRead;
}
