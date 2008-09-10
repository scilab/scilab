/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_champ.h                                                      */
/* desc : interface for champ (and champ1) routine                        */
/*------------------------------------------------------------------------*/

#ifndef _INT_CHAMP_H_
#define _INT_CHAMP_H_

#include "machine.h"

/**
* interface function for the champ routine :
* champ(x,y,fx,fy,[arfact=1.0,rect=[xMin,yMin,xMax,yMax],flag])
*
* @param[in] fname     name of the routine (ie champ)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_champ( char * fname, unsigned long fname_len ) ;

/**
* interface function for the champ routine :
* champ1(x,y,fx,fy,[arfact=1.0,rect=[xMin,yMin,xMax,yMax],flag])
*
* @param[in] fname     name of the routine (ie champ)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_champ1( char * fname, unsigned long fname_len ) ;

/**
 * Generic function for both champ and champ1.
 */
int sci_champ_G( char *fname,
                 int (*func) (double *, double *, double *, double *, integer *, integer *, char *, double *, double *, int),
                 unsigned long fname_len ) ;

#endif /* _INT_CHAMP_H_ */
