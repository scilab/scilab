/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2009
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

/*-------------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <errno.h>
#include <pcre.h>
#include "sci_malloc.h"
#include "BOOL.h"
#include "pcre_private.h"
#include "os_string.h"
#include "strsubst.h"
#include "configvariable_interface.h"
#include "sciprint.h"
#include "charEncoding.h"
#include "freeArrayOfString.h"
/*-------------------------------------------------------------------------------*/
/* A number of things vary for Windows builds. Originally, pcretest opened its
input and output without "b"; then I was told that "b" was needed in some
environments, so it was added for release 5.0 to both the input and output. (It
makes no difference on Unix-like systems.) Later I was told that it is wrong
for the input on Windows. I've now abstracted the modes into two macros that
are set here, to make it easier to fiddle with them, and removed "b" from the
input mode under Windows. */
/*-------------------------------------------------------------------------------*/
#if _MSC_VER
#include <io.h>                /* For _setmode() */
#include <fcntl.h>             /* For _O_BINARY */
#define INPUT_MODE   "r"
#define OUTPUT_MODE  "wb"
#else
#include <sys/time.h>          /* These two includes are needed */
#include <sys/resource.h>      /* for setrlimit(). */
#define INPUT_MODE   "rb"
#define OUTPUT_MODE  "wb"
#endif

#define LINK_SIZE		2

/* We have to include pcre_internal.h because we need the internal info for
displaying the results of pcre_study() and we also need to know about the
internal macros, structures, and other internal data values; pcretest has
"inside information" compared to a program that strictly follows the PCRE API.

Although pcre_internal.h does itself include pcre.h, we explicitly include it
here before pcre_internal.h so that the PCRE_EXP_xxx macros get set
appropriately for an application, not for building PCRE. */

#include <pcre.h>
#include "pcre_internal.h"

/* We need access to the data tables that PCRE uses. So as not to have to keep
two copies, we include the source file here, changing the names of the external
symbols to prevent clashes. */

#define _pcre_utf8_table1      utf8_table1
#define _pcre_utf8_table1_size utf8_table1_size
#define _pcre_utf8_table2      utf8_table2
#define _pcre_utf8_table3      utf8_table3
#define _pcre_utf8_table4      utf8_table4
#define _pcre_utt              utt
#define _pcre_utt_size         utt_size
#define _pcre_utt_names        utt_names
#define _pcre_OP_lengths       OP_lengths

#include "pcre_tables.c"

/* It is possible to compile this test program without including support for
testing the POSIX interface, though this is not available via the standard
Makefile. */

#if !defined NOPOSIX
#include "pcreposix.h"
#endif

/* It is also possible, for the benefit of the version currently imported into
Exim, to build pcretest without support for UTF8 (define NOUTF8), without the
interface to the DFA matcher (NODFA), and without the doublecheck of the old
"info" function (define NOINFOCHECK). In fact, we automatically cut out the
UTF8 support if PCRE is built without it. */

#ifndef SUPPORT_UTF8
#ifndef NOUTF8
#define NOUTF8
#endif
#endif
/*-------------------------------------------------------------------------------*/
/* Static variables */

static int callout_count = 0;
static int callout_fail_count = 0;
static int callout_fail_id = 0;

/* The buffers grow automatically if very long input lines are encountered. */

char *buffer = NULL;


static int check_match_limit(pcre *re, pcre_extra *extra, char *bptr, int len,
                             int start_offset, int options, int *use_offsets, int use_size_offsets,
                             int flag, unsigned long int *limit, int errnumber);


/*************************************************
*        Check match or recursion limit          *
*************************************************/

