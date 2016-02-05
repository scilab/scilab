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
#include <math.h>
#include "scicos_print.h"
#include "charEncoding.h"
#include "mput.h"
#include "localization.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void writeau(int *flag, int *nevprt,
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
ipar[5]   = n : buffer length in number of records
ipar[6]   = swap
ipar[7:6+lfil] = character codes for file name
*/
{
    FILE *fd = NULL;
    int n = 0, k = 0, i = 0, ierr = 0;
    double *buffer = NULL, *record = NULL;
    /*  long offset;*/
    int SCALE  = 32768;
    int BIAS   =   132;
    int CLIP   = 32635;
    int OFFSET =   335;
    double y = 0.;
    int sig = 0;
    int e = 0;
    double f = 0.;


    --ipar;
    --z;
    fd = (FILE *)(long)z[2];
    buffer = (z + 3);
    ierr = 0;
    /*
    k    : record counter within the buffer
    */

    if (*flag == 2 && *nevprt > 0)
    {
        /* add a new record to the buffer */
        n    = ipar[5];
        k    = (int)z[1];
        /* copy current record to output
        printf("%i\n",k);*/
        record = buffer + (k - 1) * (*nin);

        for (i = 0; i < *nin; i++)
        {
            y = *inptr[i];
            y = SCALE * y;
            if (y < 0.0)
            {
                y = -y;
                sig = -1;
            }
            else
            {
                sig = 1;
            }
            if (y > CLIP)
            {
                y = CLIP;
            }
            y = y + BIAS;
            f = frexp(y, &e);
            y = 64 * sig - 16 * e - (int) (32 * f) + OFFSET;
            record[i] = y;
        }
        if (k < n)
        {
            z[1] = z[1] + 1.0;
        }
        else
        {
            mput2(fd, ipar[6], buffer, ipar[5] * (*nin), "uc", &ierr);
            if (ierr != 0)
            {
                *flag = -3;
                return;
            }
            z[1] = 1.0;

        }

    }
    else if (*flag == 4)
    {
        wcfopen(fd, "/dev/audio", "wb");
        if (!fd )
        {
            scicos_print(_("Could not open /dev/audio!\n"));
            *flag = -3;
            return;
        }
        z[2] = (double)(long)fd;
        z[1] = 1.0;
    }
    else if (*flag == 5)
    {
        if (z[2] == 0)
        {
            return;
        }
        k    = (int) z[1];
        if (k > 1) /* flush rest of buffer */
        {
            mput2(fd, ipar[6], buffer, (k - 1) * (*nin), "uc", &ierr);
            if (ierr != 0)
            {
                *flag = -3;
                return;
            }
        }
        fclose(fd);
        z[2] = 0.0;
    }
    return;
}
/*--------------------------------------------------------------------------*/
