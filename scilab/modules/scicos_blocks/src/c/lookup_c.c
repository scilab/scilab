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
#include "sci_malloc.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_evalhermite.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
/*    Masoud Najafi, January 2008 */
/*    Copyright INRIA
 *    Scicos block simulator
 *    Lookup table block
 */
/*--------------------------------------------------------------------------*/
#define RPAR     block->rpar
#define nPoints  block->ipar[0]
#define Order    block->ipar[1]
#define Periodic block->ipar[2]
#define Extrapo  block->ipar[3]
#define T        RPAR[nPoints-1]-RPAR[0]
/*--------------------------------------------------------------------------*/
static int FindIndex(int, double , int , int , double *, int);
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void lookup_c(scicos_block *block, int flag)
{
    double a = 0., b = 0., c = 0., y1 = 0., y2 = 0., t1 = 0., t2 = 0.;
    int** work = (int**) block->work;
    int *ind  = NULL, inow = 0, i = 0, ip1 = 0;
    double *y = NULL, *u = NULL, u0 = 0.;
    double  d1 = 0., d2 = 0., h = 0., dh = 0., ddh = 0., dddh = 0.;

    switch (flag)
    {
        /* init */
        case 4  :  /* the workspace is used to store discrete counter value */
        {
            if ((*work = (int*) scicos_malloc(1 * sizeof(int))) == NULL)
            {
                set_block_error(-16);
                return;
            }
            ind = *work;
            ind[0] = 0;

            return;
        }
        /* event date computation */
        case 1  :
        {
            y = GetRealOutPortPtrs(block, 1);
            u = GetRealInPortPtrs(block, 1);
            u0 = u[0];
            ind = *work;
            i = ind[0];
            ip1 = i + 1;

            if ((Extrapo == 0) || ((Extrapo == 1) && ((Order == 0) || (Order == 8) || (Order == 9))))
            {
                if (u0 < RPAR[0])
                {
                    y[0] = RPAR[nPoints];
                    break;
                }
                if (u0 >= RPAR[nPoints - 1])
                {
                    y[0] = RPAR[nPoints * 2 - 1];
                    break;
                }
            }

            if (u0 < RPAR[i])
            {
                i = FindIndex(Order, u0, 0, i, RPAR, nPoints);
            }
            else if (u0 >= RPAR[ip1])
            {
                i = FindIndex(Order, u0, ip1, nPoints - 1, RPAR, nPoints);
            }
            ind[0] = i;

            if (Order == 0) /* (METHOD=='zero order-below')*/
            {
                y[0] = RPAR[nPoints + i];
                break;
            }

            if (Order == 8) /* (METHOD=='zero order-above')*/
            {
                y[0] = RPAR[nPoints + i + 1];
                break;
            }

            if (Order == 9) /* (METHOD=='zero order-nearest')*/
            {
                if (u0 < (RPAR[i] + RPAR[i + 1]) / 2 )
                {
                    y[0] = RPAR[nPoints + i];
                }
                else
                {
                    y[0] = RPAR[nPoints + i + 1];
                }
                break;
            }

            if (Order == 1)
            {
                t1 = RPAR[i];
                t2 = RPAR[i + 1];
                y1 = RPAR[nPoints + i];
                y2 = RPAR[nPoints + i + 1];
                y[0] = (y2 - y1) * (u0 - t1) / (t2 - t1) + y1;
                break;
            }

            if ((Order == 2) && (nPoints > 2))
            {
                t1 = RPAR[i];
                a = RPAR[2 * nPoints + i];
                b = RPAR[2 * nPoints + i + nPoints - 1];
                c = RPAR[2 * nPoints + i + 2 * nPoints - 2];
                y[0] = a * (u0 - t1) * (u0 - t1) + b * (u0 - t1) + c;
                break;
            }

            if ((Order >= 3) && (Order <= 7))
            {
                t1 = RPAR[i];
                t2 = RPAR[i + 1];
                y1 = RPAR[nPoints + i];
                y2 = RPAR[nPoints + i + 1];
                d1 = RPAR[2 * nPoints + i];
                d2 = RPAR[2 * nPoints + i + 1];
                /*-- this function is defined in curve_c.c ---*/
                scicos_evalhermite(&u0, &t1, &t2, &y1, &y2, &d1, &d2, &h, &dh, &ddh, &dddh, &inow);
                y[0] = h;
                break;
            }
        }
        /* event date computation */
        case 3  :
        {
            /*	ind=*block->work;
            i=ind[0];

            if ((Order==1)||(Order==0)){
              i=ind[2];
              if (i==nPoints-1){
                if (Periodic==1) {
                  i=0;
                  ind[0]=-1;
                  ind[1]=0;
                }
              }
              if (i<nPoints-1) {
                block->evout[0]=RPAR[i+1]-RPAR[i];

                ind[2]=i+1;
              }
              if (ind[2]==1)  ind[3]++;
            }

            if (Order>=2){
              if ( Periodic) {
                block->evout[0]=T;
              }else{
                if (ind[3]==0) {
                  block->evout[0]=T;
                }
              }
              ind[3]++;
              ind[0]=-1;
              ind[1]=0;

            }*/
            break;
        }

        /* finish */
        case 5  :
        {
            scicos_free(*work); /*free the workspace*/
            break;
        }

        default :
            break;
    }
}
/*--------------------------------------------------------------------------*/
static int FindIndex(int order, double inp, int idown, int iup, double *data, int N)
{

    if (inp <= data[0] )
    {
        return 0;
    }
    if (inp >= data[N - 1] )
    {
        return N - 2;
    }

    while (idown + 1 != iup )
    {
        int im = (int)((idown + iup) / 2);
        if (inp >= data[im])
        {
            idown = im;
        }
        else if (inp < data[im])
        {
            iup = im;
        }
    }

    return idown;
}
/*--------------------------------------------------------------------------*/
