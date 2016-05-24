/*
Copyright (C) 2006  EDF - Code Saturne
Copyright (C) 2001 - DIGITEO - Sylvestre LEDRU. Adapted for Scilab

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <cstring>
#include <cstdio>
#include <sstream>
#include <string>

extern "C" {
#include "backtrace.h"
#include "backtrace_print.h"
#include "localization.h"
#include "os_string.h"
}

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#define FUNCNAMEBUFFERSIZE 256
#define BTBUFFERSIZE 1024

char * backtrace_print(int niv_debut, int unmangle)
{
    size_t ind = 0;
    sci_backtrace_t * tr = 0;
    std::stringstream ss;

    tr = sci_backtrace_create();

    if (tr)
    {
        char s_func_buf[FUNCNAMEBUFFERSIZE + 3];
        const char * s_file = NULL;
        const char * s_func = NULL;
        const char * s_addr = NULL;
        const char s_unknown[] = "?";
        const char s_empty[] = "";
        const char * s_prefix = s_empty;
        char buffer[BTBUFFERSIZE];

        if (unmangle)
        {
            sci_backtrace_demangle(tr);
        }

        size_t nbr = sci_backtrace_size(tr);

        if (nbr > 0)
        {
            ss << _("\nCall stack:\n");
        }

        for (ind = niv_debut; ind < nbr; ind++)
        {
            s_file = sci_backtrace_file(tr, (int)ind);
            s_func = sci_backtrace_function(tr, (int)ind);
            s_addr = sci_backtrace_address(tr, (int)ind);

            if (!s_file)
            {
                s_file = s_unknown;
            }

            if (!s_func)
            {
                strcpy(s_func_buf, "?");
            }
            else
            {
                s_func_buf[0] = '<';
                strncpy(s_func_buf + 1, s_func, FUNCNAMEBUFFERSIZE);
                s_func_buf[FUNCNAMEBUFFERSIZE] = '\0';
                strcat(s_func_buf, ">");
            }

            if (!s_addr)
            {
                s_addr = s_unknown;
            }

            snprintf(buffer, BTBUFFERSIZE, "%s%4lu: %-8s %-32s (%s)", s_prefix, ind - niv_debut + 1, s_addr, s_func_buf, s_file);
            ss << buffer << std::endl;
        }

        sci_backtrace_destroy(tr);

        if (nbr > 0)
        {
            ss << _("End of stack\n\n");
        }
    }

    return os_strdup(ss.str().c_str());
}
