/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_data_property.c                                              */
/* desc : function to modify in Scilab the data field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "BasicAlgos.h"
#include "set_data_property.h"
#include "MALLOC.h"
#include "sci_types.h"

#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"

#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the champ data is now set as a tlist (like for surface objects) */
/* setchampdata(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setchampdata(void* _pvCtx, char* pobjUID, AssignedList * tlist )
{
    int nbRow[4];
    int nbCol[4];

    int numberArrows = 0;
    int dimensions[2];

    double * vx  = NULL;
    double * vy  = NULL;
    double * vfx = NULL;
    double * vfy = NULL;

    /* get parameters */
    vx  = getCurrentDoubleMatrixFromList( tlist, &nbRow[0], &nbCol[0] );
    vy  = getCurrentDoubleMatrixFromList( tlist, &nbRow[1], &nbCol[1] );
    vfx = getCurrentDoubleMatrixFromList( tlist, &nbRow[2], &nbCol[2] );
    vfy = getCurrentDoubleMatrixFromList( tlist, &nbRow[3], &nbCol[3] );

    /* check dim */
    if ( nbCol[0] != 1 || nbCol[1] != 1 )
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Columns vectors expected.\n"), "Tlist", 1);
        return SET_PROPERTY_ERROR;
    }

    numberArrows = nbRow[0] * nbRow[1];
    dimensions[0] = nbRow[0];
    dimensions[1] = nbRow[1];

    if ( nbRow[2] != nbRow[0] || nbCol[2] != nbRow[1] || nbRow[3] != nbRow[2] || nbCol[3] != nbCol[2] )
    {
        Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Incompatible length.\n"), "Tlist", 3, 4);
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow[0] * nbCol[0] == 0 || nbRow[1] * nbCol[1] == 0 || nbRow[2] * nbCol[2] == 0 || nbRow[3] * nbCol[3] == 0 )
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    /* Update the champ's number of arrows and dimensions then set the coordinates */
    setGraphicObjectProperty(pobjUID, __GO_NUMBER_ARROWS__, &numberArrows, jni_int, 1);
    setGraphicObjectProperty(pobjUID, __GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);

    setGraphicObjectProperty(pobjUID, __GO_BASE_X__, vx, jni_double_vector, dimensions[0]);
    setGraphicObjectProperty(pobjUID, __GO_BASE_Y__, vy, jni_double_vector, dimensions[1]);
    setGraphicObjectProperty(pobjUID, __GO_DIRECTION_X__, vfx, jni_double_vector, dimensions[0]*dimensions[1]);
    setGraphicObjectProperty(pobjUID, __GO_DIRECTION_Y__, vfy, jni_double_vector, dimensions[0]*dimensions[1]);

    return SET_PROPERTY_SUCCEED;
}

