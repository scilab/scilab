/**
   \file scoMemoryScope.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source Code of all functions interacting with the memory of the computer for the ScopeMemory structure
*/

#include "scoBase.h"
#include "scoMisc.h"
#include "scoMemoryScope.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "stdio.h"

/** This function use scicos_malloc on the whole structure and set up 0 for whole properties*/
void scoInitScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory, int number_of_subwin, int * number_of_curves_by_subwin)
{
  int i,j;
  *block_work = (ScopeMemory*)scicos_malloc(sizeof(ScopeMemory));
  if(*block_work == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  *pScopeMemory = (ScopeMemory*) *block_work;

  scoSetHandleScopeWindow((*pScopeMemory),0);

  scoSetNumberOfSubwin((*pScopeMemory),number_of_subwin);

  (*pScopeMemory)->number_of_curves_by_subwin = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->number_of_curves_by_subwin == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  for(i=0 ; i < number_of_subwin ; i++)
    {
      scoSetNumberOfCurvesBySubwin((*pScopeMemory),i,number_of_curves_by_subwin[i]);
    }

  (*pScopeMemory)->new_draw = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->new_draw == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetNewDraw(*pScopeMemory,i,0);
    }
 
  (*pScopeMemory)->shortdraw_size = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->shortdraw_size == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetShortDrawSize(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->period = (scoLineOfDouble)scicos_malloc(number_of_subwin*sizeof(scoDouble));
  if((*pScopeMemory)->period == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetPeriod(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->period_counter = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->period_counter == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetPeriodCounter(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->longdraw_size = (scoLineOfInteger)scicos_malloc(number_of_subwin*sizeof(scoInteger));
  if((*pScopeMemory)->longdraw_size == NULL)
    {
      scoScopeError(*pScopeMemory,1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetLongDrawSize(*pScopeMemory,i,0);
    }

  (*pScopeMemory)->hAxes = (scoLineOfLongInteger)scicos_malloc(number_of_subwin*sizeof(scoLongInteger));
  if((*pScopeMemory)->hAxes == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }
  for(i=0; i < number_of_subwin ; i++)
    {
      scoSetHandleAxes(*pScopeMemory,i,0);
    }


  (*pScopeMemory)->hShortDraw = (scoMatrixOfLongInteger)scicos_malloc(number_of_subwin*sizeof(scoLineOfLongInteger));
  if((*pScopeMemory)->hShortDraw == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }

  (*pScopeMemory)->hLongDraw = (scoMatrixOfLongInteger)scicos_malloc(number_of_subwin*sizeof(scoLineOfLongInteger));
  if((*pScopeMemory)->hLongDraw == NULL)
    {
      scoScopeError(*pScopeMemory, 1);
    }

  for(i = 0 ; i < number_of_subwin ; i++)
    {
      (*pScopeMemory)->hShortDraw[i] = (scoLineOfLongInteger)scicos_malloc(number_of_curves_by_subwin[i]*sizeof(scoLongInteger));
      if((*pScopeMemory)->hShortDraw[i] == NULL)
	{
	  scoScopeError(*pScopeMemory, 1);
	}
      for(j=0; j < number_of_curves_by_subwin[i] ; j++)
	{
	  scoSetHandleShortDraw(*pScopeMemory,i,j,0);
	}
  
      (*pScopeMemory)->hLongDraw[i]  = (scoLineOfLongInteger)scicos_malloc(number_of_curves_by_subwin[i]*sizeof(scoLongInteger));
      if((*pScopeMemory)->hLongDraw[i] == NULL)
	{
	  scoScopeError(*pScopeMemory, 1);
	}
      for(j=0; j < number_of_curves_by_subwin[i] ; j++)
	{
	  scoSetHandleLongDraw(*pScopeMemory,i,j,0);
	}
    }
  scoSetScopeActivation(*pScopeMemory,0); //Not activated by default
}


void scoFreeScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory)
{
  int i;

  if (*pScopeMemory!=NULL) {
    scicos_free((*pScopeMemory)->new_draw);
    scicos_free((*pScopeMemory)->number_of_curves_by_subwin);
    scicos_free((*pScopeMemory)->period_counter);
    scicos_free((*pScopeMemory)->period);
    scicos_free((*pScopeMemory)->longdraw_size);
    scicos_free((*pScopeMemory)->shortdraw_size);
    for(i = 0; i < (*pScopeMemory)->number_of_subwin ; i++) {
      scicos_free((*pScopeMemory)->hShortDraw[i]);
      scicos_free((*pScopeMemory)->hLongDraw[i]);
    }
    scicos_free((*pScopeMemory)->hShortDraw);
    scicos_free((*pScopeMemory)->hLongDraw);
    scicos_free((*pScopeMemory)->hAxes);
    scicos_free(*block_work);
  }

}

void scoRetrieveScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory)
{
  *pScopeMemory = (ScopeMemory*)*block_work;
}

/** If I have a longdraw of size 200, the next buffer size is 20 and to be sure I add 5000 more points so I ve got a 7020 pts longdraw at the end. Watch out the function use REALLOC there is no scicos_realloc() existing in scicos*/
void scoReallocLongDraw(scoGraphicalObject pLongDraw, int NbrPtsLong, int shortdraw_size, int plus)
{
  switch(sciGetEntityType(pLongDraw))
    {
    case SCI_POLYLINE:
      pPOLYLINE_FEATURE(pLongDraw)->pvx = REALLOC((pPOLYLINE_FEATURE(pLongDraw)->pvx),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      pPOLYLINE_FEATURE(pLongDraw)->pvy = REALLOC((pPOLYLINE_FEATURE(pLongDraw)->pvy),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      //If we are in 2D mode in the window at init then pvz is not initialized
      if(pPOLYLINE_FEATURE(pLongDraw)->pvz != NULL)
	{
	  pPOLYLINE_FEATURE(pLongDraw)->pvz = REALLOC((pPOLYLINE_FEATURE(pLongDraw)->pvz),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
	}
      break;
    case SCI_SEGS:
      pSEGS_FEATURE(pLongDraw)->vx = REALLOC((pSEGS_FEATURE(pLongDraw)->vx),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      pSEGS_FEATURE(pLongDraw)->vy = REALLOC((pSEGS_FEATURE(pLongDraw)->vy),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
      //If we are in 2D mode in the window at init then vz is not initialized
      if(pSEGS_FEATURE(pLongDraw)->vz != NULL)
	{
	  pSEGS_FEATURE(pLongDraw)->vz = REALLOC((pSEGS_FEATURE(pLongDraw)->vz),(NbrPtsLong+shortdraw_size+plus)*sizeof(double));
	}
      break;
    default:
      sciprint("SCOPE ERROR : Error in scoReallocLongDraw()");
      break;
    }
}