static int check_match_limit(pcre *re, pcre_extra *extra, char *bptr, int len,
                             int start_offset, int options, int *use_offsets, int use_size_offsets,
                             int flag, unsigned long int *limit, int errnumber)
{
    int count;
    int min = 0;
    int mid = 64;
    int max = -1;

    extra->flags |= flag;

    for (;;)
    {
        *limit = mid;

        count = pcre_exec(re, extra, (char *)bptr, len, start_offset, options,
                          use_offsets, use_size_offsets);

        if (count == errnumber)
        {
            min = mid;
            mid = (mid == max - 1) ? max : (max > 0) ? (min + max) / 2 : mid * 2;
        }

        else if (count >= 0 || count == PCRE_ERROR_NOMATCH ||
                 count == PCRE_ERROR_PARTIAL)
        {
            if (mid == min + 1)
            {
                break;
            }
            max = mid;
            mid = (min + mid) / 2;
        }
        else
        {
            break;    /* Some other error */
        }
    }

    extra->flags &= ~flag;
    return count;
}


/*************************************************
*               Algorithm                      *
*************************************************/

/* Read lines from named file or stdin and write to named file or stdout; lines
consist of a regular expression, in delimiters and optionally followed by
options, followed by a set of test data, terminated by an empty line. */

pcre_error_code pcre_private(char *INPUT_LINE, char *INPUT_PAT, int *Output_Start, int *Output_End, char*** _pstCapturedString, int* _piCapturedStringCount)
{
    /* ALL strings are managed as UTF-8 by default */
    int options = PCRE_UTF8;
    int size_offsets = 45;
    int size_offsets_max;
    int *offsets = NULL;
    int all_use_dfa = 0;
    BOOL LOOP_PCRE_TST = FALSE;

    /* These vectors store, end-to-end, a list of captured substring names. Assume
    that 1024 is plenty long enough for the few names we'll be testing. */

    char copynames[1024];
    char getnames[1024];

    char *copynamesptr = NULL;
    char *getnamesptr = NULL;

    int rc = 0;
    (void)pcre_config(PCRE_CONFIG_UTF8, &rc);
    if (rc != 1)
    {
        return UTF8_NOT_SUPPORTED;
    }

    /* bug 3891 */
    /* backslash characters are not interpreted for input */
    buffer = strsub(INPUT_LINE, "\\", "\\\\");

    size_offsets_max = size_offsets;
    offsets = (int *)MALLOC(size_offsets_max * sizeof(int));
    if (offsets == NULL)
    {
        if (buffer)
        {
            FREE(buffer);
            buffer = NULL;
        }
        return NOT_ENOUGH_MEMORY_FOR_VECTOR;
    }
    /* Main loop */
    LOOP_PCRE_TST = FALSE;
    while (!LOOP_PCRE_TST)
    {
        pcre *re = NULL;
        pcre_extra *extra = NULL;
        const char *error = NULL;
        char *back_p = NULL;
        char *p = NULL;
        char *pp = NULL;
        char *ppp = NULL;
        const unsigned char *tables = NULL;
        int do_G = 0;
        int do_g = 0;
        int erroroffset = 0, len = 0, delimiter;

        LOOP_PCRE_TST = TRUE;
        p = os_strdup(INPUT_PAT);
        back_p = p;
        while (isspace(*p))
        {
            p++;
        }
        if (*p == 0)
        {
            FREE(back_p);
            continue;
        }
        /* In-line pattern (the usual case). Get the delimiter and seek the end of
        the pattern; if is isn't complete, read more. */

        delimiter = *p++;

        if (isalnum(delimiter) || delimiter == '\\')
        {
            if (buffer)
            {
                FREE(buffer);
                buffer = NULL;
            }
            if (offsets)
            {
                FREE(offsets);
                offsets = NULL;
            }
            if (back_p)
            {
                FREE(back_p);
                back_p = NULL;
            }
            return DELIMITER_NOT_ALPHANUMERIC;
        }

        pp = p;

        while (*pp != 0)
        {
            if (*pp == '\\' && pp[1] != 0)
            {
                pp++;
            }
            else if (*pp == delimiter)
            {
                break;
            }
            pp++;
        }

        /* If the delimiter can't be found, it's a syntax error */
        if (*pp == 0)
        {
            if (buffer)
            {
                FREE(buffer);
                buffer = NULL;
            }
            if (offsets)
            {
                FREE(offsets);
                offsets = NULL;
            }
            if (back_p)
            {
                FREE(back_p);
                back_p = NULL;
            }
            if (offsets)
            {
                FREE(offsets);
            }
            return CAN_NOT_COMPILE_PATTERN;
        }

        /* If the first character after the delimiter is backslash, make
        the pattern end with backslash. This is purely to provide a way
        of testing for the error message when a pattern ends with backslash. */

        if (pp[1] == '\\')
        {
            *pp++ = '\\';
        }

        /* Terminate the pattern at the delimiter, and save a copy of the pattern
        for callouts. */

        *pp++ = 0;

        /* Look for options after final delimiter */

        //options = 8192;

        while (*pp != 0)
        {
            switch (*pp++)
            {
                case 'f':
                    options |= PCRE_FIRSTLINE;
                    break;
                case 'g':
                    do_g = 1;
                    break;
                case 'i':
                    options |= PCRE_CASELESS;
                    break;
                case 'm':
                    options |= PCRE_MULTILINE;
                    break;
                case 's':
                    options |= PCRE_DOTALL;
                    break;
                case 'x':
                    options |= PCRE_EXTENDED;
                    break;
                case '+':
                    break;
                case 'A':
                    options |= PCRE_ANCHORED;
                    break;
                case 'B':
                    break;
                case 'C':
                    options |= PCRE_AUTO_CALLOUT;
                    break;
                case 'D':
                    break;
                case 'E':
                    options |= PCRE_DOLLAR_ENDONLY;
                    break;
                case 'F':
                    break;
                case 'G':
                    do_G = 1;
                    break;
                case 'I':
                    break;
                case 'J':
                    options |= PCRE_DUPNAMES;
                    break;
                case 'M':
                    break;
                case 'N':
                    options |= PCRE_NO_AUTO_CAPTURE;
                    break;
                case 'S':
                    break;
                case 'U':
                    options |= PCRE_UNGREEDY;
                    break;
                case 'X':
                    options |= PCRE_EXTRA;
                    break;
                case 'Z':
                    break;
                case '8':
                {
                    int rc = 0;
                    (void)pcre_config(PCRE_CONFIG_UTF8, &rc);
                    if (rc != 1)
                    {
                        if (buffer)
                        {
                            FREE(buffer);
                            buffer = NULL;
                        }
                        if (back_p)
                        {
                            FREE(back_p);
                            back_p = NULL;
                        }
                        if (offsets)
                        {
                            FREE(offsets);
                        }
                        return UTF8_NOT_SUPPORTED;
                    }
                    options |= PCRE_UTF8;
                }
                break;
                case '?':
                    options |= PCRE_NO_UTF8_CHECK;
                    break;
                case 'L':
                    ppp = pp;
                    /* The '\r' test here is so that it works on Windows. */
                    /* The '0' test is just in case this is an unterminated line. */
                    while (*ppp != 0 && *ppp != '\n' && *ppp != '\r' && *ppp != ' ')
                    {
                        ppp++;
                    }
                    *ppp = 0;
                    if (setlocale(LC_CTYPE, (const char *)pp) == NULL)
                    {
                        goto SKIP_DATA;
                    }

                    tables = pcre_maketables();
                    pp = ppp;
                    break;
                case '>':
                    while (*pp != 0)
                    {
                        pp++;
                    }
                    while (isspace(pp[-1]))
                    {
                        pp--;
                    }
                    *pp = 0;
                    break;
                case '<':
                {
                    while (*pp++ != '>')
                    {
                        ;
                    }
                }
                break;
                case '\r':                      /* So that it works in Windows */
                case '\n':
                case ' ':
                    break;

                default:
                    goto SKIP_DATA;
            }
        }

        /* Handle compiling via the POSIX interface, which doesn't support the
        timing, showing, or debugging options, nor the ability to pass over
        local character tables. */


        {
            re = pcre_compile((char *)p, options, &error, &erroroffset, tables);
            /* Compilation failed; go back for another re, skipping to blank line
            if non-interactive. */
            if (re == NULL)
            {
SKIP_DATA:
                if (buffer)
                {
                    FREE(buffer);
                    buffer = NULL;
                }
                if (offsets)
                {
                    FREE(offsets);
                    offsets = NULL;
                }
                if (tables)
                {
                    (*pcre_free)((void*)tables);
                    tables = NULL;
                }
                if (extra)
                {
                    FREE(extra);
                    extra = NULL;
                }
                if (back_p)
                {
                    FREE(back_p);
                    back_p = NULL;
                }
                return CAN_NOT_COMPILE_PATTERN;
            }

        }        /* End of non-POSIX compile */

        /* Read data lines and test them */
        {
            char *q = NULL;
            char *bptr = NULL;
            int *use_offsets = offsets;
            int use_size_offsets = size_offsets;
            int callout_data = 0;
            int callout_data_set = 0;
            int count = 0;
            int c = 0;
            int copystrings = 0;
            int find_match_limit = 0;
            int getstrings = 0;
            int gmatched = 0;
            int start_offset = 0;
            int g_notempty = 0;
            int use_dfa = 0;

            options = 0;
            *copynames = 0;
            *getnames = 0;

            copynamesptr = copynames;
            getnamesptr = getnames;

            callout_count = 0;
            callout_fail_count = 999999;
            callout_fail_id = -1;

            if (extra != NULL)
            {
                extra->flags &= ~(PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION);
            }
            p = buffer;
            bptr = q = buffer;
            while ((c = *p++) != 0)
            {
                int i = 0;
                int n = 0;

                if (c == '\\') switch ((c = *p++))
                    {
                        case 'a':
                            c =    7;
                            break;
                        case 'b':
                            c = '\b';
                            break;
                        case 'e':
                            c =   27;
                            break;
                        case 'f':
                            c = '\f';
                            break;
                        case 'n':
                            c = '\n';
                            break;
                        case 'r':
                            c = '\r';
                            break;
                        case 't':
                            c = '\t';
                            break;
                        case 'v':
                            c = '\v';
                            break;
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                            c -= '0';
                            while (i++ < 2 && isdigit(*p) && *p != '8' && *p != '9')
                            {
                                c = c * 8 + *p++ - '0';
                            }
                            break;
                        case 'x':
                            /* Ordinary \x */
                            c = 0;
                            while (i++ < 2 && isxdigit(*p))
                            {
                                c = c * 16 + tolower(*p) - ((isdigit(*p)) ? '0' : 'W');
                                p++;
                            }
                            break;
                        case 0:   /* \ followed by EOF allows for an empty line */
                            p--;
                            continue;
                        case '>':
                            while (isdigit(*p))
                            {
                                start_offset = start_offset * 10 + *p++ - '0';
                            }
                            continue;
                        case 'A':  /* Option setting */
                            options |= PCRE_ANCHORED;
                            continue;
                        case 'B':
                            options |= PCRE_NOTBOL;
                            continue;
                        case 'C':
                            if (isdigit(*p))    /* Set copy string */
                            {
                                while (isdigit(*p))
                                {
                                    n = n * 10 + *p++ - '0';
                                }
                                copystrings |= 1 << n;
                            }
                            else if (isalnum(*p))
                            {
                                char *npp = copynamesptr;
                                while (isalnum(*p))
                                {
                                    *npp++ = *p++;
                                }
                                *npp++ = 0;
                                *npp = 0;
                                pcre_get_stringnumber(re, (char *)copynamesptr);
                                copynamesptr = npp;
                            }
                            else if (*p == '+')
                            {
                                p++;
                            }
                            else if (*p == '-')
                            {
                                p++;
                            }
                            else if (*p == '!')
                            {
                                callout_fail_id = 0;
                                p++;
                                while (isdigit(*p))
                                {
                                    callout_fail_id = callout_fail_id * 10 + *p++ - '0';
                                }
                                callout_fail_count = 0;
                                if (*p == '!')
                                {
                                    p++;
                                    while (isdigit(*p))
                                    {
                                        callout_fail_count = callout_fail_count * 10 + *p++ - '0';
                                    }
                                }
                            }
                            else if (*p == '*')
                            {
                                int sign = 1;
                                callout_data = 0;
                                if (*(++p) == '-')
                                {
                                    sign = -1;
                                    p++;
                                }
                                while (isdigit(*p))
                                {
                                    callout_data = callout_data * 10 + *p++ - '0';
                                }
                                callout_data *= sign;
                                callout_data_set = 1;
                            }
                            continue;
                        case 'G':
                            if (isdigit(*p))
                            {
                                while (isdigit(*p))
                                {
                                    n = n * 10 + *p++ - '0';
                                }
                                getstrings |= 1 << n;
                            }
                            else if (isalnum(*p))
                            {
                                char *npp = getnamesptr;
                                while (isalnum(*p))
                                {
                                    *npp++ = *p++;
                                }
                                *npp++ = 0;
                                *npp = 0;
                                pcre_get_stringnumber(re, (char *)getnamesptr);
                                getnamesptr = npp;
                            }
                            continue;
                        case 'L':
                            continue;
                        case 'M':
                            find_match_limit = 1;
                            continue;
                        case 'N':
                            options |= PCRE_NOTEMPTY;
                            continue;
                        case 'O':
                            while (isdigit(*p))
                            {
                                n = n * 10 + *p++ - '0';
                            }
                            if (n > size_offsets_max)
                            {
                                size_offsets_max = n;
                                if (offsets)
                                {
                                    FREE(offsets);
                                }
                                use_offsets = offsets = (int *)MALLOC(size_offsets_max * sizeof(int));
                            }
                            use_size_offsets = n;
                            if (n == 0)
                            {
                                use_offsets = NULL;    /* Ensures it can't write to it */
                            }
                            continue;
                        case 'P':
                            options |= PCRE_PARTIAL;
                            continue;
                        case 'Q':
                            while (isdigit(*p))
                            {
                                n = n * 10 + *p++ - '0';
                            }
                            if (extra == NULL)
                            {
                                extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
                                extra->flags = 0;
                            }
                            extra->flags |= PCRE_EXTRA_MATCH_LIMIT_RECURSION;
                            extra->match_limit_recursion = n;
                            continue;
                        case 'q':
                            while (isdigit(*p))
                            {
                                n = n * 10 + *p++ - '0';
                            }
                            if (extra == NULL)
                            {
                                extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
                                extra->flags = 0;
                            }
                            extra->flags |= PCRE_EXTRA_MATCH_LIMIT;
                            extra->match_limit = n;
                            continue;
#if !defined NODFA
                        case 'R':
                            options |= PCRE_DFA_RESTART;
                            continue;
#endif
                        case 'S':

                            continue;
                        case 'Z':
                            options |= PCRE_NOTEOL;
                            continue;
                        case '?':
                            options |= PCRE_NO_UTF8_CHECK;
                            continue;
                        case '<':
                        {
                            while (*p++ != '>')
                            {
                                ;
                            }
                        }
                        continue;
                    }
                *q++ = (char)c;
            }
            *q = 0;
            len = (int)(q - buffer);
            if ((all_use_dfa || use_dfa) && find_match_limit)
            {
                if (buffer)
                {
                    FREE(buffer);
                    buffer = NULL;
                }
                if (offsets)
                {
                    FREE(offsets);
                    offsets = NULL;
                }
                if (p)
                {
                    FREE(p);
                    p = NULL;
                }
                if (re)
                {
                    (*pcre_free)(re);
                    re = NULL;
                }
                if (tables)
                {
                    (*pcre_free)((void*)tables);
                    tables = NULL;
                }
                if (extra)
                {
                    FREE(extra);
                    extra = NULL;
                }
                return LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING;
            }
            /* Handle matching via the POSIX interface, which does not
            support timing or playing with the match limit or callout data. */
            for (;; gmatched++)    /* Loop for /g or /G */
            {

                /* If find_match_limit is set, we want to do repeated matches with
                varying limits in order to find the minimum value for the match limit and
                for the recursion limit. */

                if (find_match_limit)
                {
                    if (extra == NULL)
                    {
                        extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
                        extra->flags = 0;
                    }

                    (void)check_match_limit(re, extra, bptr, len, start_offset,
                                            options | g_notempty, use_offsets, use_size_offsets,
                                            PCRE_EXTRA_MATCH_LIMIT, &(extra->match_limit),
                                            PCRE_ERROR_MATCHLIMIT);

                    count = check_match_limit(re, extra, bptr, len, start_offset,
                                              options | g_notempty, use_offsets, use_size_offsets,
                                              PCRE_EXTRA_MATCH_LIMIT_RECURSION, &(extra->match_limit_recursion),
                                              PCRE_ERROR_RECURSIONLIMIT);
                }
                /* If callout_data is set, use the interface with additional data */
                else if (callout_data_set)
                {
                    if (extra == NULL)
                    {
                        extra = (pcre_extra *)MALLOC(sizeof(pcre_extra));
                        extra->flags = 0;
                    }
                    extra->flags |= PCRE_EXTRA_CALLOUT_DATA;
                    extra->callout_data = &callout_data;
                    count = pcre_exec(re, extra, (char *)bptr, len, start_offset,
                                      options | g_notempty, use_offsets, use_size_offsets);

                    extra->flags &= ~PCRE_EXTRA_CALLOUT_DATA;
                }
                /* The normal case is just to do the match once, with the default
                value of match_limit. */
                else
                {
                    count = pcre_exec(re, extra, (char *)bptr, len,
                                      start_offset, options | g_notempty, use_offsets, use_size_offsets);
                    if (count == 0)
                    {
                        count = use_size_offsets / 3;
                    }

                    //to retrieve backref count and values
                    if (count > 0 && _pstCapturedString != NULL && _piCapturedStringCount != NULL)
                    {
                        int i = 0;
                        int iErr = 0;

                        iErr = pcre_fullinfo(re, extra, PCRE_INFO_CAPTURECOUNT, _piCapturedStringCount);
                        //sciprint("PCRE_INFO_CAPTURECOUNT %d\n", *_piCapturedStringCount);

                        if (*_piCapturedStringCount > 0)
                        {
                            *_pstCapturedString = (char**)MALLOC(sizeof(char*) * *_piCapturedStringCount);
                            for (i = 0 ; i < *_piCapturedStringCount ; i++)
                            {
                                const char* pstSubstring = NULL;
                                pcre_get_substring(bptr, use_offsets, count, i + 1, &pstSubstring);
                                if (pstSubstring != NULL)
                                {
                                    (*_pstCapturedString)[i] = os_strdup(pstSubstring);
                                }
                                else
                                {
                                    //empty string is matching, so create it
                                    (*_pstCapturedString)[i] = os_strdup("");
                                }

                                pcre_free_substring(pstSubstring);
                            }
                        }
                    }
                }
                /* Matched */
                if (count >= 0)
                {
                    int i, maxcount;
                    maxcount = use_size_offsets / 3;
                    /* This is a check against a lunatic return value. */
                    if (count > maxcount)
                    {
                        if (buffer)
                        {
                            FREE(buffer);
                            buffer = NULL;
                        }
                        if (offsets)
                        {
                            FREE(offsets);
                            offsets = NULL;
                        }
                        if (re)
                        {
                            (*pcre_free)(re);
                            re = NULL;
                        }
                        if (tables)
                        {
                            (*pcre_free)((void*)tables);
                            tables = NULL;
                        }
                        if (extra)
                        {
                            FREE(extra);
                            extra = NULL;
                        }
                        if (back_p)
                        {
                            FREE(back_p);
                            back_p = NULL;
                        }
                        return TOO_BIG_FOR_OFFSET_SIZE;
                    }

                    for (i = 0; i < count * 2; i += 2)
                    {
                        if (use_offsets[i] >= 0)
                        {
                            *Output_Start = use_offsets[i];
                            *Output_End = use_offsets[i + 1];
                            if (buffer)
                            {
                                FREE(buffer);
                            }

                            /* use_offsets = offsets no need to free use_offsets if we free offsets */
                            if (offsets)
                            {
                                FREE(offsets);
                            }

                            /* "re" allocated by pcre_compile (better to use free function associated)*/
                            if (re)
                            {
                                (*pcre_free)(re);
                            }

                            if (extra)
                            {
                                FREE(extra);
                            }
                            if (tables)
                            {
                                /* "tables" allocated by pcre_maketables (better to use free function associated to pcre)*/
                                (*pcre_free)((void *)tables);
                                tables = NULL;
                                setlocale(LC_CTYPE, "C");
                            }

                            if (back_p)
                            {
                                FREE(back_p);
                                back_p = NULL;
                            }
                            return PCRE_FINISHED_OK;
                        }
                    }

                    for (copynamesptr = copynames; *copynamesptr != 0; copynamesptr += (int)strlen((char*)copynamesptr) + 1)
                    {
                        char copybuffer[256];
                        pcre_copy_named_substring(re, (char *)bptr, use_offsets, count, (char *)copynamesptr, copybuffer, sizeof(copybuffer));
                    }

                    for (i = 0; i < 32; i++)
                    {
                        if ((getstrings & (1 << i)) != 0)
                        {
                            const char *substring;
                            pcre_get_substring((char *)bptr, use_offsets, count, i, &substring);
                        }
                    }

                    for (getnamesptr = getnames; *getnamesptr != 0; getnamesptr += (int)strlen((char*)getnamesptr) + 1)
                    {
                        const char *substring;
                        pcre_get_named_substring(re, (char *)bptr, use_offsets, count, (char *)getnamesptr, &substring);
                    }

                }
                /* Failed to match. If this is a /g or /G loop and we previously set
                g_notempty after a null match, this is not necessarily the end. We want
                to advance the start offset, and continue. We won't be at the end of the
                string - that was checked before setting g_notempty.
                Complication arises in the case when the newline option is "any" or
                "anycrlf". If the previous match was at the end of a line terminated by
                CRLF, an advance of one character just passes the \r, whereas we should
                prefer the longer newline sequence, as does the code in pcre_exec().
                Fudge the offset value to achieve this.

                Otherwise, in the case of UTF-8 matching, the advance must be one
                character, not one byte. */
                else
                {
                    if (count == PCRE_ERROR_NOMATCH)
                    {
                        if (gmatched == 0)
                        {
                            if (tables)
                            {
                                (*pcre_free)((void *)tables);
                                tables = NULL;
                            }
                            if (re)
                            {
                                (*pcre_free)((void *)re);
                                re = NULL;
                            }
                            if (buffer)
                            {
                                FREE(buffer);
                                buffer = NULL;
                            }
                            if (offsets)
                            {
                                FREE(offsets);
                            }
                            if (p)
                            {
                                FREE(back_p);
                                back_p = NULL;
                            }
                            return NO_MATCH;
                        }
                    }

                    if (count == PCRE_ERROR_MATCHLIMIT )
                    {
                        if (tables)
                        {
                            (*pcre_free)((void *)tables);
                            tables = NULL;
                        }
                        if (re)
                        {
                            (*pcre_free)((void *)re);
                            re = NULL;
                        }
                        if (buffer)
                        {
                            FREE(buffer);
                            buffer = NULL;
                        }
                        if (offsets)
                        {
                            FREE(offsets);
                            offsets = NULL;
                        }
                        if (back_p)
                        {
                            FREE(back_p);
                            back_p = NULL;
                        }
                        return MATCH_LIMIT;
                    }
                    break;  /* Out of loop */
                }

                /* If not /g or /G we are done */
                if (!do_g && !do_G)
                {
                    break;
                }

                /* If we have matched an empty string, first check to see if we are at
                the end of the subject. If so, the /g loop is over. Otherwise, mimic
                what Perl's /g options does. This turns out to be rather cunning. First
                we set PCRE_NOTEMPTY and PCRE_ANCHORED and try the match again at the
                same point. If this fails (picked up above) we advance to the next
                character. */

                g_notempty = 0;

                if (use_offsets[0] == use_offsets[1])
                {
                    if (use_offsets[0] == len)
                    {
                        break;
                    }
                    g_notempty = PCRE_NOTEMPTY | PCRE_ANCHORED;
                }

                /* For /g, update the start offset, leaving the rest alone */

                if (do_g)
                {
                    start_offset = use_offsets[1];
                }
                /* For /G, update the pointer and length */
                else
                {
                    bptr += use_offsets[1];
                    len -= use_offsets[1];
                }
            }  /* End of loop for /g and /G */

            if (re)
            {
                (*pcre_free)(re);
                re = NULL;
            }
            if (extra)
            {
                FREE(extra);
                extra = NULL;
            }
            if (tables)
            {
                (*pcre_free)((void *)tables);
                tables = NULL;
            }

            FREE(back_p);
            back_p = NULL;
            continue;
        }    /* End of loop for data lines */
    }

    if (buffer)
    {
        FREE(buffer);
        buffer = NULL;
    }
    if (offsets)
    {
        FREE(offsets);
        offsets = NULL;
    }

    return PCRE_EXIT;
}
/*-------------------------------------------------------------------------------*/
pcre_error_code wide_pcre_private(wchar_t* _pwstInput, wchar_t* _pwstPattern, int* _piStart, int* _piEnd, wchar_t*** _pstCapturedString, int* _piCapturedStringCount)
{
    pcre_error_code iPcreStatus = PCRE_FINISHED_OK;
    int i               = 0;
    int iStart          = 0;
    int iEnd            = 0;

    char* pstInput      = wide_string_to_UTF8(_pwstInput);
    char* pstPattern    = wide_string_to_UTF8(_pwstPattern);
    char** pstCaptured  = NULL;//(char**)MALLOC(sizeof(char*) * (strlen(pstInput) + 1));

    iPcreStatus = pcre_private(pstInput, pstPattern, &iStart, &iEnd, &pstCaptured, _piCapturedStringCount);
    if (iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd)
    {
        char* pstTempStart      = NULL;
        char* pstTempEnd        = NULL;
        wchar_t* pwstTempStart  = NULL;
        wchar_t* pwstTempEnd    = NULL;

        pstTempStart            = os_strdup(pstInput);
        pstTempEnd              = os_strdup(pstInput);
        pstTempEnd[iEnd]        = 0;
        pstTempStart[iStart]    = 0;


        pwstTempStart           = to_wide_string(pstTempStart);
        pwstTempEnd             = to_wide_string(pstTempEnd);

        *_piStart               = (int)wcslen(pwstTempStart);
        *_piEnd                 = (int)wcslen(pwstTempEnd);

        if (_piCapturedStringCount && *_piCapturedStringCount > 0)
        {
            /*convert captured field in wide char*/
            *_pstCapturedString = (wchar_t**)MALLOC(sizeof(wchar_t*) * *_piCapturedStringCount);
            for (i = 0 ; i < *_piCapturedStringCount ; i++)
            {
                (*_pstCapturedString)[i] = to_wide_string(pstCaptured[i]);
            }
            freeArrayOfString(pstCaptured, *_piCapturedStringCount);
        }

        FREE(pstTempStart);
        FREE(pstTempEnd);
        FREE(pwstTempStart);
        FREE(pwstTempEnd);
    }
    else
    {
        *_piStart   = iStart;
        *_piEnd     = iEnd;
        if (_piCapturedStringCount && *_piCapturedStringCount > 0)
        {
            /*free unused captured field*/
            freeArrayOfString(pstCaptured, *_piCapturedStringCount);
        }
    }

    FREE(pstInput);
    FREE(pstPattern);
    return iPcreStatus;
}
/*-------------------------------------------------------------------------------*/
