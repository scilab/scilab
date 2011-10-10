/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_special_functions.h"
#include "stack-c.h"
#include "Scierror.h"
#include "msgs.h"
#include "zbeshv.h"
/*--------------------------------------------------------------------------*/
int sci_besselh(char *fname,unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, it2 = 0, l2 = 0, l2i = 0, mr = 0;
    int nr = 0, itr = 0, lr = 0, li = 0, lwr = 0, rhs1 = 0, lwi = 0;
    int r1 = 0, r2 = 0, na = 0, nx = 0, kode = 0, lpos = 0;
    int un = 1, ierr = 0;
    int K = 0;

    int *Lstk    = C2F(vstk).lstk-1;

    CheckRhs(2, 4);
    kode = 1;
    rhs1 = Rhs;
    if (Rhs == 4) 
    { 
        /* normalized bessel required */
        GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);CheckScalar(1,m1,n1);
        kode = (int)*stk(l1)+1;
        rhs1--;
    }
    K = 1; 
    if (Rhs > 2) 
    { 
        /* normalized bessel required */
        GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);CheckScalar(1,m1,n1);
        K = (int)*stk(l1);
    }

    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); /* get alpha */
    r1 = l1 > Lstk[Bot]; /* true if the variable has been passed by reference */

    GetRhsCVar(rhs1,MATRIX_OF_DOUBLE_DATATYPE,&it2,&m2,&n2,&l2,&l2i); /* get x */
    r2 = l2 > Lstk[Bot];/* true if the variable has been passed by reference */

    if (m1*n1 == 0) 
    { 
        /*besselh([],x) */
        LhsVar(1) = 1;
        PutLhsVar();
        return 0;
    }
    if (m2*n2 == 0) 
    {
        /*besselh(alpha,[]) */
        LhsVar(1) = rhs1;
        PutLhsVar();
        return 0;
    }

    itr = 1;
    lpos = Rhs;
    if(it2 == 0) 
    {
        int i = 0;
        CreateVar(lpos+1,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2i);
        for (i=0;i<m2*n2;i++) *stk(l2i+i) = 0.0;
        lpos = lpos + 1;
    }
    if (m1*n1 == 1) 
    { 
        /*bessely(scalar,matrix) */
        double wr[3],wi[3];
        double *xr = stk(l2);
        double *xi = stk(l2i);
        double *alpha = stk(l1);
        double *yr = NULL;
        double *yi = NULL;

        mr = m2;
        nr = n2;    
        CreateCVar(lpos+1,MATRIX_OF_DOUBLE_DATATYPE,&itr,&mr,&nr,&lr,&li);
        yr = stk(lr);
        yi = stk(li);

        nx = m2*n2;
        na = 1;

        zbeshv(xr, xi, &nx, alpha, &na, &kode, &K, yr, yi, wr, wi, &ierr);
        LhsVar(1) = lpos + 1;
    }
    else if (m2*n2 == 1) 
    { 
        /* bessely(matrix,scalar) */
        int llwr = 0, llwi = 0, nw = 0;
        mr = m1;
        nr = n1;   
        CreateCVar(lpos+1,MATRIX_OF_DOUBLE_DATATYPE,&itr,&mr,&nr,&lr,&li);
        nx = 1;
        na = m1*n1;
        nw = 3*na;
        CreateCVar(lpos+2,MATRIX_OF_DOUBLE_DATATYPE,&itr,&nx,&nw,&llwr,&llwi);
        zbeshv (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K,stk(lr),stk(li),stk(llwr),stk(llwi),&ierr);
        LhsVar(1) = lpos + 1;
    }
    else if ((m1==1 && n2==1)|| (n1==1 && m2==1)) 
    { 
        /* bessely(row,col) or bessely(col,row) */
        int lun = 1, nw = 0;
        mr = m2*n2;
        nr = m1*n1;
        CreateCVar(lpos+1,MATRIX_OF_DOUBLE_DATATYPE,&itr,&mr,&nr,&lr,&li);
        nx = m2*n2;
        na = m1*n1;
        nw = 3*na;
        CreateCVar(lpos+2,MATRIX_OF_DOUBLE_DATATYPE,&itr,&lun,&nw,&lwr,&lwi);
        zbeshv (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K, stk(lr),stk(li),stk(lwr),stk(lwi),&ierr);

        LhsVar(1) = lpos + 1;
    }
    else 
    { 
        /* element wise case */
        double wr[2],wi[2];
        CheckDimProp(1,2,m1*n1!=m2*n2) 
            mr = m2;
        nr = n2;  
        CreateCVar(lpos+1,MATRIX_OF_DOUBLE_DATATYPE,&itr,&mr,&nr,&lr,&li);
        LhsVar(1) = lpos + 1;
        nx = mr * nr;
        na = -1;
        zbeshv (stk(l2),stk(l2i),&nx,stk(l1),&na,&kode,&K,stk(lr),stk(li),wr,wi,&ierr);
    }
    if (ierr == 2) 
    {
        if ( C2F(errgst).ieee == 0) 
        { 
            ierr = 69;
            SciError(ierr);
        }
        else if (C2F(errgst).ieee == 1) 
        {
            ierr = 63;
            C2F(msgs)(&ierr,&un);
        }
    }
    else if (ierr == 3) 
    {
        /* inaccurate result */
        ierr = 4;
        C2F(msgs)(&ierr,&un);
    }
    else if (ierr == 4 || ierr == 5) 
    {
        if ( C2F(errgst).ieee==0) 
        { 
            ierr = 69;
            SciError(ierr);
        }
        else if ( C2F(errgst).ieee==1) 
        {
            ierr = 107;
            C2F(msgs)(&ierr,&un);
        }
    }

    PutLhsVar();
    return 0;
}  
/*--------------------------------------------------------------------------*/
