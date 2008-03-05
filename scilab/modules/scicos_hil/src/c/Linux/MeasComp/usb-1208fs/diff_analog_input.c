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
  typedef struct {
    int fd[4] ; //** internal USB datastructure 
    int chan   ; //** channel selection
    int range  ; //** range selection 
    int aref   ; //** zero volt reference 
  } McUsbAnIn ;

void diff_analog_input(scicos_block *block, int flag) 
{
  McUsbAnIn *AIdata ; //** define the local datastructure 
  
  int  nInterfaces = 0 ; //** if multiple USB interface are present 
  int  usb_flag        ; //** used as usb file handle 
  signed short svalue  ;
  __u8 input, channel, gain, options; //** non standard type 

  
  double   volts     ; //* 
  double * y         ; //** Roberto
  

  switch(flag) {  
      
      case Init: //** Card and port init
         *block->work = (McUsbAnIn *) scicos_malloc(sizeof(McUsbAnIn));
         if (*block->work  == NULL ) 
           { //** in case of error exit 
	     set_block_error(-16);
             return; //** --> Exit point  
           }

        AIdata = (McUsbAnIn *) *block->work; //** map the local structure in the scicos_block user data  
      
         nInterfaces = PMD_Find(MCC_VID, USB1208FS_PID, AIdata->fd);
         if ( nInterfaces <= 0 ) {
               fprintf(stderr, "USB 1208FS not found.\n");
               exit(1);
         } else
               printf("USB 1208FS Device is found! Number of Interface = %d \n", nInterfaces);

	 AIdata->chan   = block->ipar[0] ; // channel selection  
         AIdata->range  = block->ipar[1] ; // range selection 
  	 
	 //** Commented out because is too time comsuming for the Init of many blocks
         //** usbBlink_USB1208FS(AIdata->fd[0]); //** Blink the greeen led ;) 
     break; 
      
     case OutputUpdate:
          AIdata = (McUsbAnIn *) *block->work;
	  
	  channel = (__u8) AIdata->chan ;
          
	  gain = BP_20_00V;
	  switch( AIdata->range) {
	         case 0: 
		   gain = BP_20_00V;
	         break ;
		 
		 case 1:
	           gain = BP_10_00V;
	         break ;
		 
		 case 2:
                   gain = BP_5_00V;
		 break ;
                 
		 case 3:
	           gain = BP_4_00V;
		 break ;  
          
		 case 4:
	           gain = BP_2_50V;
                 break ;
		 
		 case 5:
	           gain = BP_2_00V;
                 break ;
		 
		 case 6:
	           gain = BP_1_25V;
                 break ;
		 
		 case 7:
	           gain = BP_1_00V;
                 break ;
	}	  
         
	  usb_flag = fcntl(fileno(stdin), F_GETFL);
          fcntl(0, F_SETFL, usb_flag | O_NONBLOCK);
	  
	  svalue = usbAIn_USB1208FS(AIdata->fd[0], channel, gain);
	  volts  = volts_FS(gain, svalue);
	  
          y = block->outptr[0] ;

          y[0] = volts ; 

      break;
      
      case Ending:
          AIdata = (McUsbAnIn *) *block->work;
	  close(AIdata->fd[0]); close(AIdata->fd[1]); close(AIdata->fd[2]); close(AIdata->fd[3]);
      break;	  
        
  } // close the switch
   
} // close the function 

