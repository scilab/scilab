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
/* file: sci_xarc.h                                                       */
/* desc : interface for xarc routine                                      */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "GetProperty.h"
#include "stack-c.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "CurrentSubwin.h"
#include "BuildObjects.h"

/*--------------------------------------------------------------------------*/
int sci_xarc(char *fname,unsigned long fname_len)
{
    char* psubwinUID = NULL;
    int m1 = 0,n1 = 0,l1 = 0,l2 = 0,l3 = 0,l4 = 0,l5 = 0,l6 = 0;
    long hdl = 0;
    int curcolor = 0;
    int *piCurColor = &curcolor;
    double angle1 = 0.0;
    double angle2 = 0.0;


    CheckRhs(6,6);
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);CheckScalar(1,m1,n1);
    GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l2);CheckScalar(2,m1,n1);
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l3);CheckScalar(3,m1,n1);
    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l4);CheckScalar(4,m1,n1);
    GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l5);CheckScalar(5,m1,n1);
    GetRhsVar(6,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l6);CheckScalar(6,m1,n1);

    angle1 = DEG2RAD(*istk(l5) / 64.0); /* convert to radian */
    angle2 = DEG2RAD(*istk(l6) / 64.0);

    psubwinUID = (char*)getOrCreateDefaultSubwin();

    getGraphicObjectProperty(psubwinUID, __GO_LINE_COLOR__, jni_int, (void**)&piCurColor);

    if ( strcmp(fname,"xarc") == 0 )
    {
        Objarc (&angle1,&angle2,stk(l1),stk(l2),stk(l3),stk(l4),&curcolor,NULL,FALSE,TRUE,&hdl);
    }
    else /* xfarc case */
    {
        Objarc (&angle1,&angle2,stk(l1),stk(l2),stk(l3),stk(l4),NULL,&curcolor,TRUE,FALSE,&hdl);
    }

    LhsVar(1)=0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