/*--------------------------------------------------------------------------*/
/* F.Leray 29.04.05 */
/* the grayplot data is now set as a tlist (like for surface and champ objects) */
/* setgrayplot(pobj,cstk(l2), &l3, &numrow3, &numcol3, fname) */
int setgrayplotdata(void* _pvCtx, char* pobjUID, AssignedList * tlist )
{
    BOOL result;

    int nbRow[3];
    int nbCol[3];
    int gridSize[4];

    double * pvecx = NULL;
    double * pvecy = NULL;
    double * pvecz = NULL;

    pvecx = getDoubleMatrixFromList(tlist, 2, &nbRow[0], &nbCol[0]);
    pvecy = getDoubleMatrixFromList(tlist, 3, &nbRow[1], &nbCol[1]);
    pvecz = getDoubleMatrixFromList(tlist, 4, &nbRow[2], &nbCol[2]);

    if ( nbCol[0] != 1 || nbCol[1] != 1 )
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Columns vectors expected.\n"), "Tlist", 1);
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow[2] != nbRow[0] || nbCol[2] != nbRow[1] )
    {
        Scierror(999, _("%s: Wrong size for arguments #%d: Incompatible length.\n"), "Tlist", 3);
        /* Was previously: */
#if 0
        Scierror(999, _("%s: Wrong size for arguments #%d: Incompatible length.\n"), "Tlist", "Tlist", 3);
#endif
        return 0;
    }

    if ( nbRow[0] * nbCol[0] == 0 || nbRow[1] * nbCol[1] == 0 || nbRow[2] * nbCol[2] == 0 )
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    /*
     * Update the x and y vectors dimensions
     * These vectors are column ones
     */
    gridSize[0] = nbRow[0];
    gridSize[1] = 1;
    gridSize[2] = nbRow[1];
    gridSize[3] = 1;

    /* Resizes the coordinates arrays if required */
    result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    if (result == FALSE)
    {
        Scierror(999, _("%s: No more memory.\n"), "setgrayplotdata");
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, nbRow[0]);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, nbRow[1]);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, nbRow[2]*nbCol[2]);

    return SET_PROPERTY_SUCCEED;
}
/*--------------------------------------------------------------------------*/
/* set3ddata(pobj,cstk(l2), &l3, &numrow3, &numcol3) */
int set3ddata(void* _pvCtx, char* pobjUID, AssignedList * tlist )
{
    int type = -1;
    int *piType = &type;

    int m1, n1, m2, n2, m3, n3;
    int m3n, n3n;
    int isFac3d;

    double * pvecx = NULL;
    double * pvecy = NULL;
    double * pvecz = NULL;
    int dimvectx = 0;
    int dimvecty = 0;

    double* inputColors;
    int nbInputColors;

    // number of specified colors
    int nc = 0;

    int izcol;

    /* no copy now we just perform tests on the matrices */
    pvecx = getCurrentDoubleMatrixFromList( tlist, &m1, &n1 );
    pvecy = getCurrentDoubleMatrixFromList( tlist, &m2, &n2 );
    pvecz = getCurrentDoubleMatrixFromList( tlist, &m3, &n3 );

    if ( m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1 )
    {
        if ( !(m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3) )
        {
            Scierror(999, _("%s: Wrong size for arguments #%d, #%d and #%d: Incompatible length.\n"), "Tlist", 1, 2, 3);
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        if ( m2 * n2 != n3 )
        {
            Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Incompatible length.\n"), "Tlist", 2, 3);
            return SET_PROPERTY_ERROR;
        }
        if ( m1 * n1 != m3 )
        {
            Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Incompatible length.\n"), "Tlist", 1, 3);
            return SET_PROPERTY_ERROR;
        }
        if ( m1 * n1 <= 1 || m2 * n2 <= 1 )
        {
            Scierror(999, _("%s: Wrong size for arguments #%d and #%d: Should be >= %d.\n"), "Tlist", 1, 2, 2);
            return SET_PROPERTY_ERROR;
        }
    }

    if ( m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0 )
    {
        return sciReturnEmptyMatrix(_pvCtx);
    }

    /* get color size if exists */
    if ( getAssignedListNbElement( tlist ) == 4 )
    {
        getCurrentDoubleMatrixFromList( tlist, &m3n, &n3n ) ;
        if ( m3n * n3n == m3 * n3 )
        {
            /* the color is a matrix, with same size as Z */
            izcol = 2;
        }
        else if (m3n * n3n == n3 && (m3n == 1 || n3n == 1))
        {
            /* a vector with as many colors as facets */
            izcol = 1;
        }
        else
        {
            Scierror(999, _("Wrong size for %s element: A %d-by-%d matrix or a vector of size %d expected.\n"), "color", m3, n3, n3);
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        m3n = 0;
        n3n = 0;
        izcol = 0;
    }

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_FAC3D__)
    {
        isFac3d = 1;
    }
    else
    {
        isFac3d = 0;
    }

    if ( m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1 ) /* NG beg */
    {
        /* case isfac=1;*/
        if (isFac3d == 0)
        {
            Scierror(999, _("Can not change the %s of graphic object: its type is %s.\n"), "typeof3d", "SCI_PLOT3D");
            return SET_PROPERTY_ERROR;
        }
    }
    else
    {
        /* case isfac=0;*/
        if (isFac3d == 1)
        {
            Scierror(999, _("Can not change the %s of graphic object: its type is %s.\n"), "typeof3d", "SCI_FAC3D");
            return SET_PROPERTY_ERROR;
        }
    }

    /* check the monotony on x and y */

    if (isFac3d == 1)
    {
        /* x is considered as a matrix */
        dimvectx = -1;
    }
    else if ( m1 == 1 ) /* x is a row vector */
    {
        dimvectx = n1;
    }
    else if ( n1 == 1 ) /* x is a column vector */
    {
        dimvectx = m1;
    }
    else /* x is a matrix */
    {
        dimvectx = -1;
    }

    if ( dimvectx > 1 )
    {
        int monotony = checkMonotony( pvecx, dimvectx );
        if ( monotony == 0 )
        {
            Scierror(999, _("%s: Wrong value: Vector is not monotonous.\n"), "Objplot3d");
            return SET_PROPERTY_ERROR;
        }

        /* To be implemented within the MVC */
#if 0
        psurf->flag_x = monotony;
#endif
    }

    if (isFac3d == 1)
    {
        /* x is considered as a matrix */
        dimvecty = -1;
    }
    else if (m2 == 1) /* y is a row vector */
    {
        dimvecty = n2;
    }
    else if (n2 == 1) /* y is a column vector */
    {
        dimvecty = m2;
    }
    else /* y is a matrix */
    {
        dimvecty = -1;
    }

    if ( dimvecty > 1 )
    {
        int monotony = checkMonotony( pvecy, dimvecty );
        if ( monotony == 0 )
        {
            Scierror(999, _("%s: Wrong value: Vector is not monotonous.\n"), "Objplot3d");
            return SET_PROPERTY_ERROR;
        }

        /* To be implemented within the MVC */
#if 0
        psurf->flag_y = monotony;
#endif
    }

    /* get the values now */
    rewindAssignedList( tlist );

    pvecx = getCurrentDoubleMatrixFromList( tlist, &m1, &n1 );
    pvecy = getCurrentDoubleMatrixFromList( tlist, &m2, &n2 );
    pvecz = getCurrentDoubleMatrixFromList( tlist, &m3, &n3 );

    if (isFac3d == 1)
    {
        int numElementsArray[3];
        int result;

        numElementsArray[0] = n1;
        numElementsArray[1] = m1;
        numElementsArray[2] = m3n * n3n;

        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);

        if (result == 0)
        {
            Scierror(999, _("%s: No more memory.\n"), "set3ddata");
            return SET_PROPERTY_ERROR;
        }
    }
    else if (isFac3d == 0)
    {
        int gridSize[4];
        int result;

        gridSize[0] = m1;
        gridSize[1] = n1;
        gridSize[2] = m2;
        gridSize[3] = n2;

        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

        if (result == 0)
        {
            Scierror(999, _("%s: No more memory.\n"), "set3ddata");
            return SET_PROPERTY_ERROR;
        }
    }

    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, m1 * n1);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, m2 * n2);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, m3 * n3);

    if ( getAssignedListNbElement( tlist ) == 4 ) /* F.Leray There is a color matrix */
    {
        inputColors = getCurrentDoubleMatrixFromList( tlist, &m3n, &n3n );
        nbInputColors = m3n * n3n;
    }
    else
    {
        inputColors = NULL;
        nbInputColors = 0;
    }

    /*
     * Plot 3d case not treated for now
     * To be implemented
     */
    if (isFac3d == 1)
    {
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COLORS__, inputColors, jni_double_vector, nbInputColors);
    }

    /* Color vector/matrix dimensions: to be checked for MVC implementation */
