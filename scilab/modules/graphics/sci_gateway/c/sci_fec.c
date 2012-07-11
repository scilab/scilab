/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_fec.c                                                        */
/* desc : interface for sci_fec routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "DefaultCommandArg.h"
#include "localization.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_fec(char *fname,unsigned long fname_len)
{
    int m1 = 0,n1 = 0,l1 = 0,m2 = 0,n2 = 0,l2 = 0,m3 = 0,n3 = 0,l3 = 0,m4 = 0,n4 = 0,l4 = 0, mn1 = 0;

    static rhs_opts opts[]= { {-1,"colminmax","?",0,0,0},
                              {-1,"colout","?",0,0,0},
                              {-1,"leg","?",0,0,0},
                              {-1,"mesh","?",0,0,0},
                              {-1,"nax","?",0,0,0},
                              {-1,"rect","?",0,0,0},
                              {-1,"strf","?",0,0,0},
                              {-1,"zminmax","?",0,0,0},
                              {-1,NULL,NULL,0,0}        } ;

    char * strf = NULL ;
    char * legend = NULL ;
    double * rect = NULL ;
    double * zminmax = NULL ;
    int * colminmax = NULL ;
    int * nax = NULL ;
    int * colOut = NULL ;
    BOOL flagNax = FALSE ;
    BOOL withMesh = FALSE ;


    if (Rhs <= 0)
    {
		sci_demo(fname, fname_len);
		return 0;
    }

    CheckRhs(4,12);

    if ( get_optionals(fname,opts) == 0)
    {
        PutLhsVar();
        return 0;
    }

    if ( FirstOpt() < 5)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"),fname,1, 5);
        return -1;
    }

    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
    CheckSameDims(1,2,m1,n1,m2,n2);

    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3);
    if (n3 != 5)
    {
        Scierror(999,_("%s: Wrong number of columns for input argument #%d: %d expected.\n"),fname,3,5);
        return 0;
    }

    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4);

    if (m1 * n1 == 0 || m3 == 0)
    {
        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    GetStrf(fname,5,opts,&strf);
    GetLegend(fname,6,opts,&legend);
    GetRect(fname,7,opts,&rect);
    GetNax(8,opts,&nax,&flagNax);
    GetZminmax(fname,9,opts,&zminmax);
    GetColminmax(fname,10,opts,&colminmax);
    GetColOut(fname,11,opts,&colOut);
    GetWithMesh(fname,12,opts,&withMesh);

    getOrCreateDefaultSubwin();

    if ( isDefStrf ( strf ) ) {
        char strfl[4];

        strcpy(strfl,DEFSTRFN);

        strf = strfl;
        if ( !isDefRect( rect ))
        {
            strfl[1]='7';
        }
        if ( !isDefLegend( legend ) )
        {
            strfl[0]='1';
        }
    }
    mn1 = m1 * n1;

    Objfec (stk(l1),stk(l2),stk(l3),stk(l4),&mn1,&m3,strf,legend,rect,nax,zminmax,colminmax,colOut,withMesh,flagNax);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
