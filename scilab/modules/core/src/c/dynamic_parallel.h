/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __DYNAMIC_PARALLEL_H__
#define __DYNAMIC_PARALLEL_H__

/**
* parallel gateway
* parallel is loaded dynamically at runtime
* load gw_parallel in SCI/modules/parallel/sci_gateway/gw_parallel.c
*/
int gw_dynamic_parallel(void);

/**
* unload parallel gateway
*/
BOOL dynTerminateParallel(void);

/**
* see SCI/modules/parallel/includes/concurrency.h
*/
int dynParallelForbidden(char const* fname);

/**
* see SCI/modules/parallel/includes/concurrency.h
*/
int dynParallelConcurrency(void);

#endif /* __DYNAMIC_PARALLEL_H__ */
