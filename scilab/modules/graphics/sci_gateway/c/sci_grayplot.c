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
/* file: sci_grayplot.c                                                   */
/* desc : interface for grayplot routine                                  */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "DefaultCommandArg.h"
#include "sciCall.h"
#include "localization.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_grayplot( char *fname, unsigned long fname_len )
{
    int frame_def=8;
    int *frame=&frame_def;
    int axes_def=1;
    int *axes=&axes_def;
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, m3 = 0, n3 = 0, l3 = 0;
    static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
                              {-1,"frameflag","?",0,0,0},
                              {-1,"nax","?",0,0,0},
                              {-1,"rect","?",0,0,0},
                              {-1,"strf","?",0,0,0},
                              {-1,NULL,NULL,0,0}};

    char   * strf    = NULL  ;
    double * rect    = NULL  ;
    int    * nax     = NULL  ;
    BOOL     flagNax = FALSE ;

    if (Rhs <= 0)
    {
        sci_demo(fname, fname_len);
        return 0;
    }
    CheckRhs(3,7);

    if ( get_optionals(fname,opts) == 0)
    {
        PutLhsVar();
        return 0 ;
    }

    if ( FirstOpt() < 4)
    {
        Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d.\n"),
                 fname,1, 4);
        return -1;
    }
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    CheckVector(1,m1,n1);
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    CheckVector(2,m2,n2);
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
    if (m3 * n3 == 0)
    {
        LhsVar(1)=0;
        PutLhsVar();
        return 0;
    }

    CheckDimProp(2,3,m2 * n2 != n3);
    CheckDimProp(1,3,m1 * n1 != m3);

    GetStrf(fname,4,opts,&strf);
    GetRect(fname,5,opts,&rect);
    GetNax(6,opts,&nax,&flagNax);

    getOrCreateDefaultSubwin();

    if ( isDefStrf( strf ) )
    {
        char strfl[4];

        strcpy(strfl,DEFSTRFN);

        strf = strfl;
        if ( !isDefRect( rect ) )
        {
            strfl[1]='7';
        }

        GetOptionalIntArg(fname,7,"frameflag",&frame,1,opts);
        if(frame != &frame_def)
        {
            strfl[1] = (char)(*frame+48);
        }
        GetOptionalIntArg(fname,7,"axesflag",&axes,1,opts);
        if(axes != &axes_def)
        {
            strfl[2] = (char)(*axes+48);
        }
    }

    Objgrayplot (stk(l1), stk(l2), stk(l3), &m3, &n3, strf, rect, nax, flagNax);

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
