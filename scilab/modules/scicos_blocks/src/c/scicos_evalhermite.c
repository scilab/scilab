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
#include "scicos_evalhermite.h"
/*--------------------------------------------------------------------------*/
int scicos_evalhermite(double *t, double *xa, double *xb, double *ya, double *yb, double *da, double *db, double *h, double *dh, double *ddh, double *dddh, int *i)
{
    double tmxa = 0., p = 0., c2 = 0., c3 = 0., dx = 0.;

    tmxa = *t - *xa;
    dx = 1. / (*xb - *xa);
    p = (*yb - *ya) * dx;
    c2 = (p - *da) * dx;
    c3 = (*db - p + (*da - p)) * (dx * dx);

    *h = c2 + c3 * (*t - *xb);
    *dh = *h + c3 * tmxa;
    *ddh = (*dh + c3 * tmxa) * 2.;
    *dddh = c3 * 6.;
    *h = *da + *h * tmxa;
    *dh = *h + *dh * tmxa;
    *h = *ya + *h * tmxa;

    return 0;
}
/*--------------------------------------------------------------------------*/
