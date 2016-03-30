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

#ifndef GET_KEY_H_
#define GET_KEY_H_

#define CHECK_TOKEN -1
#define CONTINUE_COMMAND 0
#define RESET_TOKEN 0
#define SEND_COMMAND 1
#define SEND_MULTI_COMMAND 2
#define DO_NOT_SEND_COMMAND 3

/**
 * Wait for user user input and Manage all the command line.
 * Function is left when the user press enter.
 * @return command line enter by the user.
 */
char *getCmdLine(void);

/**
 * Set a token to know what to do during command line edition.
 * @param token to set.
 * @return token saved.
 */
int setTokenInteruptExecution(int token);

/**
 * Get the token to know if the line must be edited.
 * @return token saved.
 */
int getTokenInteruptExecution();

int getTokenInteruptExecution(void);

#endif /* !GET_KEY_H_ */
