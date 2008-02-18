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
/* file: sci_param3d.h                                                    */
/* desc : interface for param3d routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_PARAM3D_H_
#define _INT_PARAM3D_H_

/**
* interface function for the param3d routine :
* param3d(x,y,z,[theta,alpha,leg,flag,ebox])
*
* @param[in] fname      name of the routine (ie param3d)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_param3d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PARAM3D_H_ */
