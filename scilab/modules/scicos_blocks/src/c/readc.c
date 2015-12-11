/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "scicos_print.h"
#include "machine.h"
#include "charEncoding.h"
#include "mget.h"
#include "sci_malloc.h"
#include "localization.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
static int worldsize(char type[4]);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void readc(int *flag, int *nevprt,
                                double *t, double xd[],
                                double x[], int *nx,
                                double z[], int *nz,
                                double tvec[], int *ntvec,
                                double rpar[], int *nrpar,
                                int ipar[], int *nipar,
                                double *inptr[], int insz[],
                                int *nin, double *outptr[],
                                int outsz[], int *nout)
/*
ipar[1]   = lfil : file name length
ipar[2:4] = fmt  : numbers type ascii code
ipar[5]   = is there a time record
ipar[6]   = n : buffer length in number of records
ipar[7]   = maxvoie : record size
ipar[8]   = swap
ipar[9]   = first : first record to read
ipar[10:9+lfil] = character codes for file name
ipar[10+lfil:9+lfil++ny+ievt] = reading mask
*/
{
    char str[100], type[4];
    int job = 1, three = 3;
    FILE *fd = NULL;
    int n = 0, k = 0, ievt = 0, kmax = 0, m = 0, i = 0, irep = 0, ierr = 0;
    double *buffer = NULL, *record = NULL;
    int *mask = NULL;
    long offset ;


    --ipar;
    --z;
    fd = (FILE *)(long)z[3];
    buffer = (z + 4);
    mask = ipar + 11 + ipar[1] - ipar[5];

    /*
    k    : record counter within the buffer
    kmax :  number of records in the buffer
    */

    if (*flag == 1)
    {
        n    = ipar[6];
        ievt = ipar[5];
        k    = (int)z[1];
        /* copy current record to output */
        record = buffer + (k - 1) * ipar[7] - 1;

        for (i = 0; i < outsz[0]; i++)
        {
            *(outptr[0] + i) = record[mask[ievt + i]];
        }

        if (*nevprt > 0)
        {
            /*     discrete state */
            kmax = (int)z[2];
            if (k >= kmax && kmax == n)
            {
                /*     read a new buffer */
                m = ipar[6] * ipar[7];
                for (i = 0; i < three; ++i)
                {
                    type[i] = (char) ipar[i + 2];
                }
                for (i = 2; i >= 0; i--)
                    if (type[i] != ' ')
                    {
                        type[i + 1] = '\0';
                        break;
                    }
                ierr = 0;
                mget2(fd, ipar[8], buffer, m, type, &ierr);
                if (ierr > 0)
                {
                    scicos_print(_("Read error!\n"));
                    fclose(fd);
                    z[3] = 0.0;
                    *flag = -1;
                    return;
                }
                else if (ierr < 0)
                {
                    /* EOF reached */
                    kmax = -(ierr + 1) / ipar[7];
                }
                else
                {
                    kmax = ipar[6];
                }

                z[1] = 1.0;
                z[2] = kmax;
            }
            else if (k < kmax)
            {
                z[1] = z[1] + 1.0;
            }
        }
    }
    else if (*flag == 3)
    {
        ievt = ipar[5];
        n    = ipar[6];
        k    = (int)z[1];
        kmax = (int) z[2];
        if (k > kmax && kmax < n)
        {
            if (ievt)
            {
                tvec[0] = *t - 1.0;
            }
            else
            {
                tvec[0] = *t * (1.0 + 0.0000000001);
            }
        }
        else
        {
            record = buffer + (k - 1) * ipar[7] - 1;
            if (ievt)
            {
                tvec[0] = record[mask[0]];
            }
        }
    }
    else if (*flag == 4)
    {
        for (i = 0; i < ipar[1]; ++i)
        {
            str[i] = (char) ipar[i + 10];
        }
        str[ipar[1]] = '\0';
        wcfopen(fd, str, "rb");
        if (!fd )
        {
            scicos_print(_("Could not open the file!\n"));
            *flag = -1;
            return;
        }
        z[3] = (long)fd;
        /* skip first records */
        if (ipar[9] > 1)
        {
            for (i = 0; i < three; ++i)
            {
                type[i] = (char) ipar[i + 2];
            }
            for (i = 2; i >= 0; i--)
                if (type[i] != ' ')
                {
                    type[i + 1] = '\0';
                    break;
                }
            offset = (ipar[9] - 1) * ipar[7] * worldsize(type);
            irep = fseek(fd, offset, 0) ;
            if ( irep != 0 )
            {
                scicos_print(_("Read error\n"));
                *flag = -1;
                fclose(fd);
                z[3] = 0.0;
                return;
            }
        }
        /* read first buffer */
        m = ipar[6] * ipar[7];
        for (i = 0; i < three; ++i)
        {
            type[i] = (char) ipar[i + 2];
        }
        for (i = 2; i >= 0; i--)
            if (type[i] != ' ')
            {
                type[i + 1] = '\0';
                break;
            }
        mget2(fd, ipar[8], buffer, m, type, &ierr);
        if (ierr > 0)
        {
            scicos_print(_("Read error!\n"));
            *flag = -1;
            fclose(fd);
            z[3] = 0.0;
            return;
        }
        else if (ierr < 0)
        {
            /* EOF reached */
            kmax = -(ierr + 1) / ipar[7];
        }
        else
        {
            kmax = ipar[6];
        }

        z[1] = 1.0;
        z[2] = kmax;
    }
    else if (*flag == 5)
    {
        if (z[3] == 0)
        {
            return;
        }
        fclose(fd);
        z[3] = 0.0;
    }
    return;
}
/*--------------------------------------------------------------------------*/
int worldsize(char type[4])
{
    char c;
    int wsz = 0;

    c = type[0];
    if (c == 'u')
    {
        c = type[1];
    }
    switch ( c )
    {
        case 'l' :
            wsz = sizeof(long);
            break;
        case 's' :
            wsz = sizeof(short);
            break;
        case 'c' :
            wsz = sizeof(char);
            break;
        case 'd' :
            wsz = sizeof(double);
            break;
        case 'f' :
            wsz = sizeof(float);
            break;
    }
    return wsz;
}
/*--------------------------------------------------------------------------*/
