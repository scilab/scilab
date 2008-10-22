/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * desc : interface for xs2ps routine 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _INT_XS2PS_H_
#define _INT_XS2PS_H_

/**
 * interface function for the xs2ps routine :
 *
 * @param[in] fname     name of the routine (ie xs2ps)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
*/
int sci_xs2ps( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2PS_H_ */
