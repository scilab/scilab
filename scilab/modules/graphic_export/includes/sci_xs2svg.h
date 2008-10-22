/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * desc : interface for xs2svg routine 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _INT_XS2SVG_H_
#define _INT_XS2SVG_H_

/**
 * interface function for the xs2svg routine :
 *
 * @param[in] fname     name of the routine (ie xs2svg)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2svg( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2SVG_H_ */
