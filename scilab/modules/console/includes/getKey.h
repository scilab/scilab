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

#ifndef GET_KEY_H_
#define GET_KEY_H_

#include <wchar.h>

/**
 * Wait for user user input and Manage all the command line.
 * Function is left when the user press enter.
 * @return command line enter by the user.
 */
char *getCmdLine(void);

#endif /* !GET_KEY_H_ */
