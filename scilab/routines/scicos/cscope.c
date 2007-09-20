/**
   \file cscope.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CSCOPE is a typical scope which links its input to the simulation time but there is only one input instead of CMSCOPE
   \see CSCOPE.sci in macros/scicos_blocks/Sinks/
*/

#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"

/** \fn cscope_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void cscope_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  int i;
  double *rpar;
  int *ipar, nipar;
  double period;
  int dimension;
  double ymin, ymax, xmin, xmax;
  int buffer_size;
  int win_pos[2];
  int win_dim[2];
  int win;
  int number_of_subwin;
  int number_of_curves_by_subwin[1];
  int * colors;

  /*Retrieving Parameters*/
  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  buffer_size = ipar[2];
  win = ipar[0];
  period  = rpar[3];
  win_pos[0] = ipar[(nipar-1) - 3];
  win_pos[1] = ipar[(nipar-1) - 2];
  win_dim[0] = ipar[(nipar-1) - 1];
  win_dim[1] = ipar[nipar-1];
  dimension = 2;
  number_of_curves_by_subwin[0] = GetInPortRows(block,1);

  number_of_subwin = 1;
  ymin = rpar[1];
  ymax = rpar[2];


  colors = (int*)scicos_malloc(number_of_curves_by_subwin[0]*sizeof(int));
  for(i = 0 ; i < number_of_curves_by_subwin[0] ; i++)
    {
      colors[i] = ipar[3+i];
    }

  /*Allocating memory*/
  if(firstdraw == 1)
    {
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, number_of_curves_by_subwin);
      /*Must be placed before adding polyline or other elements*/
      scoSetLongDrawSize(*pScopeMemory, 0, 50);
      scoSetShortDrawSize(*pScopeMemory,0,buffer_size);
      scoSetPeriod(*pScopeMemory,0,period);
    }

  xmin = period*scoGetPeriodCounter(*pScopeMemory,0);
  xmax = period*(scoGetPeriodCounter(*pScopeMemory,0)+1);

  /*Creating the Scope*/
  scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      scoAddTitlesScope(*pScopeMemory,"t","y",NULL);
      /*Add a couple of polyline : one for the shortdraw and one for the longdraw*/
      scoAddCoupleOfPolylines(*pScopeMemory,colors);
      /* scoAddPolylineLineStyle(*pScopeMemory,colors); */
    }
  scicos_free(colors);
}

/** \fn void cscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cscope(scicos_block * block,int flag)
{
  ScopeMemory * pScopeMemory;
  int i;
  double t;
  int NbrPtsShort;
  double * u1;
  scoGraphicalObject pShortDraw;
  switch(flag) 
    {
    case Initialization:
      {
	cscope_draw(block,&pScopeMemory,1);
	break;
      }
    case StateUpdate:
      {
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    t = get_scicos_time();
	/*Retreiving Scope in the block->work*/
	    
	/*If window has been destroyed we recreate it*/
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    cscope_draw(block,&pScopeMemory,0);
	  }
	/*Maybe we are in the end of axes so we have to draw new ones */
	scoRefreshDataBoundsX(pScopeMemory,t);

	//Cannot be factorized depends of the scope
	u1 = GetRealInPortPtrs(block,1);
	for (i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    pShortDraw  = scoGetPointerShortDraw(pScopeMemory,0,i);
	    NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
	    pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort] = t;
	    pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort] = u1[i];
	    pPOLYLINE_FEATURE(pShortDraw)->n1++;
	  }
	//End of Cannot

	//Draw the Scope
	scoDrawScopeAmplitudeTimeStyle(pScopeMemory, t);
	  }
	break;
      }
    case Ending:
      {
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	    pShortDraw = sciGetCurrentFigure();
	    pFIGURE_FEATURE(pShortDraw)->user_data = NULL;
	    pFIGURE_FEATURE(pShortDraw)->size_of_user_data = 0;
	    scoDelCoupleOfPolylines(pScopeMemory);
	  }
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break;  
      }
    }
}
