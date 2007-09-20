/**
   \file scoSetProperty.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source code of all Mutators for ScopeMemory structure
*/

#include "scoSetProperty.h"

void scoSetHandleShortDraw(ScopeMemory * pScopeMemory, int i,int j, long value)
{
  pScopeMemory->hShortDraw[i][j] = value;
}

void scoSetHandleFromPointerAxes(ScopeMemory * pScopeMemory, int i, scoGraphicalObject value)
{
  pScopeMemory->hAxes[i] = sciGetHandle(value);
}

void scoSetHandleFromPointerShortDraw(ScopeMemory * pScopeMemory, int i,int j, scoGraphicalObject value)
{
  pScopeMemory->hShortDraw[i][j] = sciGetHandle(value);
}

void scoSetHandleLongDraw(ScopeMemory * pScopeMemory, int i,int j, long value)
{
  pScopeMemory->hLongDraw[i][j] = value;
}

void scoSetHandleFromPointerLongDraw(ScopeMemory * pScopeMemory, int i,int j, scoGraphicalObject value)
{
  pScopeMemory->hLongDraw[i][j] = sciGetHandle(value);
}

void scoSetHandleScopeWindow(ScopeMemory * pScopeMemory, long value)
{
  pScopeMemory->hScopeWindow = value;
}

void scoSetHandleFromPointerScopeWindow(ScopeMemory * pScopeMemory, scoGraphicalObject value)
{
  pScopeMemory->hScopeWindow = sciGetHandle(value);
}

void scoSetNewDraw(ScopeMemory * pScopeMemory, int i, int value)
{
  pScopeMemory->new_draw[i] = value;
}

void scoSetPeriodCounter(ScopeMemory * pScopeMemory, int i, int value)
{
  pScopeMemory->period_counter[i] = value; 
}

void scoSetLongDrawSize(ScopeMemory * pScopeMemory, int i, int value)
{
  pScopeMemory->longdraw_size[i] = value;
}

void scoSetHandleAxes(ScopeMemory * pScopeMemory, int i, long value)
{
  pScopeMemory->hAxes[i] = value;
}

void scoSetNumberOfCurvesBySubwin(ScopeMemory * pScopeMemory, int i, int value)
{
  pScopeMemory->number_of_curves_by_subwin[i] = value;
}

void scoSetNumberOfSubwin(ScopeMemory * pScopeMemory, int value)
{
  pScopeMemory->number_of_subwin = value;
}

void scoSetWindowID(ScopeMemory * pScopeMemory, int value)
{
  pScopeMemory->win_id = value;
}

void scoSetShortDrawSize(ScopeMemory * pScopeMemory, int i,int value)
{
  pScopeMemory->shortdraw_size[i] = value;
}

void scoSetPeriod(ScopeMemory * pScopeMemory, int i,double value)
{
  pScopeMemory->period[i] = value;
}

void scoSetScopeActivation(ScopeMemory * pScopeMemory, int status)
{
  pScopeMemory->activated = status;
}
