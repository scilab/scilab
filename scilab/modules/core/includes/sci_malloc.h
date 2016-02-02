/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
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
