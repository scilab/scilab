/*  Scicos
*
*  Copyright (C) INRIA - Allan LAYEC
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
#include <string.h>
#include <stdlib.h>
#include "MlistGetFieldNumber.h"
#include "cvstr.h"
#include "sci_malloc.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
extern int *listentry(int *header, int i);
/*--------------------------------------------------------------------------*/
int MlistGetFieldNumber(int *ptr, const char *string)
{
    int nf = 0, longueur = 0, istart = 0, k = 0, ilocal = 0, retval = 0;
    int *headerstr = NULL;
    /* allocate a place for the trailing '\0' */
    //    static char str[nlgh + 1];

    //headerstr = listentry(ptr , 1);
    //nf = headerstr[1] * headerstr[2] - 1;  /* number of fields */
    //retval = -1;
    //for (k = 0; k < nf; k++)
    //{
    //    longueur = Min( headerstr[6 + k] - headerstr[5 + k] , nlgh); /* size of kth fieldname */
    //    istart = 5 + nf + headerstr[5 + k];  /* start of kth fieldname code */

    //    C2F(cvstr)(&longueur, &headerstr[istart], str, (ilocal = 1, &ilocal), longueur);
    //    str[longueur] = '\0';
    //    if (strcmp(string, str) == 0)
    //    {
    //        retval = k + 2;
    //        break;
    //    }
    //}
    return retval;
}
/*--------------------------------------------------------------------------*/
