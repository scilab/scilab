/**
   \file scoGetProperty.h
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Header File of the scoGetProperty.c file 
*/

#ifndef __SCO_GET_PROPERTY_H__
#define __SCO_GET_PROPERTY_H__

#include "scoBase.h"

/**
   \brief Get the Handle of the Scope Window in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \return hScopeWindow
*/
scoLongInteger scoGetHandleScopeWindow(ScopeMemory * pScopeMemory);

/**
   \brief Get the Handle of Axes i in the ScopeMemory where i is the index of the axes
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an Axes
   \return hAxes[i]
*/
scoLongInteger scoGetHandleAxes(ScopeMemory * pScopeMemory, int i);

/**
   \brief Get the Handle of the ShortDraw i,j in the ScopeMemory where i is the index of the axes and j is the index of the curve in the axes
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an Axes
   \param j index of a curve
   \return hShortDraw[i][j]
*/
scoLongInteger scoGetHandleShortDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
   \brief Get the Handle of the LongDraw i,j in the ScopeMemory where i is the index of the axes and j is the index of the curve in the axes
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an Axes
   \param j index of a curve
   \return hLongDraw[i][j]
*/
scoLongInteger scoGetHandleLongDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
   \brief Get the NewDraw value of Axes i in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \return new_draw[i]
*/
scoInteger scoGetNewDraw(ScopeMemory * pScopeMemory, int i);

/**
   \brief Get the PeriodCounter of Axes i in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \return period_counter[i]
*/
scoInteger scoGetPeriodCounter(ScopeMemory * pScopeMemory, int i);

/**
   \brief Get the TraceLength of the Axes i in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \return trace_length[i]
*/
scoInteger scoGetLongDrawSize(ScopeMemory * pScopeMemory, int i);

/**
   \brief Get the NumberOfSubwin in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \return number_of_subwin[i]
*/
scoInteger scoGetNumberOfSubwin(ScopeMemory * pScopeMemory);

/**
   \brief Get the NumberOfCurvesBySubwin of Axes i in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \return number_of_curves_by_subwin[i]
*/
scoInteger scoGetNumberOfCurvesBySubwin(ScopeMemory * pScopeMemory,int i);

/**
   \brief Get the WindowID of the Scope Graphic Window in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \return win_id
*/
scoInteger scoGetWindowID(ScopeMemory * pScopeMemory);

/**
   \brief Get the BufferSize of the Axes i in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \return buffer_sze[i]
*/
scoInteger scoGetShortDrawSize(ScopeMemory * pScopeMemory,int i);

/**
   \brief Get the Period of the Axes i in the ScopeMemory
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axes
   \return period[i]
*/
scoDouble scoGetPeriod(ScopeMemory * pScopeMemory, int i);


/**
   \brief Get the Pointer on the ScopeWindow
   \param pScopeMemory a pointer on a ScopeMemory
   \return an scoGraphicalObject which is the pointer on the Scope Window
*/
scoGraphicalObject scoGetPointerScopeWindow(ScopeMemory * pScopeMemory);

/**
   \brief Get the Pointer on Axes[i]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axe
   \return an scoGraphicalObject which is the pointer on the Axes[i]
*/
scoGraphicalObject scoGetPointerAxes(ScopeMemory * pScopeMemory, int i);

/**
   \brief Get the Pointer on ShortDraw[i][j]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axe
   \param j index of a curve in the Axes[i]
   \return an scoGraphicalObject which is the pointer on the ShortDraw[i][j]
*/
scoGraphicalObject scoGetPointerShortDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
   \brief Get the Pointer on LongDraw[i][j]
   \param pScopeMemory a pointer on a ScopeMemory
   \param i index of an axe
   \param j index of a curve in the Axes[i]
   \return an scoGraphicalObject which is the pointer on the LongDraw[i][j]
*/
scoGraphicalObject scoGetPointerLongDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
   \brief Get status of activation of the block
   \return 1 activated or 0 deactivated (by default)
  
*/
scoInteger scoGetScopeActivation(ScopeMemory * pScopeMemory);

#endif
