/*--------------------------------------------------------------------------*/
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
#ifndef  __REALTIME_H__
#define __REALTIME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "machine.h"

/**
* @TODO add comment
*/
int C2F(realtimeinit)(double *t, double *scale);

/**
* @TODO add comment
*/
int C2F(realtime)(double *t);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* __REALTIME_H__ */
/*--------------------------------------------------------------------------*/