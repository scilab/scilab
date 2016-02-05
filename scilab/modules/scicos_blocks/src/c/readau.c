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
#include <math.h>
#include <string.h>
#include "scicos_print.h"
#include "machine.h"
#include "charEncoding.h"
#include "mget.h"
#include "localization.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void readau(int *flag, int *nevprt,
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
ipar[5]   = void
ipar[6]   = n : buffer length in number of records
ipar[7]   = maxvoie : record size
ipar[8]   = swap
ipar[9]   = first : first record to read
ipar[10:9+lfil] = character codes for file name
*/
{
    char str[100], type[4];
    int job = 1, three = 3;
    FILE *fd = NULL;
    int n = 0, k = 0, kmax = 0, m = 0, i = 0, irep = 0, ierr = 0;
    double *buffer = NULL, *record = NULL;
    long offset ;
    double y = 0.;
    int quot = 0, rem = 0;
    double SCALE = 0.000030517578125;
    int ETAB[8];
    int mu = 0;
    int sig = 0;
    int e = 0;
    int f = 0;

    ETAB[0] = 0;
    ETAB[1] = 132;
    ETAB[2] = 396;
    ETAB[3] = 924;
    ETAB[4] = 1980;
    ETAB[5] = 4092;
    ETAB[6] = 8316;
    ETAB[7] = 16764;

    --ipar;
    --z;
    fd = (FILE *)(long)z[3];
    buffer = (z + 4);

    /*
    k    : record counter within the buffer
    kmax :  number of records in the buffer
    */

    if (*flag == 1)
    {
        n    = ipar[6];
        k    = (int)z[1];
        /* copy current record to output */
        record = buffer + (k - 1) * ipar[7];

        for (i = 0; i < *nout; i++)
        {
            mu = (int) record[i];

            mu = 255 - mu;
            if (mu > 127)
            {
                sig = 1;
            }
            else
            {
                sig = 0;
            }
            /* comment out for SUNOS SS 8/10/99
            divt=div(mu,16);
            e=divt.quot-8*sig+1;
            f=divt.rem;
            */
            quot = mu / 16;
            rem = mu - 16 * quot;
            e = quot - 8 * sig + 1;
            f = rem;

            y = ldexp((double)(f), (e + 2));
            /* ff=(double)(e+2);
            y=((double) f) * pow(two, ff); */

            e = ETAB[e - 1];

            y = SCALE * (1 - 2 * sig) * (e + y);

            *outptr[i] = y;
        }
        if (*nevprt > 0)
        {
            /*     discrete state */
            kmax = (int) z[2];
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
            offset = (ipar[9] - 1) * ipar[7] * sizeof(char);
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

