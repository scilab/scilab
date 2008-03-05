/*
 *
 *  Copyright (c) 2004-2005 Warren Jasper <wjasper@tx.ncsu.edu>
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
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/hiddev.h>
#include "pmd.h"

#define dsyslog if(debug) syslog

int debug = 0;

// Driver Functions

int PMD_Find(int vendor, int product, int fd[])
{
  int i;
  int num_interfaces = 0;    // number of interfaces for this HID device
  char devname[80];
  char devname_old[80];
  struct hiddev_devinfo device_info;

  for(i = 0; i < MAX_HIDDEV; i++){
    sprintf(devname, "/dev/hiddev%d", i);         // new locatin for hiddev
    sprintf(devname_old, "/dev/usb/hiddev%d", i); // old location for hiddev
    if ( (fd[num_interfaces] = open(devname, O_RDONLY)) < 0 ) {
      if ( (fd[num_interfaces] = open(devname_old, O_RDONLY)) < 0 ) {
        continue;
      }
    }
    /* get device information */
    if ( ioctl(fd[num_interfaces], HIDIOCGDEVINFO, &device_info) >= 0 ) {
      /* check vendor and product */
      if ( vendor == device_info.vendor &&
	   product == device_info.product ) {
	/* this is us */
	num_interfaces++;
	continue;
      } else {
	close(fd[num_interfaces]);
      }
    }
  }
  return num_interfaces;
}

int PMD_SendOutputReport(int fd, __u8 reportID, __u8* vals, int num_vals, int delay)
{
  int ret = 0, i;
  __s8 sval;
  struct hiddev_report_info out_report;
  struct hiddev_usage_ref uref;

  uref.report_type = HID_REPORT_TYPE_OUTPUT;
  uref.report_id = reportID;
  uref.field_index = 0;
  uref.usage_index = 0;

  /* usage code for this for this usage */
  ret = ioctl(fd, HIDIOCGUCODE, &uref);
  if(0 > ret){
    perror("SendCommandFS: ioctl to get usage code for out report");
    return ret;
  }

  /* fill in usage values. */
  for( i = 0; (i < num_vals) && vals; i++){
    uref.usage_index = i;
    sval = vals[i];
    uref.value = sval;
    ioctl(fd, HIDIOCSUSAGE, &uref);
    if(0 > ret){
      perror("SendCommandFS:ioctl to set usage value for out report");
      return ret;
    }
  }

  /* tell the driver about the usage values */
  out_report.report_type = HID_REPORT_TYPE_OUTPUT;
  out_report.report_id = reportID;
  ioctl(fd, HIDIOCGREPORTINFO, &out_report);
  if (0 > ret) {
    perror("SendCommandFS: ioctl to get out report info");
    return ret;
  }
  /* this ioctl puts the report on the wire */
  ret = ioctl(fd, HIDIOCSREPORT, &out_report);
  if (0 > ret) {
    perror("SendCommandFS: ioctl to send output report");
    return ret;
  }

  usleep(delay);
  return ret;
}

#if 0

struct hiddev_event ev[64];
int PMD_GetInputReport(int fd, __u8 reportID, __u8 *vals, int num_vals, int timeout)
{
  fd_set fdset;
  int rd;
  int ret = 0, i;
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = timeout * 1000;
  FD_ZERO(&fdset);
  i = 0;
  FD_SET(fd, &fdset);
  rd = select(fd+1, &fdset, NULL, NULL, (timeout <= 0 ? NULL : &tv));
  if (rd < 0) {
    dsyslog(LOG_INFO,"Error reading USB device - %s\n",strerror(errno));
    return rd;
  }
  if (rd == 0) {
    dsyslog(LOG_INFO,"hidread timeout fd = %d\n",fd);
    return 0;  // timeout
  }

  // now we have an event ready to go...
  rd = read(fd, ev, sizeof(ev));
  if (rd < 0) {
    dsyslog(LOG_INFO,"Error reading USB device - %s\n",strerror(errno));
    return rd;
  }
  if (rd < sizeof(ev[0])) {
    dsyslog(LOG_INFO,"Error - got short read from USB device\n");
    return -1;
  }
  
  /*struct hiddev_usage_ref_multi in_usage_multi;*/
  struct hiddev_usage_ref uref;

  uref.report_type = HID_REPORT_TYPE_INPUT;
  uref.report_id = reportID;
  uref.field_index = 0;
  uref.usage_index = 0;
  uref.usage_code = 0xff000001;

  for( i = 0; i < num_vals; i++){
    uref.usage_index = i;
    ret = ioctl(fd, HIDIOCGUSAGE, &uref);
    vals[i] = (__u8)(uref.value & 0x000000FF) ;
  }
  return num_vals;
}

#endif


int PMD_GetInputReport(int fd, __u8 reportID, __u8 *vals, int num_vals, int delay)
{
  int ret = 0, i;
  /*struct hiddev_usage_ref_multi in_usage_multi;*/
  struct hiddev_usage_ref uref;

  uref.report_type = HID_REPORT_TYPE_INPUT;
  uref.report_id = reportID;
  uref.field_index = 0;
  uref.usage_index = 0;
  uref.usage_code = 0xff000001;
  /*
  ioctl(fd, HIDIOCGUCODE, &uref);
  if( 0 > ret) {
    perror("ioctl to get output report usage code");
    return ret;
  }
  */

  for( i = 0; i < num_vals; i++){
      uref.usage_index = i;
      ioctl(fd, HIDIOCGUSAGE, &uref);
      usleep(delay);
      if(0 > ret){
          perror("ioctl to set usage value for out report");
          return ret;
      }
      vals[i] = (__u8)(uref.value & 0x000000FF) ;
  }
  return ret;
}

int PMD_GetFeatureReport(int fd, __u8 reportID, __u8 *vals, int num_vals, int delay)
{
  int ret = 0, i;
  struct hiddev_usage_ref uref;

  uref.report_type = HID_REPORT_TYPE_FEATURE;
  uref.report_id = reportID;
  uref.field_index = 0;
  uref.usage_index = 0;
  //  uref.usage_code = 0xff000001;
  ioctl(fd, HIDIOCGUCODE, &uref);
  if( 0 > ret) {
    perror("ioctl to get output report usage code");
    return ret;
  }

  for( i = 0; i < num_vals; i++){
      uref.usage_index = i;
      ioctl(fd, HIDIOCGUSAGE, &uref);
      usleep(delay);
      if(0 > ret){
          perror("ioctl to set usage value for out report");
          return ret;
      }
      vals[i] = (__u8)(uref.value & 0x000000FF) ;
  }
  return ret;
}


