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
#include <memory.h>
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "scicos.h"
#include "core_math.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void assignment(scicos_block *block, int flag)
{
    int my = 0, ny = 0, so = 0;
    double *out0 = NULL;
    my = GetOutPortRows(block, 1);
    ny = GetOutPortCols(block, 1);
    out0 = GetRealOparPtrs(block, 6);
    so = GetSizeOfOut(block, 1);
    if (flag == 4)
    {
        /*array that contains
          1 - the size of the indx1
          2 - indx1
          3 - the size of index2
          4 - index2
        */
        if ((*(block->work) = scicos_malloc((my + ny + 2) * sizeof(int))) == NULL)
        {
            set_block_error(-16);
            return;
        }

    }

    else if (flag == 1)
    {
        double *indxopt, *indxb, *indx1, *indx2;
        int typ, i, j, nev;
        int *tempind1 = NULL, *tempind2 = NULL;
        void *y = NULL, *u = NULL;
        tempind1 = (int *) * (block->work);
        tempind2 = tempind1 + (my + 1);
        indx2 = GetRealOparPtrs(block, 4);
        indx1 = GetRealOparPtrs(block, 3);
        indxopt = GetRealOparPtrs(block, 2);
        indxb = GetRealOparPtrs(block, 1);
        typ = GetOutType(block, 1);
        nev = GetNevIn(block);
        if (nev % 2 == 1)
        {
            if (*out0 == 1) /* initilize to the first input*/
            {
                void *y0, *y;
                y = GetOutPortPtrs(block, 1);
                y0 = GetInPortPtrs(block, 1);
                memcpy(y, y0, my * ny * so);
            }
        }
        if ((*indxopt == 3) | (*indxopt == 5))
        {
            if ((typ == SCSREAL_N) | (typ == SCSCOMPLEX_N))
            {
                SCSREAL_COP *ind1;
                ind1 = GetRealInPortPtrs(block, 2 + (int) * out0);
                *tempind1 = 1;
                *(tempind1 + 1) = Max(Min((int)(*ind1), my - (1 - (int)(*indxb))) - (int)(*indxb), 0);
            }
            else if ((typ == SCSINT8_N) | (typ == SCSUINT8_N))
            {
                SCSINT8_COP  *ind1;
                ind1 = Getint8InPortPtrs(block, 2 + (int) * out0);
                *tempind1 = 1;
                *(tempind1 + 1) = Max(Min((int)(*ind1), my - (1 - (int)(*indxb))) - (int)(*indxb), 0);
            }
            else if ((typ == SCSINT16_N) | (typ == SCSUINT16_N))
            {
                SCSINT16_COP *ind1;
                ind1 = Getint16InPortPtrs(block, 2 + (int) * out0);
                *tempind1 = 1;
                *(tempind1 + 1) = Max(Min((int)(*ind1), my - (1 - (int)(*indxb))) - (int)(*indxb), 0);
            }
            else if ((typ == SCSINT32_N) | (typ == SCSUINT32_N))
            {
                SCSINT32_COP *ind1;
                ind1 = Getint32InPortPtrs(block, 2 + (int) * out0);
                *tempind1 = 1;
                *(tempind1 + 1) = Max(Min((int)(*ind1), my - (1 - (int)(*indxb))) - (int)(*indxb), 0);
            }
        }
        else if ((*indxopt == 2) | (*indxopt == 4))
        {
            *tempind1 = GetOparSize(block, 3, 1) * GetOparSize(block, 3, 2);
            for (i = 0; i < *tempind1; i++)
            {
                *(tempind1 + 1 + i) = Max(Min((int)(*(indx1 + i)), my - (1 - (int)(*indxb))) - (int)(*indxb), 0);
            }
        }
        else
        {
            *tempind1 = my;
            for (i = 0; i < my; i++)
            {
                *(tempind1 + 1 + i) = i;
            }
        }
        if (ny == 1)
        {
            *tempind2 = 1;
            *(tempind2 + 1) = 0;
        }
        else   /*matrix case */
        {
            if (((*(indxopt + 1) == 3) | (*(indxopt + 1) == 5)) & ((*indxopt == 3) | (*indxopt == 5)))
            {
                if ((typ == SCSREAL_N) | (typ == SCSCOMPLEX_N))
                {
                    SCSREAL_COP *ind2;
                    ind2 = GetRealInPortPtrs(block, 3 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
                else if ((typ == SCSINT8_N) | (typ == SCSUINT8_N))
                {
                    SCSINT8_COP *ind2;
                    ind2 = Getint8InPortPtrs(block, 3 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
                else if ((typ == SCSINT16_N) | (typ == SCSUINT16_N))
                {
                    SCSINT16_COP *ind2;
                    ind2 = Getint16InPortPtrs(block, 3 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
                else if ((typ == SCSINT32_N) | (typ == SCSUINT32_N))
                {
                    SCSINT32_COP *ind2;
                    ind2 = Getint32InPortPtrs(block, 3 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
            }
            else if (((*(indxopt + 1) == 3) | (*(indxopt + 1) == 5)) & (!((*indxopt == 3) | (*indxopt == 5))))
            {
                if ((typ == SCSREAL_N) | (typ == SCSCOMPLEX_N))
                {
                    SCSREAL_COP *ind2;
                    ind2 = GetRealInPortPtrs(block, 2 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
                else if ((typ == SCSINT8_N) | (typ == SCSUINT8_N))
                {
                    SCSINT8_COP *ind2;
                    ind2 = Getint8InPortPtrs(block, 2 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
                else if ((typ == SCSINT16_N) | (typ == SCSUINT16_N))
                {
                    SCSINT16_COP *ind2;
                    ind2 = Getint16InPortPtrs(block, 2 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
                else if ((typ == SCSINT32_N) | (typ == SCSUINT32_N))
                {
                    SCSINT32_COP *ind2;
                    ind2 = Getint32InPortPtrs(block, 2 + (int) * out0);
                    *tempind2 = 1;
                    *(tempind2 + 1) = Max(Min((int)(*ind2), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
            }
            else if ((*(indxopt + 1) == 2) | (*(indxopt + 1) == 4))
            {
                *tempind2 = GetOparSize(block, 4, 1) * GetOparSize(block, 4, 2);
                for (i = 0; i < *tempind2; i++)
                {
                    *(tempind2 + 1 + i) = Max(Min((int)(*(indx2 + i)), ny - (1 - (int)(*indxb))) - (int)(*indxb), 0);
                }
            }
            else
            {
                *tempind1 = ny;
                for (i = 0; i < ny; i++)
                {
                    *(tempind2 + 1 + i) = i;
                }
            }
        }

        y = GetOutPortPtrs(block, 1);
        u = GetInPortPtrs(block, 1 + (int) * out0);
        for (j = 0; j < tempind2[0]; j++)
        {
            for (i = 0; i < tempind1[0]; i++)
            {
                memcpy(((int*)y + ((*(tempind2 + 1 + j) * (my)) + (*(tempind1 + 1 + i)))*so), u, so);
            }
        }
    }
    else if (flag == 5)
    {
        if (*(block->work) != 0)
        {
            scicos_free(*(block->work));
        }
    }
}
/*--------------------------------------------------------------------------*/
