/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - 
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "gw_statistics.h"
#include "stack-c.h"
#include "CdfBase.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(cdff)(int *,double *,double *,double*,double *,double *, int *,double *);
/*--------------------------------------------------------------------------*/
static void cdffErr(int status,double bound);
/*--------------------------------------------------------------------------*/
/*
*  hand written interface
*      Interface for cdff
*               Cumulative Distribution Function
*              F distribution
*/
/*--------------------------------------------------------------------------*/
int cdffI(char* fname,unsigned long l)
{
    int m1 = 0,n1 = 0,l1 = 0;
    Nbvars = 0;
    CheckRhs(4,5);
    CheckLhs(1,2);
    GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
    if ( strcmp(cstk(l1),"PQ")==0)
    {
        static int callpos[5] = {3,4,0,1,2};
        CdfBase(fname,3,2,callpos,"PQ",_("F,Dfn and Dfd"),1,C2F(cdff),
            cdffErr);
    }
    else if ( strcmp(cstk(l1),"F")==0)
    {
        static int callpos[5] = {2,3,4,0,1};
        CdfBase(fname,4,1,callpos,"F",_("Dfn,Dfd,P and Q"),2,C2F(cdff),
            cdffErr);
    }
    else if ( strcmp(cstk(l1),"Dfn")==0)
    {
        static int callpos[5] = {1,2,3,4,0};
        CdfBase(fname,4,1,callpos,"Dfn",_("Dfd,P,Q and F"),3,C2F(cdff),
            cdffErr);
    }
    else if ( strcmp(cstk(l1),"Dfd")==0)
    {
        static int callpos[5] = {0,1,2,3,4};
        CdfBase(fname,4,1,callpos,"Dfd",_("P,Q,F and Dfn"),4,C2F(cdff),
            cdffErr);
    }
    else
    {
        Scierror(999,_("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"),fname,1,"PQ","F","Dfn","Dfd");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static void cdffErr(int status,double bound)
{
    static char *param[7] = {"X", "P","Q","F","Dfn","Dfd"};
    switch ( status )
    {
    case 1 : 
        {
            cdfLowestSearchError(bound);
        }
        break;
    case 2 : 
        {
            cdfGreatestSearchError(bound);
        }
        break;
    case 3 :
        {
            Scierror(999," P + Q .ne. 1 \n");
        }
        break ;
    default :
        {
            CdfDefaultError(param, status, bound);
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
