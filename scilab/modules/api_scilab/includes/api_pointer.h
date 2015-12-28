/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

scilabStatus API_PROTO(getPointer)(scilabEnv env, scilabVar var, const void** val);

scilabStatus API_PROTO(setPointer)(scilabEnv env, scilabVar var, const void* val);

#ifdef __cplusplus
}
#endif
#endif /* __USERTYPE_API__ */
