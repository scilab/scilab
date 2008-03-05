// 31 Jan 2006 : Coding by Simone Mannori
//               basic examples from Comedi PDF and comedilib/demo/<sourcecode> 
//
// 4 Dec 2007 : update to Scilab 4.1.2
//
// Computational function for Comedi data acquisition cards 
// 

//** -------------------------- COMEDI ANALOG INPUT --------------------------------------

//**  General includes 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <asm/types.h>
#include <math.h>

/* Specific for scicos block developement */
#include "scicos_block4.h"  
#include "machine.h"

/* Specific for Comedilib */
#include <comedilib.h>

#define Init           4   
#define OutputUpdate   1
#define StateUpdate    2
#define Integration    0
#define Mode           9   
#define EventScheduler 3
#define Ending         5     

//** Comedi static structure inside block->work  
  typedef struct
  {
    int comedi_device ; //** 
    comedi_t *it      ; //** Comedi interface handler*/
    
    int subdev ; //** Comedi subdevice
    int chan   ; //** channel selection
  
  } ComediDigIn ;

void comedi_digital_input(scicos_block *block, int flag) 
{
  ComediDigIn *DIdata ; //** define the local datastructure 
  
  lsampl_t data      ; //** local row data input 
  double   d_data    ; //* 
  double * y         ; //** Roberto
      
  switch(flag)
  {  
      
      case Init: //** Card and port init   
        
        *block->work = (ComediDigIn *) scicos_malloc(sizeof(ComediDigIn));
        if (*block->work  == NULL ) 
          { //** in case of error exit 
	    set_block_error(-16);
            return; //** --> Exit point  
          }
	DIdata = (ComediDigIn *) *block->work; //** map the local strucutre in the scicos_block user data  
       
        DIdata->it = comedi_open("/dev/comedi0"); /* */
        // code to check  
        if ( DIdata->it <= 0 )
          {
            fprintf(stderr, "Comedi Digital Input not found. \n");
            exit(1);
          } else
         printf("Comedi Digital Input found!\n"); 
       
        DIdata->subdev = 2 ; // digital channel I/O
        DIdata->chan   = block->ipar[0] ; 
        comedi_dio_config(DIdata->it, DIdata->subdev, DIdata->chan, COMEDI_INPUT); //** configure as input 
       break; 
      
      case OutputUpdate:
        DIdata = (ComediDigIn *) *block->work ;
        comedi_dio_read( DIdata->it, DIdata->subdev, DIdata->chan, &data);
        d_data = (double) data;
	y = block->outptr[0] ;
        y[0] = d_data ;  
      break;
      
      case Ending:
          // comedi_close(it);
      break;	  
        
  } // close the switch
   
} // close the function 

