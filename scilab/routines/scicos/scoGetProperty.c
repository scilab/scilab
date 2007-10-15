/**
   \file scoGetProperty.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source code of all accessors for ScopeMemory structure
*/

#include "scoGetProperty.h"

scoLongInteger scoGetHandleScopeWindow(ScopeMemory * pScopeMemory)
{
  return pScopeMemory->hScopeWindow;
}

scoLongInteger scoGetHandleAxes(ScopeMemory * pScopeMemory, int i)
{
  return pScopeMemory->hAxes[i];
}

scoLongInteger scoGetHandleShortDraw(ScopeMemory * pScopeMemory, int i, int j)
{
  return pScopeMemory->hShortDraw[i][j];
}

scoLongInteger scoGetHandleLongDraw(ScopeMemory * pScopeMemory, int i, int j)
{
  return pScopeMemory->hLongDraw[i][j];
}

scoGraphicalObject scoGetPointerScopeWindow(ScopeMemory * pScopeMemory)
{
  return  sciGetPointerFromHandle(scoGetHandleScopeWindow(pScopeMemory));
}

scoGraphicalObject scoGetPointerAxes(ScopeMemory * pScopeMemory, int i)
{
  return sciGetPointerFromHandle(scoGetHandleAxes(pScopeMemory,i));
}

scoGraphicalObject scoGetPointerShortDraw(ScopeMemory * pScopeMemory, int i, int j)
{
  return sciGetPointerFromHandle(scoGetHandleShortDraw(pScopeMemory,i,j));
}

scoGraphicalObject scoGetPointerLongDraw(ScopeMemory * pScopeMemory, int i, int j)
{
  return sciGetPointerFromHandle(scoGetHandleLongDraw(pScopeMemory,i,j));
}

scoInteger scoGetNumberOfSubwin(ScopeMemory * pScopeMemory)
{
  return pScopeMemory->number_of_subwin;
}

scoInteger scoGetNumberOfCurvesBySubwin(ScopeMemory * pScopeMemory,int i)
{
  return pScopeMemory->number_of_curves_by_subwin[i];
}

scoInteger scoGetPeriodCounter(ScopeMemory * pScopeMemory, int i)
{
  return pScopeMemory->period_counter[i];
}

scoInteger scoGetNewDraw(ScopeMemory * pScopeMemory, int i)
{
  return pScopeMemory->new_draw[i];
}

scoInteger scoGetLongDrawSize(ScopeMemory * pScopeMemory, int i)
{
  return  pScopeMemory->longdraw_size[i];
}

scoInteger scoGetWindowID(ScopeMemory * pScopeMemory)
{
  return pScopeMemory->win_id;
}

scoInteger scoGetShortDrawSize(ScopeMemory * pScopeMemory, int i)
{
  return pScopeMemory->shortdraw_size[i];
}

scoDouble scoGetPeriod(ScopeMemory * pScopeMemory, int i)
{
  return pScopeMemory->period[i];
}

scoInteger scoGetScopeActivation(ScopeMemory * pScopeMemory)
{
  if (pScopeMemory==NULL) {
    return 0;
  }
  else {
    return pScopeMemory->activated;
  }
}
