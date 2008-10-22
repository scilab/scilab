/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_rotate_axes.h                                                */
/* desc : interface for rotate_axes routine                               */
/*------------------------------------------------------------------------*/

#ifndef _SCI_ROTATE_AXES_H_
#define _SCI_ROTATE_AXES_H_

/**
* interface function for the select_axes routine :
* rotate_axes(handle)
*
* @param[in] fname     name of the routine (ie select_axes)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_rotate_axes( char * fname, unsigned long fname_len ) ;

#endif /* _SCI_ROTATE_AXES_H_ */
