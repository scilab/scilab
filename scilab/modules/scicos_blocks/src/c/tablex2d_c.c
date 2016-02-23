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
#include <math.h>
#include "scicos_block4.h"
#include "scicos_indexfinder.h"
#include "sci_malloc.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
#define InterpExtrap    0
#define InterpEndValue  1
#define InputNearest    2
#define InputBelow      3
#define InputAbove      4
/*--------------------------------------------------------------------------*/
double computeZ(double *X, double *Y, double *Z, int nx, int ny, int EXTRM, double x, double y);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void tablex2d_c(scicos_block *block, int flag)
{
    double *y = NULL, *u1 = NULL, *u2 = NULL;
    double *X = NULL, *Y = NULL, *Z = NULL;

    int Nx = block->ipar[0];
    int Ny = block->ipar[1];
    int EXTRM = block->ipar[2];

    X = block->rpar;
    Y = X + Nx;
    Z = Y + Ny;

    switch (flag)
    {
        /* init */
        case 4  :

        case 1  :
            u1 = GetRealInPortPtrs(block, 1);
            u2 = GetRealInPortPtrs(block, 2);
            y = GetRealOutPortPtrs(block, 1);
            y[0] = computeZ(X, Y, Z, Nx, Ny, EXTRM, u1[0], u2[0]);
            break;
        case 3  :
        case 5  :
        default :
            break;
    }
}
/*--------------------------------------------------------------------------*/
double computeZ(double *X, double *Y, double *Z, int nx, int ny, int EXTRM, double x, double y)
{
    int i = 0, j = 0, im = 0, jm = 0;
    double z = 0., fq11 = 0., fq12 = 0., fq21 = 0., fq22 = 0., z1 = 0., z2 = 0., T = 0.;

    i = scicos_indexfinder(x, nx, X);
    j = scicos_indexfinder(y, ny, Y);

    if (EXTRM == InputNearest)
    {
        if (x >= X[nx - 1])
        {
            x = X[nx - 1];
        }
        else if (x <= X[0])
        {
            x = X[0];
        }
        else if ((X[i] - x) > (x - X[i - 1]))
        {
            x = X[i - 1];
        }
        else
        {
            x = X[i];
        }


        if (y >= Y[ny - 1])
        {
            y = Y[ny - 1];
        }
        else if (y <= Y[0])
        {
            y = Y[0];
        }
        else if ((Y[j] - y) > (y - Y[j - 1]))
        {
            y = Y[j - 1];
        }
        else
        {
            y = Y[j];
        }


    }
    else if (EXTRM == InputBelow)
    {
        if (x >= X[nx - 1])
        {
            x = X[nx - 1];
        }
        else if (x <= X[0])
        {
            x = X[0];
        }
        else
        {
            if (x < X[i])
            {
                x = X[i - 1];
            }
        }

        if (y >= Y[ny - 1])
        {
            y = Y[ny - 1];
        }
        else if (y <= Y[0])
        {
            y = Y[0];
        }
        else
        {
            if (y < Y[j])
            {
                y = Y[j - 1];
            }
        }

    }
    else if (EXTRM == InputAbove)
    {
        if (x >= X[nx - 1])
        {
            x = X[nx - 1];
        }
        else if (x <= X[0])
        {
            x = X[0];
        }
        else
        {
            if (x > X[i - 1])
            {
                x = X[i];
            }
        }

        if (y >= Y[ny - 1])
        {
            y = Y[ny - 1];
        }
        else if (y <= Y[0])
        {
            y = Y[0];
        }
        else
        {
            if (y > Y[j - 1])
            {
                y = Y[j];
            }
        }

    }
    else if (EXTRM == InterpEndValue)
    {
        if (x >= X[nx - 1])
        {
            x = X[nx - 1];
        }
        else if (x <= X[0])
        {
            x = X[0];
        }


        if (y >= Y[ny - 1])
        {
            y = Y[ny - 1];
        }
        else if (y <= Y[0])
        {
            y = Y[0];
        };

    }
    else
    {
        /* InterpExtrap */
    }

    im = i - 1;
    jm = j - 1;
    fq11 = Z[im + jm * nx];
    fq21 = Z[i + jm * nx];
    fq12 = Z[im + j * nx];
    fq22 = Z[i + j * nx];

    z = (X[i] - X[im]) * (Y[j] - Y[jm]);
    z1 = (fq11 * (X[i] - x) + fq21 * (x - X[im])) * (Y[j] - y);
    z2 = (fq12 * (X[i] - x) + fq22 * (x - X[im])) * (y - Y[jm]);
    T = (z1 + z2) / z;
    return T;
}
/*--------------------------------------------------------------------------*/
