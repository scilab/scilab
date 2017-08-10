/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
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

