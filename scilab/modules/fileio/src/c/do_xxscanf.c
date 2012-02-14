/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#define _GNU_SOURCE             /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include <stdio.h>
#include <ctype.h>              /* isdigit */
#include <string.h>
#include "BOOL.h"
#include "MALLOC.h"
#include "do_xxscanf.h"
#include "Scierror.h"
#include "localization.h"
#include "do_xxprintf.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
typedef int (*XXSCANF) (FILE *, char *, ...);
typedef int (*FLUSH) (FILE *);

/*--------------------------------------------------------------------------*/
static void set_xxscanf(FILE * fp, XXSCANF * xxscanf, char **target, char **strv)
{
    if (fp == (FILE *) 0)
    {
        *target = *strv;
        *xxscanf = (XXSCANF) sscanf;
    }
    else
    {
        *target = (char *)fp;
        *xxscanf = (XXSCANF) fscanf;
    }
}

/*--------------------------------------------------------------------------*/
int do_xxscanf(char *fname, FILE * fp, char *format, int *nargs, char *strv, int *retval, rec_entry * buf, sfdir * type)
{
    int nc[MAXSCAN];
    int n_directive_count = 0;
    int i = 0;
    int l_flag = 0;
    int h_flag = 0;
    int width_flag = 0;
    int width_val = 0;
    int ignore_flag = 0;
    int str_width_flag = 0;
    int num_conversion = -1;
    void *ptrtab[MAXSCAN];
    char sformat[MAX_STR];
    char backupcurrrentchar;
    char directive;
    char *p = NULL;
    char *p1 = NULL;
    char *target = NULL;
    char *sval = NULL;
    register char *currentchar = NULL;

    XXSCANF xxscanf;

    set_xxscanf(fp, &xxscanf, &target, &strv);
    currentchar = format;
    *retval = 0;

    while (TRUE)
    {
        /* scanf */
        p = currentchar;
        while (*currentchar != '%' && *currentchar != '\0')
            currentchar++;
        if (*currentchar == '%' && *(currentchar + 1) == '%')
        {
            currentchar = currentchar + 2;
            while (*currentchar != '%' && *currentchar != '\0')
                currentchar++;
        }

        if (*currentchar == 0)
        {
            break;
        }

        currentchar++;
        p1 = currentchar - 1;

        while (isdigit(((int)*currentchar)))
            currentchar++;

        width_flag = 0;

        if (p1 + 1 != currentchar)
        {
            char w = *currentchar;

            *currentchar = '\0';
            width_flag = 1;
            sscanf(p1 + 1, "%d", &width_val);
            *currentchar = w;
        }

        ignore_flag = 0;

        if (*currentchar == '*')
        {
            ignore_flag = 1;
            currentchar++;
        }
        else
        {
            l_flag = h_flag = 0;
        }

        if (*currentchar == 'l')
        {
            currentchar++;
            l_flag = 1;
        }
        else if (*currentchar == 'h')
        {
            currentchar++;
            h_flag = 1;
        }

        /* directive points to the scan directive  */

        directive = *currentchar++;

        if (directive == '[')
        {
            char *currentchar1 = currentchar--;

            while (*currentchar1 != '\0' && *currentchar1 != ']')
                currentchar1++;

            if (*currentchar1 == '\0')
            {
                Scierror(998, _("%s: An error occurred: %s\n"), fname, _("unclosed [ directive."));
                return RET_BUG;
            }

            if (currentchar1 == currentchar + 1 || strncmp(currentchar, "[^]", 3) == 0)
            {
                currentchar1++;
                while (*currentchar1 != '\0' && *currentchar1 != ']')
                    currentchar1++;

                if (*currentchar1 == '\0')
                {
                    Scierror(998, _("%s: An error occurred: %s\n"), fname, _("unclosed [ directive."));
                    return RET_BUG;
                }
            }

            directive = *currentchar1++;
            currentchar = currentchar1;
        }

        backupcurrrentchar = *currentchar;

        if (ignore_flag != 1)
        {
            num_conversion++;

            if (num_conversion >= MAXSCAN)
            {
                Scierror(998, _("%s: An error occurred: too many (> %d) conversion required.\n"), fname, MAXSCAN);
                return RET_BUG;
            }

            switch (directive)
            {
            case ']':
                if (width_flag == 0)
                    str_width_flag = 1;

                if (width_flag == 1 && width_val > MAX_STR - 1)
                {
                    Scierror(998, _("%s: An error occurred: field %d is too long (> %d) for %%[ directive.\n"), fname, width_val, MAX_STR - 1);
                    return RET_BUG;
                }

                if ((buf[num_conversion].c = MALLOC(MAX_STR)) == NULL)
                    return MEM_LACK;
                ptrtab[num_conversion] = buf[num_conversion].c;
                type[num_conversion] = SF_S;
                break;

            case 's':
                if (l_flag + h_flag)
                {
                    Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                    return RET_BUG;
                }

                if (width_flag == 0)
                    str_width_flag = 1;
                if (width_flag == 1 && width_val > MAX_STR - 1)
                {
                    Scierror(998, _("%s: An error occurred: field %d is too long (< %d) for %%s directive.\n"), fname, width_val, MAX_STR - 1);
                    return RET_BUG;
                }

                if ((buf[num_conversion].c = MALLOC(MAX_STR)) == NULL)
                    return MEM_LACK;

                ptrtab[num_conversion] = buf[num_conversion].c;
                type[num_conversion] = SF_S;
                break;

            case 'c':
                if (l_flag + h_flag)
                {
                    Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                    return RET_BUG;
                }

                if (width_flag == 1)
                    nc[num_conversion] = width_val;
                else
                    nc[num_conversion] = 1;

                if (width_flag == 1 && width_val > MAX_STR - 1)
                {
                    Scierror(998, _("%s: An error occurred: field %d is too long (< %d) for %%c directive.\n"), fname, width_val, MAX_STR - 1);
                    return RET_BUG;
                }

                if ((buf[num_conversion].c = MALLOC(MAX_STR)) == NULL)
                    return MEM_LACK;

                ptrtab[num_conversion] = buf[num_conversion].c;
                type[num_conversion] = SF_C;
                break;

            case 'o':
            case 'u':
            case 'x':
            case 'X':
                if (l_flag)
                {
                    ptrtab[num_conversion] = &buf[num_conversion].lui;
                    type[num_conversion] = SF_LUI;
                }
                else if (h_flag)
                {
                    ptrtab[num_conversion] = &buf[num_conversion].sui;
                    type[num_conversion] = SF_SUI;
                }
                else
                {
                    ptrtab[num_conversion] = &buf[num_conversion].ui;
                    type[num_conversion] = SF_UI;
                }
                break;

            case 'D':
                ptrtab[num_conversion] = &buf[num_conversion].li;
                type[num_conversion] = SF_LI;
                break;

            case 'n':
                n_directive_count++;

            case 'i':
            case 'd':
                if (l_flag)
                {
                    ptrtab[num_conversion] = &buf[num_conversion].li;
                    type[num_conversion] = SF_LI;
                }
                else if (h_flag)
                {
                    ptrtab[num_conversion] = &buf[num_conversion].si;
                    type[num_conversion] = SF_SI;
                }
                else
                {
                    ptrtab[num_conversion] = &buf[num_conversion].i;
                    type[num_conversion] = SF_I;
                }
                break;

            case 'e':
            case 'f':
            case 'g':
            case 'E':
            case 'G':
                if (h_flag)
                {
                    Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                    return RET_BUG;
                }
                else if (l_flag)
                {
                    ptrtab[num_conversion] = &buf[num_conversion].lf;
                    type[num_conversion] = SF_LF;
                }
                else
                {
                    ptrtab[num_conversion] = &buf[num_conversion].f;
                    type[num_conversion] = SF_F;
                }
                break;

            default:
                Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                return RET_BUG;
            }
            *currentchar = backupcurrrentchar;
        }
    }

    if (str_width_flag == 1)
    {
        char *f1 = format;
        char *f2 = sformat;
        char *slast = sformat + MAX_STR - 1 - 4;

        while (*f1 != '\0')
        {
            int n;

            *f2++ = *f1++;

            if (*(f1 - 1) == '%' && (*(f1) == 's' || *(f1) == '['))
            {
                n = sprintf(f2, "%d", MAX_STR - 1);
                f2 += n;
                *f2++ = *f1++;
            }

            if (f2 == slast)
            {
                Scierror(998, _("%s: An error occurred: format is too long (> %d).\n"), fname, MAX_STR - 1);
                return RET_BUG;
            }
        }

        *f2 = '\0';
        format = sformat;
    }

    *retval = (*xxscanf) ((VPTR) target, format,
                          ptrtab[0], ptrtab[1], ptrtab[2], ptrtab[3], ptrtab[4], ptrtab[5], ptrtab[6], ptrtab[7], ptrtab[8], ptrtab[9],
                          ptrtab[10], ptrtab[11], ptrtab[12], ptrtab[13], ptrtab[14], ptrtab[15], ptrtab[16], ptrtab[17], ptrtab[18], ptrtab[19],
                          ptrtab[20], ptrtab[21], ptrtab[22], ptrtab[23], ptrtab[24], ptrtab[25], ptrtab[26], ptrtab[27], ptrtab[28], ptrtab[29],
                          ptrtab[30], ptrtab[31], ptrtab[32], ptrtab[33], ptrtab[34], ptrtab[35], ptrtab[36], ptrtab[37], ptrtab[38], ptrtab[39],
                          ptrtab[40], ptrtab[41], ptrtab[42], ptrtab[43], ptrtab[44], ptrtab[45], ptrtab[46], ptrtab[47], ptrtab[48], ptrtab[49],
                          ptrtab[50], ptrtab[51], ptrtab[52], ptrtab[53], ptrtab[54], ptrtab[55], ptrtab[56], ptrtab[57], ptrtab[58], ptrtab[59],
                          ptrtab[60], ptrtab[61], ptrtab[62], ptrtab[63], ptrtab[64], ptrtab[65], ptrtab[66], ptrtab[67], ptrtab[68], ptrtab[69],
                          ptrtab[70], ptrtab[71], ptrtab[72], ptrtab[73], ptrtab[74], ptrtab[75], ptrtab[76], ptrtab[77], ptrtab[78], ptrtab[79],
                          ptrtab[80], ptrtab[81], ptrtab[82], ptrtab[83], ptrtab[84], ptrtab[85], ptrtab[86], ptrtab[87], ptrtab[88], ptrtab[89],
                          ptrtab[90], ptrtab[91], ptrtab[92], ptrtab[93], ptrtab[94], ptrtab[95], ptrtab[96], ptrtab[97], ptrtab[98],
                          ptrtab[MAXSCAN - 1]);

    *nargs = Min(num_conversion + 1, Max(*retval + n_directive_count, 0));

    for (i = 1; i <= *nargs; i++)
    {
        if (type[i - 1] == SF_C)
        {
            sval = (char *)ptrtab[i - 1];
            sval[nc[i - 1]] = '\0';
        }
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
