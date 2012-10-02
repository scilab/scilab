/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_StringBox.c                                                  */
/* desc : interface for StringBox routine                                 */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "gw_graphics.h"
#include "Scierror.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "StringBox.h"
#include "localization.h"
#include "axesScale.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "freeArrayOfString.h"
#include "BuildObjects.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#define DEFAULT_ANGLE 0.0

/*--------------------------------------------------------------------------*/
static int getScalarFromStack(int paramIndex, char * funcName, double * res);
/*--------------------------------------------------------------------------*/
static int getScalarFromStack(int paramIndex, char * funcName, double * res)
{
    int m = 0;
    int n = 0;
    size_t stackPointer = 0;
    if ( VarType(paramIndex) != sci_matrix )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), funcName, paramIndex);
        return -1 ;
    }

    /* get the handle */
    GetRhsVar( paramIndex, MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &stackPointer );

    if ( m * n != 1 )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Real scalar expected.\n"), funcName, paramIndex);
        return -1 ;
    }

    *res = *(stk(stackPointer));
    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_stringbox( char * fname, unsigned long fname_len )
{
    int type = -1;
    int *piType = &type;
    char* parentAxes = NULL;
    double* textCorners = NULL;
    int two   = 2;
    int four  = 4;
    size_t stackPointer = 0;
    double corners[4][2]; /* the four edges of the boundingRect */

    /* The function should be called with stringbox( handle ) */
    CheckRhs( 1, 6 );
    CheckLhs( 0, 1 );

    if (Rhs == 1)
    {
        int m;
        int n;
        /* A text handle should be specified */

        char * pTextUID = NULL;
        if ( VarType(1) != sci_handles )
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
            return 0 ;
        }

        /* get the handle */
        GetRhsVar( 1, GRAPHICAL_HANDLE_DATATYPE, &m, &n, &stackPointer );
        if ( m * n != 1 )
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
            return 0 ;
        }

        /* Get the handle and check that this is a text handle */
        pTextUID = (char*)getObjectFromHandle(getHandleFromStack(stackPointer));

        if ( pTextUID == NULL )
        {
            Scierror(999, _("%s: The handle is not valid.\n"), fname);
            return 0 ;
        }

        getGraphicObjectProperty(pTextUID, __GO_TYPE__, jni_int, (void **)&piType);

        if (type != __GO_LABEL__ && type != __GO_TEXT__)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
            return 0;
        }

        getGraphicObjectProperty(pTextUID, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);

        updateTextBounds(pTextUID);

        /*
         * To do: performs a projection/unprojection to obtain the bounding box in object coordinates
         * but using a rotation matrix corresponding to the default rotation angles (view == 2d)
         */

        getGraphicObjectProperty(pTextUID, __GO_CORNERS__, jni_double_vector, (void **)&textCorners);

        corners[1][0] = textCorners[0];
        corners[1][1] = textCorners[1];

        corners[0][0] = textCorners[3];
        corners[0][1] = textCorners[4];

        corners[3][0] = textCorners[6];
        corners[3][1] = textCorners[7];

        corners[2][0] = textCorners[9];
        corners[2][1] = textCorners[10];
    }
    else if (Rhs == 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d to %d expected.\n"), fname, 1, 3, 6);
        return 0 ;
    }
    else
    {
        char * parentSubwinUID = (char*)getOrCreateDefaultSubwin();
        char ** text = NULL;
        int textNbRow;
        int textNbCol;
        double xPos;
        double yPos;
        double angle = DEFAULT_ANGLE;
        int fontId;
        int *pfontId = &fontId;
        double fontSize;
        double *pfontSize = &fontSize;

        getGraphicObjectProperty(parentSubwinUID, __GO_FONT_STYLE__, jni_int, (void**)&pfontId);
        getGraphicObjectProperty(parentSubwinUID, __GO_FONT_SIZE__, jni_double, (void **)&pfontSize);

        /* Check that first argument is a string */
        if ( VarType(1) != sci_strings )
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: 2D array of strings expected.\n"), fname, 1);
            return 0 ;
        }
        GetRhsVar( 1, MATRIX_OF_STRING_DATATYPE, &textNbRow, &textNbCol, &stackPointer );
        /* retrieve it */
        text = getStringMatrixFromStack(stackPointer);

        /* Second and third arguments should be scalars */
        if (getScalarFromStack(2, fname, &xPos) < 0)
        {
            freeArrayOfString(text, textNbRow * textNbCol);
            return 0;
        }
        if (getScalarFromStack(3, fname, &yPos) < 0)
        {
            freeArrayOfString(text, textNbRow * textNbCol);
            return 0;
        }

        if (Rhs >= 4)
        {
            /* angle is defined */
            if (getScalarFromStack(4, fname, &angle) < 0)
            {
                freeArrayOfString(text, textNbRow * textNbCol);
                return 0;
            }
        }

        if (Rhs >= 5)
        {
            double fontIdD;
            /* font style is defined */
            if (getScalarFromStack(5, fname, &fontIdD) < 0)
            {
                freeArrayOfString(text, textNbRow * textNbCol);
                return 0;
            }
            fontId = (int) fontIdD;
        }

        if (Rhs >= 6)
        {
            /* font size is defined */
            if (getScalarFromStack(6, fname, &fontSize) < 0)
            {
                freeArrayOfString(text, textNbRow * textNbCol);
                return 0;
            }
        }

        /* compute the box */
        getTextBoundingBox(text, textNbRow, textNbCol, xPos, yPos, angle, fontId, fontSize, corners);
        freeArrayOfString(text, textNbRow * textNbCol);
    }


    /* copy everything into the lhs */
    stackPointer = 0; /* Fix for 64 bits: MSB of stackPointer has been set by GetRhsVar but are not reset by CreateVar */
    CreateVar( Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &two, &four, &stackPointer );
    *stk( stackPointer     )  = corners[1][0] ;
    *stk( stackPointer + 1 )  = corners[1][1] ;
    *stk( stackPointer + 2 )  = corners[0][0] ;
    *stk( stackPointer + 3 )  = corners[0][1] ;
    *stk( stackPointer + 4 )  = corners[3][0] ;
    *stk( stackPointer + 5 )  = corners[3][1] ;
    *stk( stackPointer + 6 )  = corners[2][0] ;
    *stk( stackPointer + 7 )  = corners[2][1] ;

    LhsVar( 1 ) = Rhs + 1 ;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
#undef DEFAULT_ANGLE
