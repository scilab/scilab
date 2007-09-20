/**
   \file scoSetProperty.h
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Header file of the scoSetProperty.c file
*/

#ifndef __SCO_SET_PROPERTY_H__
#define __SCO_SET_PROPERTY_H__

#include "scoBase.h"

/**
   \brief Set value in the hScopeWindow
   \param pScopeMemory a pointer on a ScopeMemory
   \param value the new value of the handle
*/
void scoSetHandleScopeWindow(ScopeMemory * pScopeMemory, long value);

/**
   \brief Set value in the hAxes
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of the handle hAxes[i]
*/
void scoSetHandleAxes(ScopeMemory * pScopeMemory, int i, long value);

/**
   \brief Set value in the hShortDraw[i][j] 
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param j index of a curve
   \param value the new value of the hShortDraw[i][j]
*/
void scoSetHandleShortDraw(ScopeMemory * pScopeMemory, int i,int j, long value);

/**
   \brief Set value in the hLongDraw[i][j]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param j index of a curve
   \param value the new value of the hLongDraw[i][j]
*/ 
void scoSetHandleLongDraw(ScopeMemory * pScopeMemory, int i,int j, long value);

/**
   \brief Set value in the new_draw[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of new_draw[i]
*/
void scoSetNewDraw(ScopeMemory * pScopeMemory, int i, int value);

/**
   \brief Set value in the period_counter[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of period_counter[i]
*/
void scoSetPeriodCounter(ScopeMemory * pScopeMemory, int i, int value);

/**
   \brief Set value in the trace_length[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of trace_length[i]
*/
void scoSetLongDrawSize(ScopeMemory * pScopeMemory, int i, int value);

/**
   \brief Set value in the number_of_subwin
   \param pScopeMemory a pointer on a ScopeMemory
   \param value the new value of number_of_subwin

*/
void scoSetNumberOfSubwin(ScopeMemory * pScopeMemory, int value);

/**
   \brief Set value in the number_of_curves_by_subwin[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of number_of_curves_by_subwin[i]
*/ 
void scoSetNumberOfCurvesBySubwin(ScopeMemory * pScopeMemory, int i, int value);

/**
   \brief Set value in the win_id
   \param pScopeMemory a pointer on a ScopeMemory
   \param value the new value of the win_id
*/
void scoSetWindowID(ScopeMemory * pScopeMemory, int value);

/**
   \brief Set value in the buffer_size[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of buffer_size[i]
*/
void scoSetShortDrawSize(ScopeMemory * pScopeMemory, int i, int value);

/**
   \brief Set value in the period[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \param value the new value of period[i]
*/
void scoSetPeriod(ScopeMemory * pScopeMemory, int i, double value);

/**
   \brief Set value of handle axes in the structure by passing a pointer on argument
   \param pScopeMemory a pointer on a ScopeMemory
   \param i the position i of the axes
   \param value a scoGraphicalObject (i.e. sciPointObj *) of an axes
*/
void scoSetHandleFromPointerAxes(ScopeMemory * pScopeMemory, int i, scoGraphicalObject value);

/**
   \brief Set value of handle shortdraw in the structure by passing a pointer on argument
   \param pScopeMemory a pointer on a ScopeMemory
   \param i the position i of the axes
   \param j the position j of the shortdraw in the axes
   \param value a scoGraphicalObject (i.e. sciPointObj *) of a shortdraw
*/
void scoSetHandleFromPointerShortDraw(ScopeMemory * pScopeMemory, int i,int j, scoGraphicalObject value);

/**
   \brief Set value of handle longdraw in the structure by passing a pointer on argument
   \param pScopeMemory a pointer on a ScopeMemory
   \param i the position i of the axes
   \param j the position j of the longdraw in the axes
   \param value a scoGraphicalObject (i.e. sciPointObj *) of a longdraw
*/
void scoSetHandleFromPointerLongDraw(ScopeMemory * pScopeMemory, int i,int j, scoGraphicalObject value);

/**
   \brief Set value of handle scopewindow in the structure by passing a pointer on argument
   \param pScopeMemory a pointer on a ScopeMemory
   \param value a scoGraphicalObject (i.e. sciPointObj *) of a window
*/
void scoSetHandleFromPointerScopeWindow(ScopeMemory * pScopeMemory, scoGraphicalObject value);


/**
   \brief Set value for activation (1 activate 0 else (by default))
   \param pScopeMemory a pointer on a ScopeMemory
   \param status : 1 activate 0 deactivate
*/
void scoSetScopeActivation(ScopeMemory * pScopeMemory, int status);


#endif
