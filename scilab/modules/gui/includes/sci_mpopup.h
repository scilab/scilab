/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * desc : interface for sci_mpopup routine
 * (temporary function waiting for uicontextmenu function) 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MPOPUP_H__
#define __MPOPUP_H__

/**
* interface function for the mpopup routine.
*
* @param[in] fname name of the routine (ie mpopup)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_mpopup( char * fname, unsigned long fname_len ) ;

#endif /* !__MPOPUP_H__ */
