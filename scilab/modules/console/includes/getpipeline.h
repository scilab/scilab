/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Scilab Enterprises - Cedric Delamarre
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#ifndef _GETPIPELINE_H_
#define _GETPIPELINE_H_

#include "dynlib_console.h"
/**
* getPipeLine function
* @return characters read from stdin when stdin is not a tty
* ie: echo "1+1" | bin/scilab -nwni
*/
CONSOLE_IMPEXP char* getPipeLine(void);

#endif /* _GETPIPELINE_H_ */
