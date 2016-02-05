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
#include "sci_malloc.h"
#include "do_xxscanf.h"
#include "scanf_functions.h"
#include "cvstr.h"
#include "localization.h"
#include "Scierror.h"
/* ************************************************************************
 *   Store data scanned by a single call to do_scan in line rowcount of data
 *   table
 ************************************************************************/
int Store_Scan(int *nrow, int *ncol, sfdir *type_s, sfdir *type, int *retval, int *retval_s, rec_entry *buf, entry **data, int rowcount, int n)
{
    int i, j, nr, nc, err;
    entry * Data;
    int blk = 20; /* block size for memory allocation */
    nr = *nrow;
    nc = *ncol;

    if (rowcount == 0)
    {
        for ( i = 0 ; i < MAXSCAN ; i++)
        {
            type_s[i] = SF_F;    /* initialisation */
        }
        if (nr < 0)
        {
            nr = blk;
            *nrow = nr; /*added by S. Steer to fix bug 2453*/
        }
        nc = n;
        *ncol = nc;
        *retval_s = *retval;
        if (n == 0)
        {
            return 0;
        }
        if ( (*data = (entry *) MALLOC(nc * nr * sizeof(entry))) == NULL)
        {
            err = DO_XXPRINTF_MEM_LACK;
            goto bad1;
        }
        for ( i = 0 ; i < nc ; i++)
        {
            type_s[i] = type[i];
        }

    }
    else
    {
        /* check if number of data read match with previous number */
        if ( (n != nc ) || (*retval_s != *retval) )
        {
            err = DO_XXPRINTF_MISMATCH;
            goto bad2;
        }
        /* check if types of data read match with previous types */
        for ( i = 0 ; i < nc ; i++)
            if (type[i] != type_s[i])
            {
                err = DO_XXPRINTF_MISMATCH;
                goto bad2;
            }

        /* check for memory and REALLOC if necessary*/
        /*&&nc>0 added by S. Steer for bug 2399 fix */
        if (rowcount >= nr && nc > 0)
        {
            nr = nr + blk;
            *nrow = nr;
            if ( (*data = (entry *) REALLOC(*data, nc * nr * sizeof(entry))) == NULL)
            {
                err = DO_XXPRINTF_MEM_LACK;
                goto bad1;
            }
        }
    }
    Data = *data;
    /* store values scanned in a new row */
    for ( i = 0 ; i < nc ; i++)
    {
        switch ( type_s[i] )
        {
            case SF_C:
            case SF_S:
                Data[i + nc * rowcount].s = buf[i].c;
                break;
            case SF_LUI:
                Data[i + nc * rowcount].d = (double)buf[i].lui;
                break;
            case SF_SUI:
                Data[i + nc * rowcount].d = (double)buf[i].sui;
                break;
            case SF_UI:
                Data[i + nc * rowcount].d = (double)buf[i].ui;
                break;
            case SF_LI:
                Data[i + nc * rowcount].d = (double)buf[i].li;
                break;
            case SF_SI:
                Data[i + nc * rowcount].d = (double)buf[i].si;
                break;
            case SF_I:
                Data[i + nc * rowcount].d = (double)buf[i].i;
                break;
            case SF_LF:
                Data[i + nc * rowcount].d = buf[i].lf;
                break;
            case SF_F:
                Data[i + nc * rowcount].d = (double)buf[i].f;
                break;
        }
    } /* rowcount */
    return 0;
bad1:
    /* FREE allocated strings in scan buffer */
    for ( j = 0 ; j < MAXSCAN ; j++)
        if ( (type_s[j] ==  SF_C) || (type_s[j] ==  SF_S))
        {
            FREE(buf[j].c);
        }
    return err;
bad2:

    return err;
}
/*--------------------------------------------------------------------------*/
void Free_Scan(int nrow, int ncol, sfdir *type_s, entry **data)
{
    int i, j;
    entry * Data;
    Data = *data;

    if (nrow != 0)
    {
        for ( j = 0 ; j < ncol ; j++)
            if ( (type_s[j] ==  SF_C) || (type_s[j] ==  SF_S) )
                /* free allocated strings in scan data area */
                for ( i = 0 ; i < nrow ; i++)
                {
                    FREE(Data[j + ncol * i].s);
                }
    }
    /* free scaned data area */
    if (ncol > 0)
    {
        FREE(Data);
    }
}
/*--------------------------------------------------------------------------*/