#if 0
    psurf->m3n = m3n; /* If m3n and n3n are 0, then it means that no color matrix/vector was in input*/
    psurf->n3n = n3n;
#endif

    return SET_PROPERTY_SUCCEED;
}
/*--------------------------------------------------------------------------*/
int set_data_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_CHAMP__)
    {
        AssignedList* tlist = NULL;
        int status = -1;

        if ( !(valueType  == sci_tlist ))
        {
            Scierror(999, "Incorrect argument, must be a Tlist!\n");
            return SET_PROPERTY_ERROR;
        }

        /* we should have 4 properties in the tlist */
        tlist = createAssignedList( 3, 4 );
        if ( tlist == NULL )
        {
            return SET_PROPERTY_ERROR;
        }

        status = setchampdata(_pvCtx, pobjUID, tlist);
        destroyAssignedList( tlist );
        return status;
    }
    else if (type == __GO_GRAYPLOT__)
    {
        AssignedList * tlist = NULL;
        int status = -1;

        if ( !(valueType  == sci_tlist ))
        {
            Scierror(999, _("Wrong type for input argument: Tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        /* we should have 3 properties in the tlist */
        tlist = createAssignedList( 3, 3 );
        if ( tlist == NULL )
        {
            return SET_PROPERTY_ERROR;
        }

        status = setgrayplotdata(_pvCtx, pobjUID, tlist);
        destroyAssignedList( tlist );
        return status;
    }
    else if (type == __GO_FAC3D__ || type == __GO_PLOT3D__)
    {
        AssignedList * tlist = NULL;
        int status = -1;
        int listSize = 0;

        if ( !(valueType  == sci_tlist ))
        {
            Scierror(999, _("Wrong type for input argument: Tlist expected.\n"));
            return SET_PROPERTY_ERROR ;
        }

        listSize = getStackListNbElement( 3 );

        if ( listSize == 3 )
        {
            tlist = createAssignedList( 3, 3 );
        }
        else if ( listSize == 4 )
        {
            tlist = createAssignedList( 3, 4 );
        }
        else
        {
            Scierror(999, _("Wrong size for input argument: %d or %d expected.\n"), 4, 5);
            return SET_PROPERTY_ERROR;
        }

        if ( tlist == NULL )
        {
            return SET_PROPERTY_ERROR;
        }

        status = set3ddata(_pvCtx, pobjUID, tlist);
        destroyAssignedList( tlist );
        return status;

    }
    else  /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
    {
        if ( !( valueType == sci_matrix ) )
        {
            Scierror(999, _("Incompatible type for property %s.\n"), "data");
            return SET_PROPERTY_ERROR;
        }

        return sciSetPoint( pobjUID, stk( stackPointer ), &nbRow, &nbCol );
    }
    return SET_PROPERTY_ERROR;

}
/*------------------------------------------------------------------------*/
