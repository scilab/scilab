/**
   \file scoMisc.h
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Header File of the scoMisc.c file 
*/

#ifndef __SCO_MISC_H__
#define __SCO_MISC_H__

#include"scoBase.h"

/**
   \brief Stop simulation and return an indication of the bug
   \param pScopeMemory a pointer on a ScopeMemory
   \param code_error a code error (see in the function to determine the right number for the moment)
*/
void scoScopeError(ScopeMemory * pScopeMemory, int code_error);

/**
 */
extern int C2F(getlabel)();

/**
 */
extern int C2F(deletewin)();

/**
 */

/**
 */
int permutobj(sciPointObj *pobj_1, sciPointObj *pparent_1,
              sciPointObj *pobj_2, sciPointObj *pparent_2);
#endif
