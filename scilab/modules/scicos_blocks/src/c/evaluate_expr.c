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
#include <stdlib.h>
#if _MSC_VER
#include <float.h>
#endif

#ifdef solaris
#include <ieeefp.h>
#endif

#include "core_math.h"
#include "scicos_block.h"
#include "machine.h" /* isinf */
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
#if _MSC_VER
/*
arcsinh z = log (z+sqrt(1+z2))
*/
double asinh(double x)
{
    return log(x + sqrt(x * x + 1));
}
/*--------------------------------------------------------------------------*/
double acosh(double x)
{
    return log(x + sqrt(x * x - 1));
}
/*--------------------------------------------------------------------------*/
/*
Inverse hyperbolic tangent (Atanh(x)) Log((1 + x) / (1 – x)) / 2
*/
double atanh(double x)
{
    return (double)(log ((1. + x) / (1. - x)) / 2);
}
#endif
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void evaluate_expr(scicos_block *block, int flag)
{
    static double stack [1000];
    static int count = 0, bottom = 0, nzcr = 0, i = 0, phase = 0;
    int j = 0;
    if (flag == 1 || flag == 6 || flag == 9)
    {
        phase = get_phase_simulation();
        bottom = -1;
        count = -1;
        nzcr = -1;
        while (count < block->nipar - 1)
        {
            count = count + 1;
            switch (block->ipar[count])
            {
                case 2:
                    count = count + 1;
                    bottom = bottom + 1;
                    if (bottom > 999)
                    {
                        set_block_error(-16);
                        return;
                    }
                    if (block->nin > 1)
                    {
                        stack[bottom] = block->inptr[block->ipar[count] - 1][0];
                    }
                    else
                    {
                        j = block->ipar[count] - 1;
                        if (j < block->insz[0])
                        {
                            stack[bottom] = block->inptr[0][block->ipar[count] - 1];
                        }
                        else
                        {
                            stack[bottom] = 0.;
                        }
                    }
                    break;
                case 6:
                    count = count + 1;
                    bottom = bottom + 1;
                    if (bottom > 999)
                    {
                        set_block_error(-16);
                        return;
                    }
                    stack[bottom] = block->rpar[block->ipar[count] - 1];
                    break;
                case 5:
                    count = count + 1;
                    /* invalid script : call a function without lhs */
                    if (bottom < 0)
                    {
                        set_block_error(-2);
                        return;
                    }

                    switch (block->ipar[count])
                    {
                        case 1:
                            stack[bottom - 1] = stack[bottom - 1] + stack[bottom];
                            bottom = bottom - 1;
                            break;
                        case 2:
                            stack[bottom - 1] = stack[bottom - 1] - stack[bottom];
                            bottom = bottom - 1;
                            break;
                        case 3:
                            stack[bottom - 1] = stack[bottom - 1] * stack[bottom];
                            bottom = bottom - 1;
                            break;
                        case 7:
                            stack[bottom - 1] = stack[bottom - 1] / stack[bottom];
                            bottom = bottom - 1;
                            break;
                        case 15:
                            stack[bottom - 1] = pow(stack[bottom - 1], stack[bottom]);
                            bottom = bottom - 1;
                            break;
                        case 16: /* case == */
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (stack[bottom - 1] == stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom - 1] == stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;

                        case 17:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (stack[bottom - 1] < stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom - 1] < stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;
                        case 18:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (stack[bottom - 1] > stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom - 1] > stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;
                        case 19:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (stack[bottom - 1] <= stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom - 1] <= stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;
                        case 20:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (stack[bottom - 1] >= stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom - 1] >= stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;
                        case 21:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (stack[bottom - 1] != stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom - 1] != stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;
                        case 28:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = ((int)stack[bottom - 1] || (int)stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = ((int)stack[bottom - 1] || (int)stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;
                        case 29:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom - 1] - stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = ((int)stack[bottom - 1] && (int)stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = ((int)stack[bottom - 1] && (int)stack[bottom]);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            stack[bottom - 1] = (double)i;
                            bottom = bottom - 1;
                            break;

                        case 30:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                block->g[nzcr] = stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = (0.0 == stack[bottom]);
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                i = (stack[bottom] == 0.0);
                            }
                            else
                            {
                                i = block->mode[nzcr];
                            }
                            if (i)
                            {
                                stack[bottom] = 1.0;
                            }
                            else
                            {
                                stack[bottom] = 0.0;
                            }
                            break;
                        case 99:
                            stack[bottom] = -stack[bottom];
                            break;
                        case 101:
                            stack[bottom] = sin(stack[bottom]);
                            break;
                        case 102:
                            stack[bottom] = cos(stack[bottom]);
                            break;
                        case 103:
                            stack[bottom] = tan(stack[bottom]);
                            break;
                        case 104:
                            stack[bottom] = exp(stack[bottom]);
                            break;
                        case 105:
                            stack[bottom] = log(stack[bottom]);
                            break;
                        case 106:
                            stack[bottom] = sinh(stack[bottom]);
                            break;
                        case 107:
                            stack[bottom] = cosh(stack[bottom]);
                            break;
                        case 108:
                            stack[bottom] = tanh(stack[bottom]);
                            break;

                        case 109:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                if (stack[bottom] > 0)
                                {
                                    i = (int)floor(stack[bottom]);
                                }
                                else
                                {
                                    i = (int)ceil(stack[bottom]);
                                }
                                if (i == 0)
                                {
                                    block->g[nzcr] = (stack[bottom] - 1) * (stack[bottom] + 1);
                                }
                                else if (i > 0)
                                {
                                    block->g[nzcr] = (stack[bottom] - i - 1.) * (stack[bottom] - i);
                                }
                                else
                                {
                                    block->g[nzcr] = (stack[bottom] - i) * (stack[bottom] - i + 1);
                                }
                                if (i % 2)
                                {
                                    block->g[nzcr] = -block->g[nzcr];
                                }
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                if (stack[bottom] > 0)
                                {
                                    stack[bottom] = floor(stack[bottom]);
                                }
                                else
                                {
                                    stack[bottom] = ceil(stack[bottom]);
                                }
                            }
                            else
                            {
                                stack[bottom] = (double) block->mode[nzcr];
                            }
                            break;
                            /*
                            if (stack[bottom]>0) {
                              stack[bottom]=floor(stack[bottom]);
                            }else{
                              stack[bottom]=ceil(stack[bottom]);
                              }*/
                            break;
                        case 110:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                if (stack[bottom] > 0)
                                {
                                    i = (int)floor(stack[bottom] + .5);
                                }
                                else
                                {
                                    i = (int)ceil(stack[bottom] - .5);
                                }
                                block->g[nzcr] = (stack[bottom] - i - .5) * (stack[bottom] - i + .5);
                                if (i % 2)
                                {
                                    block->g[nzcr] = -block->g[nzcr];
                                }
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                if (stack[bottom] > 0)
                                {
                                    stack[bottom] = floor(stack[bottom] + .5);
                                }
                                else
                                {
                                    stack[bottom] = ceil(stack[bottom] - .5);
                                }
                            }
                            else
                            {
                                stack[bottom] = (double) block->mode[nzcr];
                            }
                            break;
                            /*  if (stack[bottom]>0) {
                              stack[bottom]=floor(stack[bottom]+.5);
                            }else{
                              stack[bottom]=ceil(stack[bottom]-.5);
                            }*/
                        case 111:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                i = (int)ceil(stack[bottom]);
                                block->g[nzcr] = (stack[bottom] - i) * (stack[bottom] - i + 1);
                                if (i % 2)
                                {
                                    block->g[nzcr] = -block->g[nzcr];
                                }
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                stack[bottom] = ceil(stack[bottom]);
                            }
                            else
                            {
                                stack[bottom] = (double) block->mode[nzcr];
                            }
                            break;
                        case 112:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                i = (int)floor(stack[bottom]);
                                block->g[nzcr] = (stack[bottom] - i - 1) * (stack[bottom] - i);
                                if (i % 2)
                                {
                                    block->g[nzcr] = -block->g[nzcr];
                                }
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                stack[bottom] = floor(stack[bottom]);
                            }
                            else
                            {
                                stack[bottom] = (double) block->mode[nzcr];
                            }
                            break;
                        case 113:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                if (stack[bottom] > 0)
                                {
                                    i = 1;
                                }
                                else if (stack[bottom] < 0)
                                {
                                    i = -1;
                                }
                                else
                                {
                                    i = 0;
                                }
                                block->g[nzcr] = stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                if (stack[bottom] > 0)
                                {
                                    stack[bottom] = 1.0;
                                }
                                else if (stack[bottom] < 0)
                                {
                                    stack[bottom] = -1.0;
                                }
                                else
                                {
                                    stack[bottom] = 0.0;
                                }
                            }
                            else
                            {
                                stack[bottom] = (double) block->mode[nzcr];
                            }
                            break;
                            /* if (stack[bottom]>0) {
                              stack[bottom]=1.0;
                            }else if(stack[bottom]<0){
                              stack[bottom]=-1.0;
                            }else{
                              stack[bottom]=0.0;
                              }*/
                        case 114:  /* abs */
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                if (stack[bottom] > 0)
                                {
                                    i = 1;
                                }
                                else if (stack[bottom] < 0)
                                {
                                    i = -1;
                                }
                                else
                                {
                                    i = 0;
                                }
                                block->g[nzcr] = stack[bottom];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                if (stack[bottom] > 0)
                                {
                                    /* stack[bottom] = stack[bottom]; */
                                }
                                else
                                {
                                    stack[bottom] = -stack[bottom];
                                }
                            }
                            else
                            {
                                stack[bottom] = stack[bottom] * (block->mode[nzcr]);
                            }
                            break;
                            /* if (stack[bottom]>0) {
                              stack[bottom]=stack[bottom];
                            }else {
                              stack[bottom]=-stack[bottom];
                              }*/
                        case 115:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                if (stack[bottom] > stack[bottom - 1])
                                {
                                    i = 0;
                                }
                                else
                                {
                                    i = 1;
                                }
                                block->g[nzcr] = stack[bottom] - stack[bottom - 1];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                stack[bottom - 1] = Max(stack[bottom - 1], stack[bottom]);
                            }
                            else
                            {
                                stack[bottom - 1] = stack[bottom - block->mode[nzcr]];
                            }
                            bottom = bottom - 1;
                            break;
                        case 116:
                            if (block->ng > 0)
                            {
                                nzcr = nzcr + 1;
                            }
                            if (flag == 9)
                            {
                                if (stack[bottom] < stack[bottom - 1])
                                {
                                    i = 0;
                                }
                                else
                                {
                                    i = 1;
                                }
                                block->g[nzcr] = stack[bottom] - stack[bottom - 1];
                                if (phase == 1)
                                {
                                    block->mode[nzcr] = i;
                                }
                            }
                            if (phase == 1 || block->ng == 0)
                            {
                                stack[bottom - 1] = Min(stack[bottom - 1], stack[bottom]);
                            }
                            else
                            {
                                stack[bottom - 1] = stack[bottom - block->mode[nzcr]];
                            }
                            bottom = bottom - 1;
                            break;
                        case 117:
                            stack[bottom] = asin(stack[bottom]);
                            break;
                        case 118:
                            stack[bottom] = acos(stack[bottom]);
                            break;
                        case 119:
                            stack[bottom] = atan(stack[bottom]);
                            break;
                        case 120:
                            stack[bottom] = asinh(stack[bottom]);
                            break;
                        case 121:
                            stack[bottom] = acosh(stack[bottom]);
                            break;
                        case 122:
                            stack[bottom] = atanh(stack[bottom]);
                            break;
                        case 123:
                            stack[bottom - 1] = atan2(stack[bottom - 1], stack[bottom]);
                            bottom = bottom - 1;
                            break;

                        case 124:
                            stack[bottom] = log10(stack[bottom]);
                            break;
                    }
                    break;

                default:
                    set_block_error(-2);
                    return;
            }
        }
        if (bottom < 0)
        {
            set_block_error(-2);
            return;
        }
#if _MSC_VER
        if (!_finite(stack[bottom]) || _isnan(stack[bottom]))
        {
#else
        if (isinf(stack[bottom]) || isnan(stack[bottom]))
        {
#endif
            if (flag == 6)
            {
                return;
            }
            set_block_error(-2);
            return;
        }
        else
        {
            block->outptr[0][0] = stack[bottom];
        }
    }
}
/*--------------------------------------------------------------------------*/
