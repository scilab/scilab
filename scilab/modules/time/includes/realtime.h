/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int C2F(realtimeinit)(double *t,double *scale);

/**
* @TODO add comment
*/
int C2F(realtime)(double *t);

#ifdef __cplusplus
}
#endif

#endif /* __REALTIME_H__ */ 
/*--------------------------------------------------------------------------*/