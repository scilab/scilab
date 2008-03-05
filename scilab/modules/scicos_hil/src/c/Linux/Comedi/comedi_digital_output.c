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
    comedi_t *it      ; //** Comedi interface handler */
    
    int subdev ; //** Comedi subdevice
    int chan   ; //** channel selection
  
    double threshold  ;
  
  } ComediDigOut ;


void comedi_digital_output(scicos_block *block, int flag) 
{
  ComediDigOut *DOdata ; //** define the local datastructure 
  
  double * u ; //** Scicos floating point input variable 
  unsigned int data   ; /*  */
     
  switch(flag) 
  {  
      
      case Init: //** Card and port init
        *block->work = (ComediDigOut *) scicos_malloc(sizeof(ComediDigOut));
         if (*block->work  == NULL ) 
           { //** in case of error exit 
	     set_block_error(-16);
             return; //** --> Exit point  
           }
	 DOdata = (ComediDigOut *) *block->work; //** map the local strucutre in the scicos_block user data  
              
         DOdata->it = comedi_open("/dev/comedi0"); /* */
         // code to check  
         if ( DOdata->it <= 0 ) {
            fprintf(stderr, "Comedi Digital Output not found. \n");
            exit(1);
          } else
            printf("Comedi Digital Output found!\n"); 
       
         DOdata->subdev = 2 ; // digital 
         DOdata->chan    = block->ipar[0] ; 
         DOdata->threshold = block->rpar[0] ; //
       
         comedi_dio_config(DOdata->it, DOdata->subdev , DOdata->chan, COMEDI_OUTPUT);
         data = 0 ;
         comedi_dio_write(DOdata->it, DOdata->subdev, DOdata->chan, data)  ; /* clear the output */
      break; 
      
      case StateUpdate:
         DOdata = (ComediDigOut *) *block->work ;
         
         u = block->inptr[0] ; //** get the input from Scilab/Scicos
                                      
	 if ( u[0] >  DOdata->threshold)
	   data = 1 ; // :)
	 else
	   data = 0 ; // :(         
               
	 comedi_dio_write(DOdata->it, DOdata->subdev, DOdata->chan, data)  ;

      break;
      
      case Ending:
        DOdata = (ComediDigOut *) *block->work ;
	data = 0 ; //** low output 
	comedi_dio_config(DOdata->it, DOdata->subdev, DOdata->chan, COMEDI_OUTPUT);
        comedi_dio_write(DOdata->it, DOdata->subdev, DOdata->chan, data)  ;
        
	// comedi_close(it);
      
      break;	  
        
  } // close the switch
   
} // close the function 

