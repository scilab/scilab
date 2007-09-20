/**
   \file bouncexy.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief BOUNCEXY has to be used with bounce_ball block
   \see BOUNCEXY.sci in macros/scicos_blocks/Misc/
*/

#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"

/** \fn bouncexy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void bouncexy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  scoGraphicalObject pAxes;
  scoGraphicalObject pTemp;
  double * z;
  double *rpar;
  int *ipar, nipar;  
  int i,j;
  int dimension;
  double ymin, ymax, xmin, xmax;
  int win;
  int number_of_subwin;
  int number_of_curves_by_subwin;
  int * colors;
  int imode;
  double * size_balls;
  double radius_max;

  /*Retrieving Parameters*/
  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  win = ipar[0];
  if (win == -1)
    {
      win = 20000 + get_block_number() ; 
    }
  dimension = 2;
  imode = ipar[1];
  number_of_curves_by_subwin = GetInPortRows(block,1);
  radius_max = 0;
  size_balls = (double*)scicos_malloc(number_of_curves_by_subwin*sizeof(double));
  z = GetDstate(block);
  for(i = 0 ; i < number_of_curves_by_subwin ; i++)
    {
      size_balls[i] = z[6*i+2];
      if(radius_max < size_balls[i])
	{
	  radius_max = size_balls[i];
	}
    }
  number_of_subwin = 1;
  xmin = rpar[0];
  xmax = rpar[1];
  ymin = rpar[2];
  ymax = rpar[3];
  colors = (int*)scicos_malloc(number_of_curves_by_subwin*sizeof(int));
  for(i = 0 ; i < number_of_curves_by_subwin ; i++)
    {
      colors[i] = ipar[i+2];
    }
  if(firstdraw == 1)
    {
      /*Allocating memory*/
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
    }
  /*Creating the Scope*/
  scoInitOfWindow(*pScopeMemory, dimension, win, NULL, NULL, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
  pTemp = scoGetPointerScopeWindow(*pScopeMemory);
  pAxes = scoGetPointerAxes(*pScopeMemory,0);

  pSUBWIN_FEATURE(pAxes)->isoview = TRUE;

  (pSUBWIN_FEATURE(pAxes)->axes).axes_visible[0] = FALSE;
  (pSUBWIN_FEATURE(pAxes)->axes).axes_visible[1] = FALSE;

  sciSetIsBoxed(pAxes, FALSE);

  pFIGURE_FEATURE(pTemp)->pixmap = 1;
  pFIGURE_FEATURE(pTemp)->wshow = 1;

  for(j = 0 ; j < number_of_curves_by_subwin ; j++)
    {
      scoAddSphereForShortDraw(*pScopeMemory, 0, j, size_balls[j], colors[j]);
    }
  scoAddRectangleForLongDraw(*pScopeMemory,0,0,xmin,(ymax-fabs(ymin)),fabs(xmax-xmin),fabs(ymax-ymin));
  sciDrawObj(scoGetPointerLongDraw(*pScopeMemory,0,0));
    }
  scicos_free(colors);
  scicos_free(size_balls);

}

/** \fn void bouncexy(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void bouncexy(scicos_block * block,int flag)
{
  ScopeMemory * pScopeMemory;
  scoGraphicalObject pShortDraw;
  scoGraphicalObject pLongDraw;
  double * z;
  double t;
  int i;
  double * u1, *u2;
  double * size_balls;
  switch(flag) 
    {
    case Initialization:
      {
	bouncexy_draw(block,&pScopeMemory,1);
	break;
      }
    case StateUpdate:
      {

	/*Retreiving Scope in the block->work*/
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    t = get_scicos_time();
	/*If window has been destroyed we recreate it*/
	if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	  {
	    bouncexy_draw(block,&pScopeMemory,0);
	  }

	//Cannot be factorized depends of the scope
	size_balls = (double*)scicos_malloc(scoGetNumberOfCurvesBySubwin(pScopeMemory,0)*sizeof(double));
	z = GetDstate(block);
	for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    size_balls[i] = z[6*i+2];
	  }
	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);
	for (i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	  {
	    pShortDraw  = scoGetPointerShortDraw(pScopeMemory,0,i);
	    pLongDraw  = scoGetPointerLongDraw(pScopeMemory,0,i);
	    pARC_FEATURE(pShortDraw)->x = u1[i]-size_balls[i]/2;
	    pARC_FEATURE(pShortDraw)->y = u2[i]+size_balls[i]/2;

	  }

	sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	
	if(pFIGURE_FEATURE(scoGetPointerScopeWindow(pScopeMemory))->pixmap == 1)
	  {
	    C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	    sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
	  }
	else
	  {
	    sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
	  }

	scicos_free(size_balls);
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
	  }
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break;  
      }
    }
}
