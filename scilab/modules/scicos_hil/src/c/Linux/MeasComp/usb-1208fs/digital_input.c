//**
// 24 Jan 2006 : Coding by Simone Mannori
//               basic examples from:  Warren Jasper <wjasper@tx.ncsu.edu>
//                                     Mike Erickson <merickson@nc.rr.com>
//**
//  6 Dec 2006 : Update for Scilab 4.1.2
//**

//** -------------------------- DIGITAL INPUT -------------------------------
// 
// The two (Port A, Port B) digital I/O are bit addressable but each port MUST BE
// configured as ALL outputs OR inputs: no mixed input/output bit on the same port
// are allowed (see “usb-1208FS.c” and “usb-1208FS.h” for the details). Please group
// all the inputs on a port and all the outputs on the other port. Two output port 
// and two input ports configurations are allowed.
//

 
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
  typedef struct {
    int fd[4] ; //** internal USB datastructure 
    int chan    ; //** channel selection
    int portsel ; //** 0-> Port A, 1->Port B 
    int portdir ; //** 0->ALL Inputs, 1->ALL Outputs  
  } McUsbDigIn ;

void digital_input(scicos_block *block, int flag) 
{
  McUsbDigIn *DIdata ; //** define the local datastructure 
  
  __u8 input ;
  int nInterfaces = 0; 
  
  unsigned int bitmask   ;
  unsigned int portinput ; 
  
  double   d_data    ; //* 
  double * y         ; //** Roberto 

    
  switch(flag) {  
      
      case Init: //** Card and port init   
       *block->work = (McUsbDigIn *) scicos_malloc(sizeof(McUsbDigIn));
        if (*block->work  == NULL ) 
          { //** in case of error exit 
	    set_block_error(-16); return; //** --> Exit point  
          }
	DIdata = (McUsbDigIn *) *block->work; //** map the local structure in the scicos_block user data  
       
        nInterfaces = PMD_Find(MCC_VID, USB1208FS_PID, DIdata->fd);
         if ( nInterfaces <= 0 ) {
               fprintf(stderr, "USB 1208FS not found.\n");
               exit(1);
         } else
               printf("USB 1208FS Device is found! Number of Interfaces = %d \n", nInterfaces);

	 DIdata->chan  = block->ipar[0] ; // channel selection  
         DIdata->portsel  = block->ipar[1] ; //** 0->Port A, 1->Port B 
	 DIdata->portdir  = block->ipar[2] ; //** 0->ALL Inputs, 1->ALL Outputs  
	 
	 if (DIdata->portdir == 0 )
	   {
	    if (DIdata->portsel == 0 ) 
	      usbDConfigPort_USB1208FS(DIdata->fd[0], DIO_PORTA, DIO_DIR_IN);
	    if (DIdata->portsel == 1 ) 
	      usbDConfigPort_USB1208FS(DIdata->fd[0], DIO_PORTB, DIO_DIR_IN);
	  }
	 
	 //** Commented out because is too time comsuming for the Init of many blocks
	 //** usbBlink_USB1208FS(DIdata->fd[0]); //** Blink the greeen led ;) 
       break; 
      
      case OutputUpdate:
        DIdata = (McUsbDigIn *) *block->work;
	
	if (DIdata->portsel == 0 )
	  {
	           usbDIn_USB1208FS(DIdata->fd[0], DIO_PORTA, &input);
	  }
	
	if (DIdata->portsel == 1 )
	  {
	           usbDIn_USB1208FS(DIdata->fd[0], DIO_PORTB, &input);
	  }
	  
	bitmask = 1   ;
	bitmask = bitmask << DIdata->chan ;
	// printf("chan = %d \n", DIdata->chan ); //** debug
	// printf("Port = %d \n", DIdata->portsel ); //** debug 
	// printf("bitmask = %d \n", bitmask); //** debug 
	portinput = (unsigned int) input ;
	portinput = portinput & bitmask    ;
	
	if (portinput == 0 )
	    d_data  = (double) 0.0 ; //** low state 
	else
	    d_data  = (double) 1.0 ; //** high state 

        y = block->outptr[0] ; //** acquire the pointer 
	y[0] = d_data ;        //** update the pointer 

      break;
      
      case Ending:
           DIdata = (McUsbDigIn *) *block->work;
           close(DIdata->fd[0]); close(DIdata->fd[1]); close(DIdata->fd[2]); close(DIdata->fd[3]);
      break;	  
        
  } // close the switch
   
} // close the function 

