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


//** ==================================
void roby_getstr(char * str, int par[], int init, int len)
{
  int i     ;
  int j = 0 ;

  for(i=init; i<init+len; i++)
    str[j++] = (char) par[i];

  str[j]='\0';
}
//** =====================================


 //** Comedi static structure inside block->work  
  typedef struct
  {
    char ComediDevName[20]; //** 
    comedi_t *it      ; //** Comedi interface handler*/
    
    int subdev ; //** Comedi subdevice
    int chan   ; //** channel selection
    int range  ; //** range selection 
    int aref   ; //** zero volt reference 
    lsampl_t maxdata  ;        /* maximum sample value (raw data) */
    comedi_range *range_ds   ; /* range */
  
  } ComediAnIn ;

//** Main Scicos Block 
void comedi_analog_input(scicos_block *block, int flag) 
{
  ComediAnIn *AIdata ; //** define the local datastructure 
  char sName[15]     ;
  int NameLen ;
  
  lsampl_t data      ; //** local row data input 
  double   volts     ; //* 
  double * y         ; //** Roberto

  switch (flag)
  {  
      case Init: //** Card and port init   
        *block->work = (ComediAnIn *) scicos_malloc(sizeof(ComediAnIn));
        if (*block->work  == NULL ) 
          { //** in case of error exit 
	   set_block_error(-16); //** magic code 
           return; //** --> Exit point  
          }

        AIdata = (ComediAnIn *) *block->work; //** map the local structure in the "scicos_block" user data  
             
        NameLen = (int) block->ipar[3] ;
        fprintf(stderr, "NameLen %d \n", NameLen);
	
	roby_getstr(sName, block->ipar, 4, NameLen);   //** get the "comedi0" string 
	fprintf(stderr, "sName %s \n", sName);
	
	sprintf(AIdata->ComediDevName,"/dev/%s",sName); //** create the string 
	fprintf(stderr, "ComediDevName: %s \n", AIdata->ComediDevName);
	
	AIdata->it = comedi_open(AIdata->ComediDevName); /* */
        if (!(AIdata->it))
          {
            fprintf(stderr, "Comedi Analog Input at %s not found. \n", AIdata->ComediDevName);
            exit(1);
          }
        else
            printf("Comedi AnalogInput found! \n"); 
       
        AIdata->subdev = 0 ; // analog channels input
        AIdata->chan   = block->ipar[0] ; // channel selection  
        AIdata->range  = block->ipar[1] ; // range selection 
        AIdata->aref = AREF_GROUND      ; // zero volt reference 
        AIdata->maxdata =   comedi_get_maxdata(AIdata->it, AIdata->subdev, AIdata->chan);
        AIdata->range_ds =  comedi_get_range  (AIdata->it, AIdata->subdev, AIdata->chan, AIdata->range);
 
       break; 
      
      case OutputUpdate:
        AIdata = (ComediAnIn *) *block->work;

	comedi_data_read(AIdata->it, AIdata->subdev, AIdata->chan, AIdata->range, AIdata->aref, &data);

        volts = comedi_to_phys(data, AIdata->range_ds, AIdata->maxdata);

        //** printf(" datain = %f \n", volts) ; //** DEBUG only 

        y = block->outptr[0] ; 

        y[0] = volts ; 

     
      break;
      
      case Ending:
      
      // comedi_close(it);
      
      break;	  
        
  } // close the switch
   
} // close the function 

