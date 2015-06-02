/*  Scicos
*
*  Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include "var2sci.h"
#include "import.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
int var2sci(void *x, int n, int m, int typ_var)
{
    ///************************************
    //* variables and constants d?inition
    //************************************/
    ///*counter and address variable declaration*/
    //int nm = 0, il = 0, l = 0, j = 0, i = 0, err = 0;

    ///*define all type of accepted ptr */
    //SCSREAL_COP *x_d = NULL, *ptr_d = NULL;
    //SCSINT8_COP *x_c = NULL, *ptr_c = NULL;
    //SCSUINT8_COP *x_uc = NULL, *ptr_uc = NULL;
    //SCSINT16_COP *x_s = NULL, *ptr_s = NULL;
    //SCSUINT16_COP *x_us = NULL, *ptr_us = NULL;
    //SCSINT_COP *x_i = NULL, *ptr_i = NULL;
    //SCSUINT_COP *x_ui = NULL, *ptr_ui = NULL;
    //SCSINT32_COP *x_l = NULL, *ptr_l = NULL;
    //SCSUINT32_COP *x_ul = NULL, *ptr_ul = NULL;

    ///* Check if the stack is not full */
    //if (Top >= Bot)
    //{
    //    return 1;
    //}
    //else
    //{
    //    Top = Top + 1;
    //    il = iadr(*Lstk(Top));
    //    l = sadr(il + 4);
    //}

    ///* set number of double needed to store data */
    //if (typ_var == SCSREAL_N)
    //{
    //    nm = n * m;    /*double real matrix*/
    //}
    //else if (typ_var == SCSCOMPLEX_N)
    //{
    //    nm = n * m * 2;    /*double real matrix*/
    //}
    //else if (typ_var == SCSINT_N)
    //{
    //    nm = (int)(ceil((n * m) / 2) + 1);    /*int*/
    //}
    //else if (typ_var == SCSINT8_N)
    //{
    //    nm = (int)(ceil((n * m) / 8) + 1);    /*int8*/
    //}
    //else if (typ_var == SCSINT16_N)
    //{
    //    nm = (int)(ceil((n * m) / 4) + 1);    /*int16*/
    //}
    //else if (typ_var == SCSINT32_N)
    //{
    //    nm = (int)(ceil((n * m) / 2) + 1);    /*int32*/
    //}
    //else if (typ_var == SCSUINT_N)
    //{
    //    nm = (int)(ceil((n * m) / 2) + 1);    /*uint*/
    //}
    //else if (typ_var == SCSUINT8_N)
    //{
    //    nm = (int)(ceil((n * m) / 8) + 1);    /*uint8*/
    //}
    //else if (typ_var == SCSUINT16_N)
    //{
    //    nm = (int)(ceil((n * m) / 4) + 1);    /*uint16*/
    //}
    //else if (typ_var == SCSUINT32_N)
    //{
    //    nm = (int)(ceil((n * m) / 2) + 1);    /*uint32*/
    //}
    //else if (typ_var == SCSUNKNOW_N)
    //{
    //    nm = n * m;    /*arbitrary scilab object*/
    //}
    //else
    //{
    //    nm = n * m;    /*double real matrix*/
    //}

    ///*check if there is free space for new data*/
    //err = l + nm - *Lstk(Bot);
    //if (err > 0)
    //{
    //    return 2;
    //}

    ///**************************
    //* store data on the stack
    //*************************/
    //switch (typ_var) /*for each type of data*/
    //{
    //    case SCSREAL_N    : /* set header */
    //        *istk(il) = sci_matrix; /*double real matrix*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 0;
    //        x_d = (SCSREAL_COP *) x;
    //        ptr_d = (SCSREAL_COP *) stk(l);
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_d[j] = x_d[j];
    //        }
    //        break;

    //    case SCSCOMPLEX_N : /* set header */
    //        *istk(il) = 1; /*double complex matrix*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 1;
    //        x_d = (SCSCOMPLEX_COP *) x;
    //        ptr_d = (SCSCOMPLEX_COP *) stk(l);
    //        for (j = 0; j < 2 * m * n; j++)
    //        {
    //            ptr_d[j] = x_d[j];
    //        }
    //        break;

    //    case SCSINT_N     : /* set header */
    //        *istk(il) = sci_ints; /*int*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 4;
    //        x_i = (SCSINT_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_i = (SCSINT_COP *) istk(il + 4);
    //            ptr_i[j] = x_i[j];
    //        }
    //        break;

    //    case SCSINT8_N    : /* set header */
    //        *istk(il) = sci_ints; /*int8*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 1;
    //        x_c = (SCSINT8_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_c = (SCSINT8_COP *) istk(il + 4);
    //            ptr_c[j] = x_c[j];
    //        }
    //        break;

    //    case SCSINT16_N   : /* set header */
    //        *istk(il) = sci_ints; /*int16*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 2;
    //        x_s = (SCSINT16_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_s = (SCSINT16_COP *) istk(il + 4);
    //            ptr_s[j] = x_s[j];
    //        }
    //        break;

    //    case SCSINT32_N   : /* set header */
    //        *istk(il) = sci_ints; /*int32*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 4;
    //        x_l = (SCSINT32_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_l = (SCSINT32_COP *) istk(il + 4);
    //            ptr_l[j] = x_l[j];
    //        }
    //        break;

    //    case SCSUINT_N   : /* set header */
    //        *istk(il) = sci_ints; /*uint*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 14;
    //        x_ui = (SCSUINT_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_ui = (SCSUINT_COP *) istk(il + 4);
    //            ptr_ui[j] = x_ui[j];
    //        }
    //        break;

    //    case SCSUINT8_N   : /* set header */
    //        *istk(il) = sci_ints; /*uint8*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 11;
    //        x_uc = (SCSUINT8_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_uc = (SCSUINT8_COP *) istk(il + 4);
    //            ptr_uc[j] = x_uc[j];
    //        }
    //        break;

    //    case SCSUINT16_N  : /* set header */
    //        *istk(il) = sci_ints; /*uint16*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 12;
    //        x_us = (SCSUINT16_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_us = (SCSUINT16_COP *) istk(il + 4);
    //            ptr_us[j] = x_us[j];
    //        }
    //        break;

    //    case SCSUINT32_N  : /* set header */
    //        *istk(il) = sci_ints; /*uint32*/
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 14;
    //        x_ul = (SCSUINT32_COP *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_ul = (SCSUINT32_COP *) istk(il + 4);
    //            ptr_ul[j] = x_ul[j];
    //        }
    //        break;

    //    case SCSUNKNOW_N  :
    //        x_d = (double *) x;
    //        C2F(unsfdcopy)(&nm, x_d, (j = 1, &j), stk(*Lstk(Top)), (i = 1, &i));
    //        break;

    //    default         : /* set header */
    //        *istk(il) = sci_matrix; /* double by default */
    //        *istk(il + 1) = n;
    //        *istk(il + 2) = m;
    //        *istk(il + 3) = 0;
    //        x_d = (double *) x;
    //        for (j = 0; j < m * n; j++)
    //        {
    //            ptr_d = (double *) stk(il + 4);
    //            ptr_d[j] = x_d[j];
    //        }
    //        break;
    //}

    ///* set value in lstk */
    //*Lstk(Top + 1) = l + nm;

    return 0;
}
/*--------------------------------------------------------------------------*/
