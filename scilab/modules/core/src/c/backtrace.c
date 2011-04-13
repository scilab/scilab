/*
  Copyright (C) 2006  EDF - Code Saturne
  Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU. Adapted for Scilab

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

        bt->size = tr_size - 1;

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

            char *s = tr_strings[i + 1];    /* Shift by 1 to ignore current function */

            const char *s_addr = NULL;
            const char *s_func = NULL;
            const char *s_file = NULL;

            for (l = 0; s[l] != '\0'; l++) ;

            /* Remove brackets around adress */
            for (j = l; j > 0 && s[j] != ']'; j--) ;
            if (s[j] == ']')
            {
                s[j] = '\0';
                l = j;
                for (j = l - 1; j > 0 && s[j] != '['; j--) ;
                if (s[j] == '[')
                {
                    s_addr = s + j + 1;
                    bt->s_addr[i] = malloc((strlen(s_addr) + 1) * sizeof(char));
                    if (bt->s_addr[i] != NULL)
                    {
                        strcpy(bt->s_addr[i], s_addr);
                    }
                }
            }
            if (j == 0)
            {
                continue;
            }

            /* Find function name and position (in parentheses) */
            while (j > 0 && s[j] != ')')
                j--;
            if (s[j] == ')')
            {
                s[j] = '\0';
                while (j > 0 && s[j] != '(')
                {
                    j--;
                }
                if (j > 0 && s[j] == '(')
                {
                    s_func = s + j + 1;
                    while (j > 0 && (s[j] == '(' || s[j] == ' '))
                    {
                        s[j--] = '\0';
                    }
                    bt->s_func[i] = malloc((strlen(s_func) + 1) * sizeof(char));
                    if (bt->s_func[i] != NULL)
                    {
                        strcpy(bt->s_func[i], s_func);
                    }
                }
            }
            if (j == 0)
            {
                continue;
            }

            /* Find executable or library name */

            if (s_func == NULL)
            {               /* With no function name found */
                for (j = 0; j < l && s[j] != ' '; j++) ;
                if (s[j] == ' ')
                {
                    s[j] = '\0';
                }
            }

            while (j > 0 && s[j] != '/')
            {
                j--;
            }
            if (j < l)
            {
                s_file = s + j + 1;
                bt->s_file[i] = malloc((strlen(s_file) + 1) * sizeof(char));
                if (bt->s_file[i] != NULL)
                {
                    strcpy(bt->s_file[i], s_file);
                }
            }

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
