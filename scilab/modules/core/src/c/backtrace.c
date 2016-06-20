/*
  Copyright (C) 2006  EDF - Code Saturne
  Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU. Adapted for Scilab
  Copyright (C) 2011 - DIGITEO - Bruno JOFRET. Trace parse algorithm rewrite.

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

#include <assert.h>//
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"
#include "sci_malloc.h"

#if defined(HAVE_GLIBC_BACKTRACE)
#include <memory.h>
#include <execinfo.h>
#endif

#if defined(HAVE_GLIBC_BACKTRACE) && defined(__GNUC__)
#define _GNU_SOURCE
#endif

#define __USE_GNU
#ifndef _MSC_VER
#include <dlfcn.h>
#endif

#include "backtrace.h"

/*-----------------------------------------------------------------------------
 * Local type definitions
 *-----------------------------------------------------------------------------*/

/*
 * BFT backtrace descriptor
 */

struct _sci_backtrace_t
{

    int size;               /* Total depth of backtrace */

    char **s_file;          /* File names */
    char **s_func;          /* Function names */
    char **s_addr;          /* Addresses */

};

/* Associated typedef documentation (for backtrace.h) */

/*============================================================================
 * Public function definitions
 *============================================================================*/

/*
 * Build a backtrace description structure.
 *
 * @return pointer to sci_backtrace_t backtrace descriptor (NULL in case of
 *         error, or if backtracing is unavailable on this architecture).
 */

sci_backtrace_t *sci_backtrace_create(void)
{
#if defined(HAVE_GLIBC_BACKTRACE)

    sci_backtrace_t *bt = NULL;

    /* Create backtrace structure */

    bt = malloc(sizeof(sci_backtrace_t));

    if (bt != NULL)
    {

        void * tr_array[200];
        int tr_size = backtrace(tr_array, 200);
        int i = 0;
        Dl_info * infos = NULL;

        /* Create arrays; we use malloc() here instead of BFT_MALLOC, as a
          * backtrace is useful mainly in case of severe errors, so we avoid
          * higher level constructs as much as possible at this stage. */

        if (tr_size < 2)// || tr_strings == NULL)
        {
            free(bt);
            return NULL;
        }

        bt->size = tr_size;

        bt->s_file = malloc(tr_size * sizeof(char *));
        bt->s_func = malloc(tr_size * sizeof(char *));
        bt->s_addr = malloc(tr_size * sizeof(char *));

        /* If allocation has failed, free other allocated arrays, and return NULL */

        if (bt->s_file == NULL || bt->s_func == NULL || bt->s_addr == NULL)
        {

            if (bt->s_file != NULL)
            {
                free(bt->s_file);
            }
            if (bt->s_func != NULL)
            {
                free(bt->s_func);
            }
            if (bt->s_addr != NULL)
            {
                free(bt->s_addr);
            }

            free(bt);
            return NULL;

        }

        infos = (Dl_info *)MALLOC(sizeof(Dl_info));

        for (i = 0; i < bt->size; i++)
        {
            char buffer[32];
            void * p = tr_array[i];

            bt->s_file[i] = NULL;
            bt->s_func[i] = NULL;
            bt->s_addr[i] = NULL;

            if (dladdr(p, infos))
            {
                bt->s_func[i] = infos->dli_sname ? strdup(infos->dli_sname) : strdup(" ");
                bt->s_file[i] = infos->dli_fname ? strdup(infos->dli_fname) : strdup(" ");

                // we calculate the relative address in the library
                snprintf(buffer, 32, "%p", (void*)(p - infos->dli_fbase));
                bt->s_addr[i] = strdup(buffer);
            }
        }

        FREE(infos);
        infos = NULL;
    }

    return bt;

#else /* defined(HAVE_GLIBC_BACKTRACE) */
    return NULL;
#endif
}

/*
 * Free a backtrace description structure.
 *
 * @param [in, out] bt pointer to backtrace description structure.
 * @return NULL pointer.
 */

