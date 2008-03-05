// 24 Jan 2006 : Coding by Simone Mannori
//               basic examples from:  Warren Jasper <wjasper@tx.ncsu.edu>
//                                     Mike Erickson <merickson@nc.rr.com>
//**
//  6 Dec 2006 : Update for Scilab 4.1.2
//**
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <asm/types.h>
#include <math.h>

#include "scicos_block4.h"  /* Specific for scicos block developement */
#include "machine.h"


#include "pmd.h"           /* Specific for MC USB devices  */ 
#include "usb-1208FS.h"    /* Specific for the card model  */  

#define Init           4   /* Initialization */ 
#define OutputUpdate   1
#define StateUpdate    2
#define Integration    0
#define Mode           9    /* Zero Crossing */
#define EventScheduler 3
#define Ending         5   
 
 //** static structure inside block->work  
  typedef struct
   {
    int fd[4] ; //** internal USB datastructure 
    int chan   ; //** channel selection
   } McUsbAnOut ;

void analog_output(scicos_block *block, int flag) 
{
  McUsbAnOut *AOdata ; //** define the local datastructure 

  int nInterfaces = 0; 
  __u8  input, channel, gain, options; //** non standard type 
  __u16 value ;
  int usb_flag;

  double   volts     ; //* 
  double * u         ; //** Roberto
      
  switch(flag) {  
      
      case Init: //** Card and port init   
         *block->work = (McUsbAnOut *) scicos_malloc(sizeof(McUsbAnOut));
         if (*block->work  == NULL ) 
           { //** in case of error exit 
	     set_block_error(-16); return; //** --> Exit point  
           }
        AOdata = (McUsbAnOut *) *block->work; //** map the local strucutre in the scicos_block user data 
	 
	 nInterfaces = PMD_Find(MCC_VID, USB1208FS_PID, AOdata->fd);
         if ( nInterfaces <= 0 ) {
               fprintf(stderr, "USB 1208FS not found.\n");
               exit(1);
         } else
               printf("USB 1208FS Device is found! Number of Interfaces = %d \n", nInterfaces);

	 AOdata->chan  = block->ipar[0]  ; // channel selection  
   
	 channel = (__u8) AOdata->chan ; //** channel zero        
         value = (__u16)  0            ; //** range selection 0  ; //** usb dataformat 
 
	 usbAOut_USB1208FS(AOdata->fd[0], channel, value); //** zero on the output 0
         
	 //** Commented out because is too time comsuming for the Init of many blocks
         //** usbBlink_USB1208FS(AOdata->fd[0]); //** Blink the greeen led ;) 
      break; 
      
      case StateUpdate:
               AOdata = (McUsbAnOut *) *block->work;

               u = block->inptr[0] ; //** get the input from Scilab/Scicos

               volts = u[0] ; //** put in a variable 

	       if ( volts < 0 ) 
	               volts = 0 ; //** lower saturation  
	       if ( volts > 4.095 ) 
	               volts = 4.095 ; //** upper saturation        
	       value   = (__u16) ((volts * 4095.0) / 4.095) ; //** output normalized 0-4.095 Volt with usb dataformat
	       channel = (__u8) AOdata->chan ; //** channel selection 
	       usbAOut_USB1208FS(AOdata->fd[0], channel, value);
      break;
      
      case Ending:
               AOdata = (McUsbAnOut *) *block->work;
               value = (__u16)  0  ; //** usb dataformat 
	       channel = (__u8) 0  ; //** channel zero 
	       usbAOut_USB1208FS(AOdata->fd[0], channel, value); //** zero on the output 
	       close(AOdata->fd[0]); close(AOdata->fd[1]); close(AOdata->fd[2]); close(AOdata->fd[3]);
      break;	  
        
  } // close the switch
   
} // close the function 

