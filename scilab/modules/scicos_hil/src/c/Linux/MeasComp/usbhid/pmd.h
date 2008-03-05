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

/*
 * your kernel needs to be configured with /dev/usb/hiddev support
 * I think most distros are already
 * mikee@signalscape.com
 *
 */

#ifndef PMD_H
#define PMD_H

#define MAX_HIDDEV 128
#define CBUSB_SUCCESS 0;

#define MCC_VID (0x09DB)   // USB Vendor Code for Measurement Computing

/* For the LS devices */
int PMD_SendCommandLS(int fd, __u8* vals, int num_vals);
int PMD_RecvResponseLS(int fd, __u8 *vals, int num_vals);

/* For the PMD  devices */
int PMD_SendOutputReport(int fd, __u8 reportID, __u8* vals, int num_vals, int delay);
int PMD_GetInputReport(int fd, __u8 reportID, __u8 *vals, int num_vals, int delay);
int PMD_GetFeatureReport(int fd, __u8 reportID, __u8 *vals, int num_vals, int delay);
int PMD_Find(int vendor, int product, int fd[]);

#endif  //PMD_H
