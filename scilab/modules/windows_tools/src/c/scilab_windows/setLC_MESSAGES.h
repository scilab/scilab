
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __SETLC_MESSAGES__
#define __SETLC_MESSAGES__

/**
* Initialize LC_MESSAGES environment variable
* To fix a bug with Windows multilanguage (MUI)
* see J-B and Simoné pcs 
* english GUI & french keyboard
* with French GUI installed but NOT used
*/
void setLC_MESSAGES(void);

#endif /* __SETLC_MESSAGES__ */
/*--------------------------------------------------------------------------*/