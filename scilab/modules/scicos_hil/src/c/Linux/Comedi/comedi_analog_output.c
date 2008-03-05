// 31 Jan 2006 : Coding by Simone Mannori
//               basic examples from Comedi PDF and comedilib/demo/<sourcecode> 
//
// 4 Dec 2007 : update to Scilab 4.1.2
//
// Computational function for Comedi data acquisition cards 
// 

//** -------------------------- COMEDI ANALOG OUTPUT -------------------------------

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
    comedi_t *it      ; //** Comedi interface handler */
    
    int subdev ; //** Comedi subdevice
    int chan   ; //** channel selection
    int range  ; //** range selection 
    int aref   ; //** zero volt reference 
    lsampl_t maxdata  ;        /* maximum sample value (raw data) */
    comedi_range *range_ds   ; /* range */
  
  } ComediAnOut ;

void comedi_analog_output(scicos_block *block, int flag) 
{
  ComediAnOut *AOdata ; //** define the local datastructure
 
  lsampl_t data      ; //** local row dta input 
  double   volts     ; //* 
  double * u         ; //** Roberto

      
  switch(flag)
  {  
      case Init: //** Card and port init   
        *block->work = (ComediAnOut *) scicos_malloc(sizeof(ComediAnOut));
        if (*block->work  == NULL ) 
          { //** in case of error exit 
	    set_block_error(-16); 
            return; //** --> Exit point  
          }
	AOdata = (ComediAnOut *) *block->work; //** map the local strucutre in the scicos_block user data  
	   
	AOdata->it = comedi_open("/dev/comedi0"); /* */
        
        // code to check  
        if ( AOdata->it <= 0 )
          {
            fprintf(stderr, "Comedi Analog Output not found. \n");
            exit(1);
          }
        else
            printf("Comedi Analog Output found!\n"); 
       
        AOdata->subdev = 1 ; // analog channel output
        AOdata->chan   = block->ipar[0] ; // output channel selection
        AOdata->range  = block->ipar[1] ; // range selection
        AOdata->aref   = AREF_GROUND    ; // zero volt reference
        AOdata->maxdata  = comedi_get_maxdata(AOdata->it, AOdata->subdev, AOdata->chan);
        AOdata->range_ds = comedi_get_range  (AOdata->it, AOdata->subdev, AOdata->chan, AOdata->range);
        
        volts = 0.000 ; // zero on the output 
        data = comedi_from_phys(volts, AOdata->range_ds, AOdata->maxdata) ; // convert to integer data
        //** force the output to zero pysical volts 
        comedi_data_write(AOdata->it, AOdata->subdev, AOdata->chan, AOdata->range, AOdata->aref, data)  ;
      break; 
      
      case StateUpdate:
         AOdata = (ComediAnOut *) *block->work ;
         u = block->inptr[0] ; //** get the input from Scilab/Scicos 
         volts = u[0] ;       
   	 data = comedi_from_phys(volts, AOdata->range_ds, AOdata->maxdata) ;  
	 comedi_data_write(AOdata->it, AOdata->subdev, AOdata->chan, AOdata->range, AOdata->aref, data)  ;
      break;
      
      case Ending:
        AOdata = (ComediAnOut *) *block->work ;
        volts  = 0.000 ; // zero on the output 
        data = comedi_from_phys(volts, AOdata->range_ds, AOdata->maxdata) ;
        comedi_data_write(AOdata->it, AOdata->subdev, AOdata->chan, AOdata->range, AOdata->aref, data)  ;
        
	// comedi_close(it);
      
      break;	  
        
  } // close the switch
   
} // close the function 

