/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __COMMAND_H__
#define __COMMAND_H__

/* maximum number of chars allowed on line */
#define MAX_LINE_LEN 512	

/**
* reads a scilab line 
* @param[in] current prompt
* @param[in] interrupt mode
*/
int read_line (char *prompt, int interrupt);

#endif /* __COMMAND_H__ */
/*--------------------------------------------------------------------------*/
