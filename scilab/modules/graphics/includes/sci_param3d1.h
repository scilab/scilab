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
/* file: sci_param3d1.h                                                   */
/* desc : interface for param3d1 routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_PARAM3D1_H_
#define _INT_PARAM3D1_H_

/**
* interface function for the param3d1 routine :
* param3d1(x,y,z,[theta,alpha,leg,flag,ebox])
* param3d1(x,y,list(z,colors),[theta,alpha,leg,flag,ebox])
*
* @param[in] fname      name of the routine (ie param3d1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_param3d1( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PARAM3D1_H_ */
