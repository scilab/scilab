/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
#ifndef __RESIZESIGNAL_H__
#define __RESIZESIGNAL_H__
#include <signal.h>

/**
 * Set the console width read from the term size.
 */
void resize_handler(int sig);


/**
* register the signal SIGWINCH.
*/
int resizesignal(void);

#endif /* __RESIZESIGNAL_H__ */
