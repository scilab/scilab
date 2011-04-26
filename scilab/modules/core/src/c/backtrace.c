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

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"

#if defined(HAVE_GLIBC_BACKTRACE)
#include <memory.h>
#include <execinfo.h>
#endif

#if defined(HAVE_GLIBC_BACKTRACE) && defined(HAVE_CPLUS_DEMANGLE)
#include <demangle.h>
#endif

#if defined(HAVE_GLIBC_BACKTRACE) && defined(__GNUC__)
#define _GNU_SOURCE
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

    int i, j, l;

    sci_backtrace_t *bt = NULL;

    /*
    ** nbIgnoreCall
    ** When signal is trapped, catching functions will be present
    ** within call stack so we shift to remove them.
    **
    */
    int nbIgnoredCall = 3;

    /* Create backtrace structure */

    bt = malloc(sizeof(sci_backtrace_t));

    if (bt != NULL)
    {

        void *tr_array[200];
        int tr_size = backtrace(tr_array, 200);
        char **tr_strings = backtrace_symbols(tr_array, tr_size);

       /* Create arrays; we use malloc() here instead of BFT_MALLOC, as a
         * backtrace is useful mainly in case of severe errors, so we avoid
         * higher level constructs as much as possible at this stage. */

        if (tr_size < 2 || tr_strings == NULL)
        {
            free(bt);
            return NULL;
        }

        bt->size = tr_size - nbIgnoredCall;

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

        for (i = 0; i < bt->size; i++)
        {
            bt->s_file[i] = NULL;
            bt->s_func[i] = NULL;
            bt->s_addr[i] = NULL;
        }

        /* Now parse backtrace strings and build arrays */

        for (i = 0; i < bt->size; i++)
        {

            char *s = tr_strings[i+nbIgnoredCall];    /* Shift by nbIgnoredCall to ignore functions */

            char *sLibName = NULL;
            char *sAddr = NULL;
            char *sFunName = NULL;

            /*
            ** Mac OS Backtrace are formated using spaces :
            ** 0   my_backtrace                        0x0000000100001d79 bft_backtrace_create + 61
            ** 1   my_backtrace                        0x0000000100000fc8 _cs_base_backtrace_print + 46
            ** 2   my_backtrace                        0x0000000100002386 bft_backtrace_print + 35
            **
            ** Linux does it with special characters : [] ()
            ** ./my_backtrace(bft_backtrace_create+0x41) [0x402055]
            ** ./my_backtrace() [0x401474]
            ** ./my_backtrace(bft_backtrace_print+0x25) [0x402645]
            */
#ifdef __APPLE__
            /* Find executable or library name */
            strtok(s, " "); // Will find first occurence
            sLibName = strtok(NULL, " "); // retrieve second occurence.
            /* Find adress */
            sAddr = strtok(NULL, " "); // retrieve third occurence.
            /* Find function name */
            sFunName = strtok(NULL, " "); // retrieve fourth occurence.
#else
            strtok(s, "[]"); // Will find first occurence
            sAddr = strtok(NULL, "[]"); // retrieve second occurence.
            strtok(s, "()"); // Will find first occurence
            sFunName = strtok(NULL, "()"); // retrieve second occurence.
            sLibName = strtok(s, "("); // Will find first occurence
#endif

            bt->s_func[i] = (sFunName != NULL ? strdup(sFunName) : NULL);
            bt->s_file[i] = (sLibName != NULL ? strdup(sLibName) : NULL);
            bt->s_addr[i] = (sAddr != NULL ? strdup(sAddr) : NULL);
        }

        /* Free temporary memory
         * (only main pointer needs to be freed according to glibc documentation) */

        free((void *)tr_strings);

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

    int i, j, l;

    if (bt != NULL)
    {

        for (i = 0; i < bt->size; i++)
        {

            char *s_cplus_func_p = NULL;
            char *s_cplus_func = NULL;
            int l2 = 0;

            if (bt->s_func[i] == NULL)
            {
                continue;
            }

            for (j = 0; bt->s_func[i][j] != '\0' && bt->s_func[i][j] != '+'; j++) ;

            if (bt->s_func[i][j] == '+')
            {
                l2 = strlen(bt->s_func[i] + j);
                bt->s_func[i][j] = '\0';
            }

            s_cplus_func_p = cplus_demangle(bt->s_func[i], auto_demangling);
            printf("%s ; %s\n", bt->s_func[i], s_cplus_func_p);

            if (s_cplus_func_p == NULL)
            {
                continue;
            }

            l = strlen(s_cplus_func_p);

            if (l == 0)
            {
                continue;
            }

            s_cplus_func = malloc(l + l2 + 1);

            if (s_cplus_func != NULL)
            {
                strncpy(s_cplus_func, s_cplus_func_p, l + 1);
                if (l2 > 0)
                {
                    bt->s_func[i][j] = '+';
                    strcpy(s_cplus_func + l, bt->s_func[i] + j);
                }
                s_cplus_func[l + l2] = '\0';
                free(bt->s_func[i]);
                bt->s_func[i] = s_cplus_func;

            }

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
