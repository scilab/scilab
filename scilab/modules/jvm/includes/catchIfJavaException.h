
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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

/*--------------------------------------------------------------------------*/
#ifndef __CATCHIFJAVAEXCEPTION_H__
#define __CATCHIFJAVAEXCEPTION_H__

#include "BOOL.h" /* BOOL */

/**
 * Check if there is an exception and print if it is the case
 * @param errorMsg the error message which will be displayed if an exception is catched
 * @return
 */
BOOL catchIfJavaException(char *errorMsg);

#endif /* __CATCHIFJAVAEXCEPTION_H__*/
/*--------------------------------------------------------------------------*/
