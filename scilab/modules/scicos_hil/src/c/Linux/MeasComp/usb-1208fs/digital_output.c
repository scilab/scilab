//**
// 24 Jan 2006 : Coding by Simone Mannori
//               basic examples from:  Warren Jasper <wjasper@tx.ncsu.edu>
//                                     Mike Erickson <merickson@nc.rr.com>
//**
//  6 Dec 2006 : Update for Scilab 4.1.2
//**

//** -------------------------- DIGITAL OUTPUT -------------------------------
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
    double threshold  ;
  } McUsbDigOut ;

// These two variables are used for the single bit handling 
// Storing the output state is possible to avoid the necessity to read the output register
// Read the output register is possible but is time consuming.
// Both output state variables are static and global because each blocks can modify the state
// of a single bit without changing the other output bits. 
static unsigned int PortAOutputState ; // State of the digital output port
static unsigned int PortBOutputState ;
  
  
void digital_output(scicos_block *block, int flag) 
{
  McUsbDigOut *DOdata ; //** define the local datastructure 
  
  __u8 input ;
  int nInterfaces = 0;
  
  unsigned int bitmask ; 
    
  double * u ; //** Scicos floating point input variable 

  switch(flag) {  
      
      case Init: //** Card and port init
        *block->work = (McUsbDigOut *) scicos_malloc(sizeof(McUsbDigOut));
        if (*block->work  == NULL ) 
          { //** in case of error exit 
	    set_block_error(-16); return; //** --> Exit point  
          }
	DOdata = (McUsbDigOut *) *block->work; //** map the local structure in the scicos_block user data  
       
        nInterfaces = PMD_Find(MCC_VID, USB1208FS_PID, DOdata->fd);
         if ( nInterfaces <= 0 ) {
               fprintf(stderr, "USB 1208FS not found.\n");
               exit(1);
         } else
               printf("USB 1208FS Device is found! Number of Interfaces = %d \n", nInterfaces);

	 DOdata->chan  = block->ipar[0] ; // channel selection  
         DOdata->portsel  = block->ipar[1] ; //** 0->Port A, 1->Port B 
	 DOdata->threshold  = block->rpar[0] ; //** threshold level 
         
	 if (DOdata->portsel == 0 )
	   {
	    PortAOutputState = 0x00; // State of the digital output port
	    usbDConfigPort_USB1208FS(DOdata->fd[0], DIO_PORTA, DIO_DIR_OUT);
	    usbDOut_USB1208FS(DOdata->fd[0], DIO_PORTA, (__u8)0x00); //** reset outputs Port A
	   }
	 if (DOdata->portsel == 1 ) 
	   {
	    PortBOutputState = 0 ;
	    usbDConfigPort_USB1208FS(DOdata->fd[0], DIO_PORTB, DIO_DIR_OUT);
	    usbDOut_USB1208FS(DOdata->fd[0], DIO_PORTB, (__u8)0x00); //** Reset outputs Port B
	   }
	 
      break; 
      
      case StateUpdate:
         DOdata = (McUsbDigOut *) *block->work;

         u = block->inptr[0] ; //** get the input from Scilab/Scicos      
	 
	 bitmask = 1 ;
	 
	 if (DOdata->portsel == 0) 
	   {
	     if ( u[0] > DOdata->threshold)
	        PortAOutputState = PortAOutputState | (bitmask << DOdata->chan)  ; // :)
	      else
	        PortAOutputState = PortAOutputState & ~(bitmask << DOdata->chan) ; // :( 

	     usbDOut_USB1208FS(DOdata->fd[0], DIO_PORTA, (__u8) PortAOutputState); //** outputs Port A
           }

	 if (DOdata->portsel == 1) 
	   {
	     if ( u[0] > DOdata->threshold)
	        PortBOutputState = PortBOutputState | (bitmask << DOdata->chan)  ; // :)
	       else
	        PortBOutputState = PortBOutputState & ~(bitmask << DOdata->chan) ; // :( 
              
	     usbDOut_USB1208FS(DOdata->fd[0], DIO_PORTB, (__u8) PortBOutputState); //** outputs Port B
           }
      break;
      
      case Ending:
          DOdata = (McUsbDigOut *) *block->work;
	  
	  if (DOdata->portsel == 0 )
	   {
	    PortAOutputState = 0x00; // State of the digital output port
	    usbDOut_USB1208FS(DOdata->fd[0], DIO_PORTA, (__u8)0x00); //** Reset outputs Port A
	   }
	  
	  if (DOdata->portsel == 1 ) 
	   {
	    PortBOutputState = 0x00; // State of the digital output port
	    usbDOut_USB1208FS(DOdata->fd[0], DIO_PORTB, (__u8)0x00); //** Reset outputs Port B
	   }
	   
	 close(DOdata->fd[0]); close(DOdata->fd[1]); close(DOdata->fd[2]); close(DOdata->fd[3]);
      
      break;	  
        
  } // close the switch
   
} // close the function 

