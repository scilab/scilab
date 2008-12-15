/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: get_ticks_utils.h                                                */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#ifndef _GET_TICKS_UTILS_H_
#define _GET_TICKS_UTILS_H_

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "returnPropertyList.h"

int buildTListForTicks( const double * locations, char * labels[], int nbTics ) ;

int CreatePrettyGradsFromNax(sciPointObj * psubwin,int * Nax);

double * FreeUserGrads(double * u_xgrads);

int GraduateWithNax(sciSubWindow * ppsubwin,double *min,double *max,int nbtics, double * grads);

char ** AllocAndSetUserLabelsFromMdl(char ** u_xlabels, char ** u_xlabels_MDL, int u_nxgrads);

char ** FreeUserLabels(char ** u_xlabels, int *u_nxgrads);

double * AllocUserGrads(double * u_xgrads, int nb);
int CopyUserGrads(double *u_xgrad_SRC, double *u_xgrad_DEST, int dim);


#endif /* _GET_TICKS_UTILS_H_ */
