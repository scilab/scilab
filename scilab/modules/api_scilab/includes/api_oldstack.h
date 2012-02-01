/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
  * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __OLDSTACK_API__
#define __OLDSTACK_API__

#include "dynlib_api_scilab.h"
#ifdef __cplusplus
extern "C" {
#endif

static int api_fake_int = 0;

/*  TOP  */
#ifdef Top
#undef Top
#define Top api_Top(pvApiCtx)
#endif
int api_Top(void* pvApiCtx);

/*  RHS  */
#ifdef Rhs
#undef Rhs
#define Rhs api_Rhs(pvApiCtx)
#endif
int api_Rhs(void* pvApiCtx);

/*  LHS  */
#ifdef Lhs
#undef Lhs
#define Lhs api_Lhs(pvApiCtx)
#endif
int api_Lhs(void* pvApiCtx);

/*  CheckRhs  */
#ifdef CheckRhs
#undef CheckRhs
#define CheckRhs(a,b) \
	if(api_CheckRhs(a,b, pvApiCtx) == 0) \
	{ \
		return 1; \
	}
#endif
int api_CheckRhs(int _iMin, int _iMax, void* pvApiCtx);

/*  CheckLhs  */
#ifdef CheckLhs
#undef CheckLhs
#define CheckLhs(a,b) \
	if(api_CheckLhs(a,b, pvApiCtx) == 0) \
	{ \
		return 1; \
	}
#endif
int api_CheckLhs(int _iMin, int _iMax, void* pvApiCtx);

//LhsVar(1) = Rhs + 1;
#ifdef LhsVar
#undef LhsVar
#define LhsVar(x) *api_LhsVar(x, pvApiCtx)
#endif
int* api_LhsVar(int _iVal, void* _pvCtx);

#ifdef PutLhsVar
#undef PutLhsVar
#define PutLhsVar();
#endif

#ifdef OverLoad
#undef OverLoad
#define OverLoad(x) api_OverLoad(x, pvApiCtx)
#endif
void api_OverLoad(int _iVal, void* pvApiCtx);

#ifdef __cplusplus
}
#endif
#endif /* __OLDSTACK_API__ */
