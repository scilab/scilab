/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
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
#ifndef __ISLITTLEENDIAN_H__
#define __ISLITTLEENDIAN_H__

#include "machine.h"

/**
* islittleendian
* @return if it is a little endian (0 or 1)
*/
int islittleendian(void);

/**
* C2F(getendian)
* @param void
* @return the endian (0 or 1)
*/
int C2F(getendian)(void);

#endif /* __ISLITTLEENDIAN_H__ */
/*--------------------------------------------------------------------------*/
