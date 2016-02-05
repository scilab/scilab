/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
