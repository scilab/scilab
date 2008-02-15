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

#ifndef __GETCOMMANDLINEARGS_H__
#define __GETCOMMANDLINEARGS_H__

/* max of command token */

#define MAXCMDTOKENS 256

/**
* get command line arguments
* @param returns nbargs number of arguments
* @return arguments (char **) 
*/
char ** getCommandLineArgs(int *nbargs);

/**
* save command line arguments
* param argv
* param nbargs
*/
int setCommandLineArgs(char **argv, int nbargs);

#endif /* __GETCOMMANDLINEARGS_H__ */
/*--------------------------------------------------------------------------*/ 
