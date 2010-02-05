/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#ifndef  __IPMPAR1_H__
#define __IPMPAR1_H__

#include "dynlib_statistics.h"

#include "machine.h"

/**
 * Get machine's largest integer
 * called by dcdlib in ipmpar.f
 * Should be replaced with appropriate compile-time value
 * @return largest machine's integer
 */
STATISTICS_IMPEXP int C2F(largestint)(void);

#endif /* __IPMPAR1_H__ */
