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
/* file: sci_plot3d.h                                                     */
/* desc : interface for plot3d and plot3d1 routines                       */
/*------------------------------------------------------------------------*/

#ifndef _INT_PLOT3D_H_
#define _INT_PLOT3D_H_

/**
* interface function for the plot3d routine.
*
* @param[in] fname     name of the routine (ie plot3d or plot3d1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot3d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PLOT3D_H_ */
