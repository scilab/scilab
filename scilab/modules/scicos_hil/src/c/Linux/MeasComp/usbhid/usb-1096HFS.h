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


#ifndef USB_1096HFS_H
#define USB_1096HFS_H

#define USB1096HFS_VID  (0x09DB)
#define USB1096HFS_PID  (0x0083)

#define DIO_PORT0A     (0x0)
#define DIO_PORT0B     (0x1)
#define DIO_PORT0C_LOW (0x2)
#define DIO_PORT0C_HI  (0x3)
#define DIO_PORT1A     (0x4)
#define DIO_PORT1B     (0x5)
#define DIO_PORT1C_LOW (0x6)
#define DIO_PORT1C_HI  (0x7)
#define DIO_PORT2A     (0x8)
#define DIO_PORT2B     (0x9)
#define DIO_PORT2C_LOW (0xa)
#define DIO_PORT2C_HI  (0xb)
#define DIO_PORT3A     (0xc)
#define DIO_PORT3B     (0xd)
#define DIO_PORT3C_LOW (0xe)
#define DIO_PORT3C_HI  (0xf)

#define DIO_DIR_OUT (0x00)
#define DIO_DIR_IN (0x01)


/* Commands and Codes for USB 1096-HFS HID reports */
#define DCONFIG          (0x01) // Configure digital port
#define DIN              (0x03) // Read digital port
#define DOUT             (0x04) // Write digital port
#define DBIT_IN          (0x05) // Read Digital port bit
#define DBIT_OUT         (0x06) // Write Digital port bit

#define CINIT            (0x20) // Initialize counter
#define CIN              (0x21) // Read Counter

#define MEM_READ         (0x30) // Read Memory
#define MEM_WRITE        (0x31) // Write Memory

#define BLINK_LED        (0x40) // Causes LED to blink
#define RESET            (0x41) // Reset USB interface
#define GET_STATUS       (0x44) // Retrieve device status
#define GET_ALL          (0x46) // Retrieve all digital input values

#define PREPARE_DOWNLOAD (0x50) // Prepare for program memory download
#define WRITE_CODE       (0x51) // Write program memory
#define WRITE_SERIAL     (0x53) // Write new serial number to device
#define READ_CODE        (0x55) // Read program memory


/* function prototypes for the USB-1096HFS */
void usbDConfigPort_USB1096HFS(int fd, __u8 port, __u8 direction);
__u8 usbDIn_USB1096HFS(int fd, __u8 port);
void usbDOut_USB1096HFS(int fd, __u8 port, __u8 value);
__u8 usbDBitIn_USB1096HFS(int fd, __u8 port, __u8 bit);
void usbDBitOut_USB1096HFS(int fd, __u8 port, __u8 bit, __u8 value);
void usbInitCounter_USB1096HFS(int fd);
__u32 usbReadCounter_USB1096HFS(int fd);
void usbReadMemory_USB1096HFS(int fd, __u16 address, __u8 count, __u8* memory);
int usbWriteMemory_USB1096HFS(int fd, __u16 address, __u8 count, __u8* data);
int usbBlink_USB1096HFS(int fd);
int usbReset_USB1096HFS(int fd);
__u16 usbGetStatus_USB1096HFS(int fd);
void usbGetAll_USB1096HFS(int fd, __u8 data[]);

void usbPrepareDownload_USB1096HFS(int fd);
void usbWriteSerial_USB1096HFS(int fd, __u8 serial[8]);
void usbWriteCode_USB1096HFS(int fd, __u32 address, __u8 count, __u8 data[]);
int usbReadCode_USB1096HFS(int fd, __u32 address, __u8 count, __u8 data[]);

#endif //USB_1096HFS_H
