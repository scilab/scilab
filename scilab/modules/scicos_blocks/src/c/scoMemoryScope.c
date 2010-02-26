/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
/**
   \file scoMemoryScope.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source Code of all functions interacting with the memory of the computer for the ScopeMemory structure
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "scoBase.h"
#include "scoMisc.h"
#include "scoMemoryScope.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "sciprint.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
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
/*--------------------------------------------------------------------------*/ 
//** 15/16 May 2008
/* Some explanations about this garbled code: 
- if you stop a Scicos simulation (intentionally or not) and 
- you delete one or more scope windows and
- you try to "restart" or "end" the simulation
- you force the "ending" section of each Scicos blocks, including scopes
- inside the "ending" section of each Scicos scope there is a call to this
very function "scoFreeScopeMemory" that free some data structure

BUT

- if you have ALREADY close manually the windows, some of them are aready 
"freed" 

THEN

- you need to check before try to free the resources 

For the moment we (Benoit, Simone, JB) have decided to leave some commented
debug code.  

*/

void scoFreeScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory)
{
	int i;

	if (*pScopeMemory != NULL) 
	{
		//**printf("BANZAI \n");

		//** if ( (*pScopeMemory)->new_draw != NULL ) 
		//**  printf("[ 1 ] \n");
		scicos_free((*pScopeMemory)->new_draw);

		//** if ( (*pScopeMemory)->number_of_curves_by_subwin != NULL )
		//**  printf("[ 2 ] \n");
		scicos_free((*pScopeMemory)->number_of_curves_by_subwin);

		//** if ( (*pScopeMemory)->period_counter != NULL )
		//**  printf("[ 3 ] \n");
		scicos_free((*pScopeMemory)->period_counter);

		//** if ( (*pScopeMemory)->period != NULL)
		//**  printf("[ 4 ] \n");
		scicos_free((*pScopeMemory)->period);

		//** if ( (*pScopeMemory)->longdraw_size != NULL )
		//**  printf("[ 5 ] \n");
		scicos_free((*pScopeMemory)->longdraw_size);

		//** if ( (*pScopeMemory)->shortdraw_size != NULL )
		//**  printf("[ 6 ] \n");
		scicos_free((*pScopeMemory)->shortdraw_size);

		if ( (*pScopeMemory)->number_of_subwin != NULL )
		{
			for (i = 0; i < (*pScopeMemory)->number_of_subwin ; i++) 
			{
				//** if ( (*pScopeMemory)->hShortDraw[i] != NULL )
				scicos_free((*pScopeMemory)->hShortDraw[i]);
				//** if ( (*pScopeMemory)->hLongDraw[i] != NULL)
				scicos_free((*pScopeMemory)->hLongDraw[i]);
			}
		}

		//** if ( (*pScopeMemory)->hShortDraw != NULL)
		//**  printf("[ 7 ] \n");
		scicos_free((*pScopeMemory)->hShortDraw);

		//** if ( (*pScopeMemory)->hLongDraw != NULL)
		//**  printf("[ 8 ] \n");
		scicos_free((*pScopeMemory)->hLongDraw);

		//** if ( (*pScopeMemory)->hAxes != NULL)
		//**  printf("[ 9 ] \n");
		scicos_free((*pScopeMemory)->hAxes);

		//** if ( *block_work != NULL )
		//**  printf("[ 10 ] \n");
		scicos_free(*block_work);

		//**  printf("[ END ] \n");
	}

}
/*--------------------------------------------------------------------------*/ 
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
		sciprint(_("SCOPE ERROR : Error in scoReallocLongDraw()"));
		break;
	}
}
/*--------------------------------------------------------------------------*/ 
