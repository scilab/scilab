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

#ifndef	READER_H_
#define	READER_H_

#include <wchar.h>

/**
 * Get the command line.
 * @param pointer of adress of the structure.
 * mean the value of the adress may change.
 * @return command line put by user.
 */
char *getCmdLine(void);

#endif /* !READER_H_ */
