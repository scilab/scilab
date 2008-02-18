
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __PCRE_ERROR_H__
#define __PCRE_ERROR_H__


/**
 * 
 * Display a human readable error message (PCRE error).
 * @param fname the name of the Scilab function
 * @param errorCode the error code returned by the pcre function
 */
void pcre_error(char *fname, int errorCode);

#endif

