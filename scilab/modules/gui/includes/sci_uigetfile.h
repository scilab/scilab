/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Sylvestre KOUMAR
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_uigetfile.h                                                  */
/* desc : interface for sci_uigetfile routine                             */
/*------------------------------------------------------------------------*/

#ifndef _INT_UIGETFILE_H_
#define _INT_UIGETFILE_H_

/**
* interface function for the uigetfile routine :
* [rep,win] = uigetfile([flag],[sel])
*
* @param[in] fname     name of the routine (ie uigetfile)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_uigetfile( char * fname, unsigned long fname_len ) ;

#endif /* _INT_UIGETFILE_H_ */
