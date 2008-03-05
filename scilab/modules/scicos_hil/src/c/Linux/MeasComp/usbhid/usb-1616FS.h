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

#ifndef USB_1616FS_H
#define USB_1616FS_H

#define USB1616FS_VID (0x09DB)
#define USB1616FS_PID (0x0081)

#define DIO_DIR_IN  (0x01)
#define DIO_DIR_OUT (0x00)

/* Commands and HID Report ID for PMD 1608FS  */
#define DCONFIG          (0x01) // Configure digital port
#define DCONFIG_BIT      (0x02) // Configure individual digital port bits
#define DIN              (0x03) // Read digital port
#define DOUT             (0x04) // Write digital port
#define DBIT_IN          (0x05) // Read digital port bit
#define DBIT_OUT         (0x06) // Write digital port bit

#define AIN              (0x10) // Read analog input channel
#define AIN_SCAN         (0x11) // Scan analog channels
#define AIN_STOP         (0x12) // Stop input scan
#define ALOAD_QUEUE      (0x13) // Load the channel/gain queue

#define CINIT            (0x20) // Initialize counter
#define CIN              (0x21) // Read Counter

#define MEM_READ         (0x30) // Read Memory
#define MEM_WRITE        (0x31) // Write Memory

#define BLINK_LED        (0x40) // Causes LED to blink
#define RESET            (0x41) // Reset USB interface
#define SET_TRIGGER      (0x42) // Configure external trigger
#define SET_SYNC         (0x43) // Configure sync input/output
#define GET_STATUS       (0x44) // Get device status
#define SET_CAL          (0x45) // Set calibaration output

#define PREPARE_DOWNLOAD (0x50) // Prepare for program memory download
#define WRITE_CODE       (0x51) // Write program memory
#define READ_CHECKSUM    (0x52) // Return program memory checksum
#define WRITE_SERIAL     (0x53) // Write a new serial number to device
#define UPDATE_CODE      (0x54) // Update program memory and reset
#define READ_CODE        (0x55) // Read program memory

#define EXT_TRIG_FAILING_EDGE 0;
#define EXT_TRIG_RAISING_EDGE 1;
#define SYNC_MASTER 0
#define SYNC_SLAVE  1

// Gain Ranges
#define BP_10_00V  (0x0)           // Single Ended +/- 10.0 V
#define BP_5_00V   (0x1)           // Single Ended +/- 5.00 V
#define BP_2_50V   (0x2)           // Single Ended +/- 2.50 V
#define BP_2_00V   (0x3)           // Single Ended +/- 2.00 V
#define BP_1_25V   (0x4)           // Single Ended +/- 1.25 V
#define BP_1_00V   (0x5)           // Single Ended +/- 1.00 V
#define BP_0_625V  (0x6)           // Single Ended +/- 0.625 V
#define BP_0_3125V (0x7)           // Single Ended +/- 0.3125 V

// Option values for AInScan
#define AIN_EXECUTION     0x1  // 1 = single execution, 0 = continuous execution
#define AIN_BURST_MODE    0x2  // 1 = burst I/O mode,   0 = normal I/O mode
#define AIN_TRANSFER_MODE 0x4  // 1 = Immediate Transfer mode  0 = block transfer mode
#define AIN_TRIGGER       0x8  // 1 = Use External Trigger
#define AIN_EXTERN_SYNC   0x10 // 1 = Use External Sync
#define AIN_DEBUG_MODE    0x20 // 1 = debug mode

/* function prototypes for the PMD-1608FS */
void usbDConfigPort_USB1616FS(int fd, __u8 direction);
void usbDConfigBit_USB1616FS(int fd, __u8 bit_num, __u8 direction);
__u8 usbDIn_USB1616FS(int fd);
__u8 usbDInBit_USB1616FS(int fd, __u8 bit_num);
void usbDOut_USB1616FS(int fd, __u8 value);
void usbDOutBit_USB1616FS(int fd, __u8 bit_num, __u8 value);

signed short usbAIn_USB1616FS(int fd, __u8 channel, __u8 range);
void usbAOut_USB1616FS(int fd, __u8 channel, __u16 value);
void usbAOutStop_USB1616FS(int fd);
void usbAInStop_USB1616FS(int fd);
int usbAInScan_USB1616FS(int fd[], __u8 lowchannel, __u8 highchannel, __u32 count,
			  float *frequency, __u8 options, __s16 data[]);
void usbAInLoadQueue_USB1616FS(int fd, __u8 gains[8]);

void usbInitCounter_USB1616FS(int fd);
__u32 usbReadCounter_USB1616FS(int fd);

void usbReadMemory_USB1616FS( int fd, __u16 address, __u8 type, __u8 count, __u8* memory);
int usbWriteMemory_USB1616FS(int fd, __u16 address,  __u8 count, __u8 data[]);
int usbBlink_USB1616FS(int fd);
int usbReset_USB1616FS(int fd);
__u16 usbGetStatus_USB1616FS(int fd);
void usbSetTrigger_USB1616FS(int fd, __u8 type);
void usbSetSync_USB1616FS(int fd, __u8 type);
float volts_USB1616FS( const int gain, const signed short num );
void usbPrepareDownload_USB1616FS(int fd);
__u16 usbReadChecksum_USB1616FS(int fd);
void usbWriteSerial_USB1616FS(int fd, __u8 serial[8]);
void usbUpdateCode_USB1616FS(int fd);
int usbReadCode_USB1616FS(int fd, __u32 address, __u8 count, __u8 data[]);

#endif // USB-1616FS_H