sci_backtrace_t *sci_backtrace_destroy(sci_backtrace_t * bt)
{
    int i;

    if (bt != NULL)
    {

        for (i = 0; i < bt->size; i++)
        {

            if (bt->s_file[i] != NULL)
            {
                free(bt->s_file[i]);
            }
            if (bt->s_func[i] != NULL)
            {
                free(bt->s_func[i]);
            }
            if (bt->s_addr[i] != NULL)
            {
                free(bt->s_addr[i]);
            }

        }

        if (bt->s_file != NULL)
        {
            free(bt->s_file);
        }
        if (bt->s_func != NULL)
        {
            free(bt->s_func);
        }
        if (bt->s_addr != NULL)
        {
            free(bt->s_addr);
        }

        free(bt);

    }

    return NULL;
}

/*
 * Demangle a backtrace description structure (for C++).
 *
 * @param [in, out] bt pointer to backtrace description structure.
 */

void sci_backtrace_demangle(sci_backtrace_t * bt)
{
#if defined(HAVE_GLIBC_BACKTRACE) && defined(HAVE_CPLUS_DEMANGLE)
    int i, l;

    if (bt != NULL)
    {
        for (i = 0; i < bt->size; i++)
        {

            char *s_cplus_func_p = NULL;
            char *s_cplus_func = NULL;
            size_t funcnamesize = 0;
            int status = 0;

            if (bt->s_func[i] == NULL)
            {
                continue;
            }

            s_cplus_func_p = sci_demangle(bt->s_func[i], NULL, &funcnamesize, &status);

            if (s_cplus_func_p == NULL)
            {
                continue;
            }

            if (status)
            {
                free(s_cplus_func_p);
                continue;
            }

            l = strlen(s_cplus_func_p);

            if (l == 0)
            {
                free(s_cplus_func_p);
                continue;
            }

            s_cplus_func = malloc(l + 1);

            if (s_cplus_func != NULL)
            {
                strncpy(s_cplus_func, s_cplus_func_p, l + 1);
                s_cplus_func[l] = '\0';
                free(bt->s_func[i]);
                bt->s_func[i] = s_cplus_func;
            }

            free(s_cplus_func_p);
        }

    }

#endif /* defined(HAVE_GLIBC_BACKTRACE) && defined(HAVE_CPLUS_DEMANGLE) */
}

/*
 * Return the depth of a backtrace.
 *
 * @param [in] bt pointer to backtrace description structure.
 * @return backtrace depth.
 */

int sci_backtrace_size(const sci_backtrace_t * bt)
{
    return bt->size;
}

/*
 * Return file name associated with a backtrace at a given depth.
 *
 * @param [in] bt pointer to backtrace description structure.
 * @param [in] depth index in backtrace structure (< sci_backtrace_size(bt)).
 * @return file name at the given depth, or NULL.
 */

const char *sci_backtrace_file(sci_backtrace_t * bt, int depth)
{
    const char *retval = NULL;

    if (bt != NULL)
    {
        if (depth < bt->size)
        {
            retval = bt->s_file[depth];
        }
    }

    return retval;
}

/*
 * Return function name associated with a backtrace at a given depth.
 *
 * @param [in] bt pointer to backtrace description structure.
 * @param [in] depth index in backtrace structure (< sci_backtrace_size(bt)).
 * @return function name at the given depth, or NULL.
 */

const char *sci_backtrace_function(sci_backtrace_t * bt, int depth)
{
    const char *retval = NULL;

    if (bt != NULL)
    {
        if (depth < bt->size)
        {
            retval = bt->s_func[depth];
        }
    }

    return retval;
}

/*
 * Return address associated with a backtrace at a given depth.
 *
 * @param [in] bt pointer to backtrace description structure.
 * @param [in] depth index in backtrace structure (< sci_backtrace_size(bt)).
 * @return  address at the given depth, or NULL.
 */

const char *sci_backtrace_address(sci_backtrace_t * bt, int depth)
{
    const char *retval = NULL;

    if (bt != NULL)
    {
        if (depth < bt->size)
        {
            retval = bt->s_addr[depth];
        }
    }

    return retval;
}
