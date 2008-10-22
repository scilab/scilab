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
/* file: sci_plot2d.h                                                     */
/* desc : interface for plot2d routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_PLOT2D_
#define _INT_PLOT2D_

/**
 * interface function fot the plot2d routine :
 * plot2d(x,y,[style,strf,leg,rect,nax])
 *
 * @param[in] fname      name of the routine (ie plot2d)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
 */
int sci_plot2d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PLOT2D_ */
