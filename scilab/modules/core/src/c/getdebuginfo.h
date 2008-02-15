/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GETDEBUGINFO_H__
#define __GETDEBUGINFO_H__

/**
 * The commun stuff for the debug info retrieving
 */

#define NB_DEBUG_ELEMENT 255

typedef struct debugMessage {
	char *description;
	char *value;
} debug_message;

#endif /* __GETDEBUGINFO_H__ */
/*--------------------------------------------------------------------------*/

