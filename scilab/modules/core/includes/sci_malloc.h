/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*-----------------------------------------------------------------------------------*/
#ifndef __SCILAB_MALLOC_H__
#define __SCILAB_MALLOC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _MSC_VER
#include "win_mem_alloc.h" /* MALLOC */
#else /* others O.S*/
#include "sci_mem_alloc.h" /* MALLOC */
#endif

#ifdef __cplusplus
};
#endif /* extern "C" */

#endif /* __SCILAB_MALLOC_H__ */
/*-----------------------------------------------------------------------------------*/
