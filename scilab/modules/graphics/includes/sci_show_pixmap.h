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
/* file: sci_show_pixmap.h                                                */
/* desc : interface for show_pixmap routine                               */
/*------------------------------------------------------------------------*/

#ifndef _SCI_SHOW_PIXMAP_H_
#define _SCI_SHOW_PIXMAP_H_

/**
* interface function for the show_pixmap routine :
* show_pixmap()
*
* @param[in] fname     name of the routine (ie show_pixmap)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_show_pixmap( char * fname, unsigned long fname_len ) ;

#endif /* _SCI_SHOW_PIXMAP_H_ */
