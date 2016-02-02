/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
