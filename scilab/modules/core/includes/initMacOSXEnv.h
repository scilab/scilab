/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __INITMACOSXENV_H__
#define __INITMACOSXENV_H__

/* 
 * Specific wrapper for mac os X which is going to call realmin in a specific
 * thread.
 * Takes the same args as StartScilabEngine 
 * @param argc
 * @param argv initial_script the initial script given to Scilab
 * @param iFileIndex initial script index in argv : the initial script given to Scilab
 * @return 1 if OK or 0 if KO
 */ 

int initMacOSXEnv(int argc, char *argv[], int iFileIndex);

int StartScilabEngine(int argc, char *argv[], int iFileIndex);

#endif /* !__INITMACOSXENV_H__ */
