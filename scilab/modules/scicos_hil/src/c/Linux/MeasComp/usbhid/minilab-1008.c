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
#include "minilab-1008.h"

enum Mode {Differential, SingleEnded};

/* configures digital port */
void usbDConfigPort_miniLAB1008(int fd, __u8 port, __u8 direction)
{
  __u8 cmd[3];

  cmd[0] = DCONFIG;
  cmd[1] = port;
  cmd[2] = direction;
  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* reads digital port  */
void usbDIn_miniLAB1008(int fd, __u8 port, __u8* din_value)
{
  __u8 cmd[2];
  
  cmd[0] = DIN;
  cmd[1] = port;
  
  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, din_value, 1, LS_DELAY);

}

/* writes digital port */
void usbDOut_miniLAB1008(int fd, __u8 port, __u8 value) 
{
  __u8 cmd[3];
  
  cmd[0] = DOUT;
  cmd[1] = port;
  cmd[2] = value;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* reads digital port bit */
__u8 usbDBitIn_miniLAB1008(int fd, __u8 port, __u8 bit) 
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
void usbDBitOut_miniLAB1008(int fd, __u8 port, __u8 bit, __u8 value)
{
  __u8 cmd[4];
  
  cmd[0] = DBIT_OUT;
  cmd[1] = port;
  cmd[2] = bit;
  cmd[3] = value;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* reads from analog in */
signed short usbAIn_miniLAB1008(int fd, __u8 channel, __u8 range)
{
  enum Mode mode;

  __u8 cmd[3];
  __s16 value;

  cmd[0] = AIN;
  cmd[1] = channel;
  cmd[2] = range;

  if ( range == SE_10_00V ) {
    mode = SingleEnded;
  } else {
    mode = Differential;
  }

  if (channel > 3 && mode == Differential ) {
    printf("usbAIN: channel out of range for differential mode.\n");
    return -1;
  }

  if (channel > 7 && mode == SingleEnded ) {
    printf("usbAIN: channel out of range for single ended mode.\n");
    return -1;
  }

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, (__u8 *) &value, sizeof(value), LS_DELAY);
  if ( mode == Differential ) {
    /* the data is a 2's compliment signed 12 bit number */
    value /= (1<<4);
  } else {
    /* the data is a  2's compliment signed 11 bit number */
    value /= (1<<5);
  }
  return value;
}

  
/* writes to analog out */
void usbAOut_miniLAB1008(int fd, __u8 channel, __u16 value) 
{
  __u8 cmd[4];

  cmd[0] = AOUT;
  cmd[1] = channel;
  cmd[2] = (__u8) (value & 0xff);
  cmd[3] = (__u8) ((value >> 8) & 0xff);
  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

void usbAInScan_miniLAB1008(int fd, __u16 count, int rate, __u8 low_channel, __u8 high_channel, __u8 options, __s16 value[], __u8 gainLoadQueue[])
{
  int i, idx;
  int scans;
  __u16 scanIndex;
  __u16 actual_scanIndex;
  __u8 chanCount;
  __u8 chanLoadQueue[8];
  
  struct {
    __u8 cmd;
    __u8 lo_count;
    __u8 hi_count;
    __u8 timer_preload;
    __u8 timer_prescale;
    __u8 options;
    __u8 extra[2];
  } out;

  struct {
//    __u8 recordNum;
    __u8 data[96];
    __u8 error;
    __u8 readAddress[2];
    __u8 writeAddress[2];
    __u8 scanIndex[2];
    __u8 extra;
  } in;
  
  __u8 buffer[9] = { 0, 0, 0, 0, 0, 0, 0, 0};

  int timerMult;
  __u8 timerVal;
  __u8 prescale;
  __u8 preload;
  __u8 setupTime;

  usbAInStop_miniLAB1008(fd);   // just to make sure.

  if ((100 <= rate) && (rate < 200)) {       // Select 256:1 prescaler
    prescale = 7;
    setupTime = 0;
  } else if ((200 <= rate) && (rate < 400)) { // Select 128:1 prescaler
    prescale = 6;
    setupTime = 0;
  } else if ((400 <= rate) && (rate < 800)) { // Select 64:1 prescaler
    prescale = 5;
    setupTime = 0;
  } else if ((800 <= rate) && (rate < 1500)) { // Select 32:1 prescaler
    prescale = 4;
    setupTime = 1;
  } else if ((1500 <= rate) && (rate < 3000)) { // Select 16:1 prescaler
    prescale = 3;
    setupTime = 3;
  } else if ((3000 <= rate) && (rate < 6000)) { // Select 8:1 prescaler
    prescale = 2;
    setupTime = 6;
  } else if ((6000 <= rate) && (rate < 8192)) { // Select 4:1 prescaler
    prescale = 1;
    setupTime = 10;
  } else {
    printf("usbAInScan_miniLAB1008: sampling rate out of range.\n");
    return;
  }

  timerMult = 1 << (prescale + 1);
  timerVal = (__u8) ((256 - (MINILAB_CLOCK / (rate * timerMult))) + 0.5);
  preload = timerVal + setupTime;

  /* set up gain queue */
  chanCount = high_channel - low_channel + 1;
  for ( i = 0; i < chanCount; i++ ) {
    chanLoadQueue[i] = low_channel + i;
  }
  printf("Sending AInLoadQueue report now ...\n");
  usbAInLoadQueue_miniLAB1008(fd, chanCount, chanLoadQueue, gainLoadQueue);

  out.cmd = AIN_SCAN;
  out.lo_count = count & 0xff;
  out.hi_count = (count >> 8) & 0xff;
  out.timer_preload = preload;
  out.timer_prescale = prescale;
  out.options = options;

  printf("Sending ainscan report now ...\n");
  PMD_SendOutputReport(fd, 0, (__u8*) &out, sizeof(out), LS_DELAY);
  sleep(1);

  /*
     If in external trigger mode, then wait for the device to send back
     notice that the trigger has been received, then startup the acquisition
  */

  if ( options & AIN_TRIGGER ) {
    while ( buffer[1] != 0xC3 ) {  // wait until external trigger received
      buffer[0] = 0;
      PMD_GetInputReport(fd, 0, (__u8  *) &buffer, sizeof(buffer), LS_DELAY);
    }
  }

  /*
    In Burst Mode, wait for End of Block acquisition flag (0xA5)
  */

  if ( options & AIN_BURST_MODE ) {
    buffer[1] = 0;
    while ( buffer[1] != 0xA5 ) {  // wait until external trigger received
      buffer[0] = 0;
      PMD_GetInputReport(fd, 0, (__u8  *) &buffer, sizeof(buffer), LS_DELAY);
    }
  }
  
  /*
    Retrieve the AInScan Response
    GET HID Feature Report to collect the data buffer.  Each buffer will be 105
    bytes long.  The first bye will contain the record number and can be ignored.
    The following 96 bytes will represent 64 samples of data.
  */

  in.scanIndex[0] = 0xff;
  in.scanIndex[1] = 0xff;
  scanIndex = 0;
  idx = 0;
  for ( scans = 0; scans < count/65 + 1; scans++ ) {
    memset(&in, 0xbeef, sizeof(in));
    do {
      PMD_GetFeatureReport(fd, 0, (__u8 *) &in, sizeof(in), 2*LS_DELAY);
      actual_scanIndex = (__u16) (in.scanIndex[0] | (in.scanIndex[1] << 8));
      printf("scan_index[0] = %d  scan_index[1] = %d\n", in.scanIndex[0],
	     in.scanIndex[1]);
    } while ( scanIndex != actual_scanIndex );
    scanIndex++;
    printf("Completed scan %d  error = %d\n", scanIndex, in.error);
    for ( i = 0; i < 96; i += 3, idx += 2 ) {
      printf("data[%d] = %#x  data[%d] = %#x\n", i, in.data[i], i+1, in.data[i+1]);
      value[idx] = in.data[i] | ((in.data[i+1]<<4) & 0x0f00);
      value[idx + 1] = in.data[i+2] | ((in.data[i+1]<< 8) & 0x0f00);
    }
  }
}

void usbAInLoadQueue_miniLAB1008(int fd, __u8 chanCount, __u8 chanLoadQueue[], __u8 gainLoadQueue[])
{
  int i;   
  struct {
    __u8 cmd;
    __u8 count;
    __u8 gains[6];
  } out;

  out.cmd = ALOAD_QUEUE;
  out.count = chanCount;   // can be 1, 2, 4, or 8
  chanCount = (chanCount == 8) ? 6 : chanCount;
  for ( i = 0; i < chanCount; i++ ) {
    out.gains[i] = (chanLoadQueue[i] & 0x7) | gainLoadQueue[i] | 0x80;
  }
  PMD_SendOutputReport(fd, 0, (__u8*) &out, sizeof(out), LS_DELAY);

  // Configure the rest of the channels (channel 6 and 7 )
  if ( out.count == 8 ) {
    out.count = 0x2;
    for ( i = 6; i < 8; i++ ) {
    out.gains[6-i] = (chanLoadQueue[i] & 0x7) | gainLoadQueue[i] | 0x80;  
    }
    PMD_SendOutputReport(fd, 0, (__u8*) &out, sizeof(out), LS_DELAY);
  }
}

void usbAInStop_miniLAB1008(int fd)
{
  __u8 cmd[1];
  
  cmd[0] = AIN_STOP;
  
  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}  


/* Initialize the counter */
void usbInitCounter_miniLAB1008(int fd)
{
  __u8 cmd[1];
  
  cmd[0] = CINIT;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

__u32 usbReadCounter_miniLAB1008(int fd)
{
  __u8 cmd[1];
  __u32 value;

  cmd[0] = CIN;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, (__u8  *) &value, sizeof(value), LS_DELAY);

  return value;
}

void usbReadMemory_miniLAB1008(int fd, __u16 address, __u8 *data, __u8 count)
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
int usbBlink_miniLAB1008(int fd)
{
  __u8 cmd[1];

  cmd[0] = BLINK_LED;

  return PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* resets the USB device */
int usbReset_miniLAB1008(int fd)
{
  __u8 cmd[1];

  cmd[0] = RESET;

  return PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

/* configure external triger */
void usbSetTrigger_miniLAB1008(int fd, __u8 type, __u8 chan)
{
  __u8 cmd[3];

  cmd[0] = SET_TRIGGER;
  cmd[1] = type;
  cmd[2] = chan;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
}

__u8 usbGetID_miniLAB1008(int fd)
{
  __u8 cmd[1];
  __u8 data;

  cmd[0] = GET_ID;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  PMD_GetInputReport(fd, 0, &data, 1, LS_DELAY);
  sleep(1);  // wait a long time here;

  return data;
}

void usbSetID_miniLAB1008(int fd, __u8 id)
{
  __u8 cmd[2];

  cmd[0] = SET_ID;
  cmd[1] = id;

  PMD_SendOutputReport(fd, 0, cmd, sizeof(cmd), LS_DELAY);
  sleep(1);    // wait a long time here;
}
