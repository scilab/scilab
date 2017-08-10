/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/

#ifndef __USERTYPE_API__
#define __USERTYPE_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_usertype.h. Include api_scilab.h instead.
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createPointer        API_PROTO(createPointer)
#define scilab_getPointer           API_PROTO(getPointer)
#define scilab_setPointer           API_PROTO(setPointer)

/* Scilab 6 API*/
scilabVar API_PROTO(createPointer)(scilabEnv env, void* val);

scilabStatus API_PROTO(getPointer)(scilabEnv env, scilabVar var, void** val);

scilabStatus API_PROTO(setPointer)(scilabEnv env, scilabVar var, void* val);

#ifdef __cplusplus
}
#endif
#endif /* __USERTYPE_API__ */
