/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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
