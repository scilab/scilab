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


#ifndef USB_1024LS_H
#define USB_1024LS_H

#define USB1024LS_PID  (0x0076)
#define USB1024HLS_PID (0x007F)

#define DIO_PORTA (0x01)
#define DIO_PORTB (0x04)
#define DIO_PORTC_LOW (0x08)
#define DIO_PORTC_HI  (0x02)

#define DIO_DIR_IN (0x01)
#define DIO_DIR_OUT (0x00)


/* Commands and Codes for USB 1024-LS HID reports */
#define DCONFIG     (0x0D)     // Configure digital port
#define DIN         (0x00)     // Read digital port
#define DOUT        (0x01)     // Write digital port
#define DBIT_IN     (0x02)     // Read Digital port bit
#define DBIT_OUT    (0x03)     // Write Digital port bit

#define CINIT       (0x05)     // Initialize counter
#define CIN         (0x04)     // Read Counter

#define MEM_READ    (0x09)     // Read Memory
#define MEM_WRITE   (0x0A)     // Write Memory

#define BLINK_LED   (0x0B)     // Causes LED to blink
#define RESET       (0x11)     // Reset USB interface
#define SET_ID      (0x0C)     // Set the user ID
#define GET_ID      (0x0F)     // Get the user ID


/* function prototypes for the USB-1024LS */
void usbDConfigPort_USB1024LS(int fd, __u8 port, __u8 direction);
__u8 usbDIn_USB1024LS(int fd, __u8 port);
void usbDOut_USB1024LS(int fd, __u8 port, __u8 value);
__u8 usbDBitIn_USB1024LS(int fd, __u8 port, __u8 bit);
void usbDBitOut_USB1024LS(int fd, __u8 port, __u8 bit, __u8 value);
void usbInitCounter_USB1024LS(int fd);
__u32 usbReadCounter_USB1024LS(int fd);
void usbReadMemory_USB1024LS(int fd, __u16 address, __u8 *data, __u8 count);
int usbBlink_USB1024LS(int fd);
int usbReset_USB1024LS(int fd);
__u8 usbGetID_USB1024LS(int fd);
void usbSetID_USB1024LS(int fd, __u8 id);


#endif //USB_1024LS_H
