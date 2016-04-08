/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
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
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#define _GNU_SOURCE             /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include <stdio.h>
#include <ctype.h>              /* isdigit */
#include <string.h>
#include "BOOL.h"
#include "sci_malloc.h"
#include "do_xxscanf.h"
#include "Scierror.h"
#include "localization.h"
#include "core_math.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
typedef int (*XXSCANF) (FILE* , char* , ...);
typedef int (*FLUSH) (FILE*);

/*--------------------------------------------------------------------------*/
static void set_xxscanf(FILE* fp, XXSCANF* xxscanf, char** target, wchar_t* strv)
{
    if (fp == (FILE *) 0)
    {
        *target = wide_string_to_UTF8(strv);
        *xxscanf = (XXSCANF) sscanf;
    }
    else
    {
        *target = (char*)fp;
        *xxscanf = (XXSCANF) fscanf;
    }
}
static void delete_target(FILE* fp, char* target)
{
    if (target && fp == (FILE *) 0)
    {
        FREE(target);
    }
}
/*--------------------------------------------------------------------------*/
int do_xxscanf (const wchar_t *fname, FILE *fp, const wchar_t *format, int *nargs, wchar_t *strv, int *retval, rec_entry *buf, sfdir *type)
{
    int nc[MAXSCAN];
    int n_directive_count = 0;
    int i = 0;
    int l_flag = 0;
    int h_flag = 0;
    int width_flag = 0;
    int width_val = 0;
    int ignore_flag = 0;
    int str_flag = 0;
    int num_conversion = -1;
    void *ptrtab[MAXSCAN] = {NULL};
    wchar_t sformat[MAX_STR];
    wchar_t backupcurrrentchar;
    wchar_t directive;
    wchar_t *p1 = NULL;
    char* target = NULL;
    wchar_t *sval = NULL;
    register wchar_t *currentchar = NULL;
    char* strFormat = NULL;

    XXSCANF xxscanf;

    set_xxscanf(fp, &xxscanf, &target, strv);
    currentchar = format;
    *retval = 0;

    while (TRUE)
    {
        /* scanf */
        while (*currentchar != L'%' && *currentchar != L'\0')
        {
            currentchar++;
        }
        if (*currentchar == L'%' && *(currentchar + 1) == L'%')
        {
            currentchar = currentchar + 2;
            continue;
        }

        if (*currentchar == 0)
        {
            break;
        }

        currentchar++;
        p1 = currentchar - 1;

        while (isdigit(((int)*currentchar)))
        {
            currentchar++;
        }

        width_flag = 0;

        if (p1 + 1 != currentchar)
        {
            wchar_t w = *currentchar;
            *currentchar = L'\0';
            width_flag = 1;
            swscanf(p1 + 1, L"%d", &width_val);
            *currentchar = w;
        }

        ignore_flag = 0;

        if (*currentchar == L'*')
        {
            ignore_flag = 1;
            currentchar++;
        }
        else
        {
            l_flag = h_flag = 0;
        }

        if (*currentchar == L'l')
        {
            currentchar++;
            l_flag = 1;
        }
        else if (*currentchar == L'h')
        {
            currentchar++;
            h_flag = 1;
        }

        /* directive points to the scan directive  */

        directive = *currentchar++;

        if (directive == L'[')
        {
            wchar_t *currentchar1 = currentchar--;

            while (*currentchar1 != L'\0' && *currentchar1 != L']')
            {
                currentchar1++;
            }

            if (*currentchar1 == L'\0')
            {
                delete_target(fp, target);
                Scierror(998, _("%s: An error occurred: %s\n"), fname, _("unclosed [ directive."));
                return DO_XXPRINTF_RET_BUG;
            }

            if (currentchar1 == currentchar + 1 || wcsncmp(currentchar, L"[^]", 3) == 0)
            {
                currentchar1++;
                while (*currentchar1 != L'\0' && *currentchar1 != L']')
                {
                    currentchar1++;
                }

                if (*currentchar1 == L'\0')
                {
                    delete_target(fp, target);
                    Scierror(998, _("%s: An error occurred: %s\n"), fname, _("unclosed [ directive."));
                    return DO_XXPRINTF_RET_BUG;
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
                delete_target(fp, target);
                Scierror(998, _("%s: An error occurred: too many (> %d) conversion required.\n"), fname, MAXSCAN);
                return DO_XXPRINTF_RET_BUG;
            }

            switch (directive)
            {
                case L']':
                    str_flag = 1;

                    if (width_flag == 1 && width_val > MAX_STR - 1)
                    {
                        delete_target(fp, target);
                        Scierror(998, _("%s: An error occurred: field %d is too long (> %d) for %%[ directive.\n"), fname, width_val, MAX_STR - 1);
                        return DO_XXPRINTF_RET_BUG;
                    }

                    if ((buf[num_conversion].c = (wchar_t*)MALLOC(MAX_STR * sizeof(wchar_t))) == NULL)
                    {
                        delete_target(fp, target);
                        return DO_XXPRINTF_MEM_LACK;
                    }
                    ptrtab[num_conversion] = buf[num_conversion].c;
                    type[num_conversion] = SF_S;
                    break;

                case L's':
                    if (l_flag + h_flag)
                    {
                        delete_target(fp, target);
                        Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                        return DO_XXPRINTF_RET_BUG;
                    }

                    str_flag = 1;
                    if (width_flag == 1 && width_val > MAX_STR - 1)
                    {
                        delete_target(fp, target);
                        Scierror(998, _("%s: An error occurred: field %d is too long (< %d) for %%s directive.\n"), fname, width_val, MAX_STR - 1);
                        return DO_XXPRINTF_RET_BUG;
                    }

                    if ((buf[num_conversion].c = (wchar_t*)MALLOC(MAX_STR * sizeof(wchar_t))) == NULL)
                    {
                        delete_target(fp, target);
                        return DO_XXPRINTF_MEM_LACK;
                    }

                    ptrtab[num_conversion] = buf[num_conversion].c;
                    type[num_conversion] = SF_S;
                    break;

                case L'c':
                    str_flag = 1;
                    if (l_flag + h_flag)
                    {
                        delete_target(fp, target);
                        Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                        return DO_XXPRINTF_RET_BUG;
                    }

                    if (width_flag == 1)
                    {
                        nc[num_conversion] = width_val;
                    }
                    else
                    {
                        nc[num_conversion] = 1;
                    }

                    if (width_flag == 1 && width_val > MAX_STR - 1)
                    {
                        delete_target(fp, target);
                        Scierror(998, _("%s: An error occurred: field %d is too long (< %d) for %%c directive.\n"), fname, width_val, MAX_STR - 1);
                        return DO_XXPRINTF_RET_BUG;
                    }

                    if ((buf[num_conversion].c = (wchar_t*)MALLOC(MAX_STR * sizeof(wchar_t))) == NULL)
                    {
                        delete_target(fp, target);
                        return DO_XXPRINTF_MEM_LACK;
                    }

                    ptrtab[num_conversion] = buf[num_conversion].c;
                    type[num_conversion] = SF_C;
                    break;

                case L'o':
                case L'u':
                case L'x':
                case L'X':
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

                case L'D':
                    ptrtab[num_conversion] = &buf[num_conversion].li;
                    type[num_conversion] = SF_LI;
                    break;

                case L'n':
                    n_directive_count++;
                    //pass to next statement

                case L'i':
                case L'd':
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

                case L'e':
                case L'f':
                case L'g':
                case L'E':
                case L'G':
                    if (h_flag)
                    {
                        delete_target(fp, target);
                        Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                        return DO_XXPRINTF_RET_BUG;
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
                {
                    delete_target(fp, target);
                    Scierror(998, _("%s: An error occurred: %s\n"), fname, _("Bad conversion."));
                    return DO_XXPRINTF_RET_BUG;
                }
            }
            *currentchar = backupcurrrentchar;
        }
    }

    // replace %s by %ls
    if (str_flag)
    {
        wchar_t *f1 = format;
        wchar_t *f2 = sformat;
        wchar_t *slast = sformat + MAX_STR - 1 - 4;

        int bFirst = 0;
        while (*f1 != L'\0')
        {
            int n;
            if (*(f1) == L'%')
            {
                bFirst = 1;
            }
            *f2++ = *f1++;
            while (isdigit(((int)*f1)))
            {
                *f2++ = *f1++;
            }

            if (bFirst && ( *(f1) == L's'  || *(f1) == L'[' || *(f1) == L'c'))
            {
                bFirst = 0;
                if (*(f1 - 1) != L'l' && *(f1 - 1) != L'L')
                {
                    *f2++  = L'l';
                }
            }
            if ( (*(f1) == L's' || *(f1) == L'[')
                    &&
                    (   (*(f1 - 1) == L'%')
                        ||
                        ((*(f1 - 1) == L'l' || *(f1 - 1) == L'L') && (*(f1 - 2) == L'%'))
                    )
               )
            {
                f2--;
                n = os_swprintf(f2, MAX_STR - 1, L"%d%c", MAX_STR - 1, L'l');
                f2 += n;
                *f2++ = *f1++;
            }

            if (f2 == slast)
            {
                delete_target(fp, target);
                Scierror(998, _("%s: An error occurred: format is too long (> %d).\n"), fname, MAX_STR - 1);
                return DO_XXPRINTF_RET_BUG;
            }
        }

        *f2 = L'\0';
        format = sformat;
    }

    strFormat = wide_string_to_UTF8(format);
    *retval = (*xxscanf) ((VPTR) target, strFormat,
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
    FREE(strFormat);
    delete_target(fp, target);

    *nargs = Min(num_conversion + 1, Max(*retval + n_directive_count, 0));

    for (i = 1; i <= *nargs; i++)
    {
        if (type[i - 1] == SF_C)
        {
            sval = (wchar_t *)ptrtab[i - 1];
            sval[nc[i - 1]] = L'\0';
        }
    }

    if (*retval == 0)
    {
        return DO_XXPRINTF_MISMATCH;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
