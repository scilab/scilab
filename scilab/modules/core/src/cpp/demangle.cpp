/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
