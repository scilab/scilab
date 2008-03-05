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


#ifndef USB_1208LS_H
#define USB_1208LS_H

#define USB1208LS_PID (0x007A)

#define DIO_PORTA     (0x01)
#define DIO_PORTB     (0x04)
#define DIO_AUXPORT   (0x10)

#define DIO_DIR_IN  (0x01)
#define DIO_DIR_OUT (0x00)

#define MINILAB_CLOCK (6000000L)  // 6 MHz clock
#define LS_DELAY (20*1000)

/* Commands and Codes for USB 1208-LS HID reports */
#define DCONFIG     (0x0D)     // Configure digital port
#define DIN         (0x00)     // Read digital port
#define DOUT        (0x01)     // Write digital port
#define DBIT_IN     (0x02)     // Read digital port bit
#define DBIT_OUT    (0x03)     // Write digital port bit

#define AIN         (0x06)     // Read analog input channel
#define AIN_SCAN    (0x0E)     // Scan analog channels
#define AIN_STOP    (0x10)     // Stop scan
#define ALOAD_QUEUE (0x07)     // Load the channel/gain queue

#define AOUT        (0x08)     // Write analog output channel

#define CINIT       (0x05)     // Initialize counter
#define CIN         (0x04)     // Read Counter

#define MEM_READ    (0x09)     // Read Memory
#define MEM_WRITE   (0x0A)     // Write Memory

#define BLINK_LED   (0x0B)     // Causes LED to blink
#define RESET       (0x11)     // Reset USB interface
#define SET_TRIGGER (0x14)     // Configure external trigger
#define SET_ID      (0x0C)     // Set the user ID
#define GET_ID      (0x0F)     // Get the user ID

#define OFFSET_ADJUSTMENT  (0x1F00)   // Offset Adjustment for the A/D        0x1F00 - 0x1F4F
#define SE_GAIN_ADJUSTMENT (0x1F50)   // Single Ended Gain Adjustment for A/D 0x1F50 - 0x1F5F
#define DE_GAIN_ADJUSTMENT (0x1F60)   // Differential Gain Adjustment for A/D 0x1F60 - 0x1F67
#define CAL_PIN_VOLTAGE    (0x1FA0)   // Calibration pin voltage              0x1FA0 - 0x1FA3

#define EXT_TRIG_FAILING_EDGE 0;
#define EXT_TRIG_RAISING_EDGE 1;

// Gain Ranges

#define SE_10_00V  (0x8)           // Single Ended +/- 10.0 V

#define BP_20_00V  (0x00)           // Differential +/- 20.0 V
#define BP_10_00V  (0x10)           // Differential +/- 10.0 V
#define BP_5_00V   (0x20)           // Differential +/- 5.00 V
#define BP_4_00V   (0x30)           // Differential +/- 4.00 V
#define BP_2_50V   (0x40)           // Differential +/- 2.50 V
#define BP_2_00V   (0x50)           // Differential +/- 2.00 V
#define BP_1_25V   (0x60)           // Differential +/- 1.25 V
#define BP_1_00V   (0x70)           // Differential +/- 1.00 V
#define SE_10_00V  (0x8)            // Single Ended +/- 10.0 V

// Option values for AInScan
#define AIN_EXECUTION     0x1  // 1 = single execution, 0 = continuous execution
#define AIN_BURST_MODE    0x2  // 1 = Burst Mode
#define AIN_TRANSFER      0x4  // 1 = Block Transfer Mode
#define AIN_TRIGGER       0x8  // 1 = Use External Trigger

/* function prototypes for the USB-1208LS */
void usbDConfigPort_USB1208LS(int fd, __u8 port, __u8 direction);
void usbDIn_USB1208LS(int fd, __u8 port, __u8* din_value);
void usbDOut_USB1208LS(int fd, __u8 port, __u8 value);
__u8 usbDBitIn_USB1208LS(int fd, __u8 port, __u8 bit);
void usbDBitOut_USB1208LS(int fd, __u8 port, __u8 bit, __u8 value);
signed short usbAIn_USB1208LS(int fd, __u8 channel, __u8 range);
void usbAInScan_USB1208LS(int fd, __u16 count, int rate, __u8 low_channel, __u8 high_channel, __u8 options, __s16 value[], __u8 gainLoadQueue[]);
void usbAInLoadQueue_USB1208LS(int fd, __u8 chanCount, __u8 chanLoadQueue[], __u8 gainLoadQueue[]);
void usbAOut_USB1208LS(int fd, __u8 channel, __u16 value);
void usbAInStop_USB1208LS(int fd);
void usbInitCounter_USB1208LS(int fd);
__u32 usbReadCounter_USB1208LS(int fd);
void usbReadMemory_USB1208LS(int fd, __u16 address, __u8 *data, __u8 count);
int usbBlink_USB1208LS(int fd);
int usbReset_USB1208LS(int fd);
void usbSetTrigger_USB1208LS(int fd, __u8 type, __u8 chan);
__u8 usbGetID_USB1208LS(int fd);
void usbSetID_USB1208LS(int fd, __u8 id);
float volts_LS(const int gain, const signed short num);


#endif //USB_1208LS_H
