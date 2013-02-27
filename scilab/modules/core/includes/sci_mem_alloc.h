/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#include <stdlib.h>

#ifdef _MSC_VER
#error This file should not be used on windows
#endif

#ifdef MALLOC
#undef MALLOC
#endif

#ifdef FREE
#undef FREE
#endif

#ifdef CALLOC
#undef CALLOC
#endif

#ifdef REALLOC
#undef REALLOC
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#define MALLOC malloc
#define FREE free
#define REALLOC realloc
#define CALLOC calloc

#define SCISTACKMALLOC malloc
#define SCISTACKFREE free

#endif /* __SCI_MEM_ALLOC__ */

