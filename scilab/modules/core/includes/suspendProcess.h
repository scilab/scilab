/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef SUSPEND_PROCESS_H_
#define SUSPEND_PROCESS_H_

/**
 * Fuction call when CTRL + Z is pressed.
 * Reset termcaps and colors to the shell settings when scilab is suspended.
 * This function is a sighandler_t called by signal with SIGTSTP.
 */
void suspendProcess(int signum);

/**
 * Fuction call when Scilab is called back with fg.
 * Set termcaps and colors to Scilab settings.
 * This function is a sighandler_t called by signal with SIGCONT.
 */
void continueProcess(int signum);

#endif /* !SUSPEND_PROCESS_H_ */
