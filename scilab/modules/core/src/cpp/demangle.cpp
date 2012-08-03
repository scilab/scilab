/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <cstring>

extern "C" {
#include "machine.h"
#include "backtrace.h"
}

#if defined(HAVE_GLIBC_BACKTRACE) && defined(HAVE_CPLUS_DEMANGLE)
#include <cxxabi.h>
#endif

char * sci_demangle(const char * __mangled_name, char * __output_buffer, size_t * __length, int * __status)
{
#if defined(HAVE_GLIBC_BACKTRACE) && defined(HAVE_CPLUS_DEMANGLE)
    return abi::__cxa_demangle(__mangled_name, __output_buffer, __length, __status);
#else
    *__status = -3;
    return 0;
#endif
}
