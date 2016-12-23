/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to BUILD new objects :
 - allocating memory
 - setting default value
 - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>

#include "BuildObjects.h"
#include "CurrentObject.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "CloneObjects.h"
#include "StringMatrix.h"
#include "Scierror.h"
#include "CurrentFigure.h"
#include "FigureList.h"
#include "localization.h"
#include "Interaction.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"
#include "loadTextRenderingAPI.h"

#include "sci_malloc.h"             /* MALLOC */
#include "Scierror.h"

#include "Format.h"             // computeDefaultTicsLabels

#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "CurrentObject.h"
#include "FigureModel.h"
#include "AxesModel.h"
#include "createGraphicObject.h"


/**
 * If a current subwin exists: return it
 * Otherwise create a new figure with JoGLView.
 **/
GRAPHICS_IMPEXP int getOrCreateDefaultSubwin(void)
{
    int iSubWinUID = getCurrentSubWin();

    if (iSubWinUID == 0)
    {
        int iNewId = getValidDefaultFigureId();
        int iFig = createNewFigureWithAxes();
        //set new figure id
        setGraphicObjectProperty(iFig, __GO_ID__, &iNewId, jni_int, 1);
        // the current figure,
        iSubWinUID = getCurrentSubWin();
    }

    return iSubWinUID;
}

/**ConstructText
 * This function creates the parents window (manager) and the elementaries structures
 * @param  char *pparentsubwinUID : parent subwin UID
 * @param  char * text[] : initial text matrix string.
 * @param  int nbCol : the number column of the text
 * @param  int nbRow : the number of row of the text
 * @return  : object UID if ok , NULL if not
 */
int ConstructText(int iParentsubwinUID, char **text, int nbRow, int nbCol, double x,
                  double y, BOOL autoSize, double userSize[2], BOOL centerPos, int *foreground, int *background,
                  BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align)
{
    int parentType = -1;
    int *piParentType = &parentType;
    int iObj = 0;

    getGraphicObjectProperty(iParentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return 0;
    }

    iObj = createText(iParentsubwinUID, text, nbRow, nbCol, x, y,
                      autoSize, (double*)userSize, centerPos, foreground, background, isboxed, isline, isfilled, align);

    setGraphicObjectRelationship(iParentsubwinUID, iObj);
    setCurrentObject(iObj);
    return getCurrentObject();
}

/**ConstructLegend
 * This function creates a Legend structure
 * @param  char *pparentsubwinUID : parent subwin UID
 * @param  char ** text : initial text matrix string array
 * @param long long tabofhandles[] : the array of polyline handles
 * @param int nblegends : the number of legend items
 * @return : object UID if ok , NULL if not
 */
int ConstructLegend(int iParentsubwinUID, char **text, int* tabofhandles, int nblegends)
{
    int iObj = 0;
    iObj = createLegend(iParentsubwinUID, text, nblegends, tabofhandles, nblegends);
    setCurrentObject(iObj);

    return iObj;
}

/*---------------------------------------------------------------------------------*/
/**
 * Create a polyline but does not add it to Scilab hierarchy
 */
int ConstructPolyline(int iParentsubwinUID, double *pvecx, double *pvecy, double *pvecz,
                      int closed, int n1, int plot, int *foreground, int *background,
                      int *mark_style, int *mark_foreground, int *mark_background, BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    int iObj = 0;
    int i = 0;
    BOOL result = FALSE;
    int iPolyline = 0;
    double *dataVector = NULL;
    int numElementsArray[2];
    int zCoordinatesSet = 0;
    int iBackgroundSize = 1;

    if (background != NULL)
    {
        if (isinterpshaded == TRUE)
        {
            iBackgroundSize = n1;
        }
    }
    else
    {
        iBackgroundSize = 0;
    }

    iObj = createPolyline(iParentsubwinUID, closed > 0 ? TRUE : FALSE, plot, foreground, background, iBackgroundSize,
                          mark_style, mark_foreground, mark_background, isline, isfilled, ismark, isinterpshaded);

    iPolyline = createDataObject(iObj, __GO_POLYLINE__);

    if (iPolyline == 0)
    {
        deleteGraphicObject(iObj);
        return 0;
    }


    /*
     * First element: number of gons (always 1 for a Polyline)
     * Second one: number of vertices composing the Polyline
     */
    numElementsArray[0] = 1;
    numElementsArray[1] = n1;

    /* Data */
    if (n1 != 0)
    {
        /*
         * Sets the number of elements (vertices composing the polyline) and allocates the coordinates array
         * The FALSE value is used to identify a failed memory allocation for now.
         */
        result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

        if (result == FALSE)
        {
            deleteGraphicObject(iObj);
            deleteDataObject(iObj);
            return 0;
        }

        dataVector = (double*)MALLOC(3 * n1 * sizeof(double));

        if (dataVector == NULL)
        {
            deleteGraphicObject(iObj);
            deleteDataObject(iObj);
            return 0;
        }

        if (pvecx && pvecy)
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[i] = pvecx[i];
                dataVector[n1 + i] = pvecy[i];
            }
        }
        else
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[i] = 0.0;
                dataVector[n1 + i] = 0.0;
            }
        }

        /**DJ.Abdemouche 2003**/
        if (pvecz == NULL)
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[2 * n1 + i] = 0.0;
            }

            zCoordinatesSet = 0;
        }
        else
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[2 * n1 + i] = pvecz[i];
            }

            zCoordinatesSet = 1;
        }

        /*
         * We could probably do without the dataVector copy by individually setting
         * x, y or z coordinates, and initializing coordinates to 0 during allocation
         * to ensure consistency
         */
        setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_COORDINATES__, dataVector, jni_double, n1);

        FREE(dataVector);

        setGraphicObjectProperty(iObj, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_double, n1);
    }
    else
    {
        /* 0 points */
        result = setGraphicObjectProperty(iObj, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

        if (result == FALSE)
        {
            deleteGraphicObject(iObj);
            deleteDataObject(iObj);
            return 0;
        }
    }

    return iObj;
}

/**ConstructSurface
 * This function creates Surface Structure
 */
int ConstructSurface(int iParentsubwinUID, sciTypeOf3D typeof3d,
                     double *pvecx, double *pvecy, double *pvecz, double *zcol,
                     int izcol, int dimzx, int dimzy,
                     int *flag, double *ebox, int flagcolor, int *isfac, int *m1, int *n1, int *m2, int *n2, int *m3, int *n3, int *m3n, int *n3n)
{
    int iObj = 0;
    int parentType = -1;
    int *piParentType = &parentType;
    int const surfaceTypes[2] = { __GO_PLOT3D__, __GO_FAC3D__ };

    double *clipRegion = NULL;

    int nx = 0, ny = 0, nz = 0, nc = 0;
    int result = 0;
    int hiddenColor = 0;
    int *piHiddenColor = &hiddenColor;
    int surfaceMode = 0;

    /* To be modified: the MVC does not allow Plot3d objects with color data yet */
    if (typeof3d == SCI_PLOT3D)
    {
        nx = dimzx;
        ny = dimzy;
        nz = dimzx * dimzy;
        if (flagcolor == 2)
        {
            /* one color per facet: nc = dimzx * dimzy */
            nc = nz;
        }
        else if (flagcolor == 3)
        {
            /*
             * one color per edge: nc = 4* dimzx * dimzy ??????
             * 3 or 4 vertices are needed: I think we take 4 to have enough allocated memory
             */
            nc = nz * 4;
        }
        /* made by Djalel : comes from the genfac3d case */
        else
        {
            nc = 0;
        }
    }
    /* DJ.A 2003 */
    else
    {
        /* case SCI_FAC3D */
        nx = dimzx * dimzy;
        ny = dimzx * dimzy;
        nz = dimzx * dimzy;
        if (flagcolor == 2)
        {
            /* one color per facet: nc = dimzy */
            nc = dimzy;
        }
        else if (flagcolor == 3)
        {
            /* one color per edge: nc = dimzx * dimzy */
            nc = nz;
        }
        else
        {
            nc = 0;
        }
    }

    getGraphicObjectProperty(iParentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    /* test using sciGetEntityType replaced by a test on the type string */
    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return 0;
    }

    //iObj = createGraphicObject(surfaceTypes[*isfac]);
    iObj = createSurface(iParentsubwinUID, surfaceTypes[*isfac], flagcolor, *flag);
    createDataObject(iObj, surfaceTypes[*isfac]);

    /* Plot3d case */
    if (!*isfac)
    {
        int gridSize[4];

        gridSize[0] = *m1;
        gridSize[1] = *n1;
        gridSize[2] = *m2;
        gridSize[3] = *n2;

        /* Allocates the coordinates arrays */
        result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    }
    /* Fac3d case */
    else
    {
        int numElementsArray[3];

        /*
         * First element: number of n-gons
         * Second element: number of vertices per n-gon
         * Third element: number of input colors
         */
        numElementsArray[0] = dimzy;
        numElementsArray[1] = dimzx;
        numElementsArray[2] = nc;

        /* Allocates the coordinates and color arrays */
        result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);
    }

    if (result == 0)
    {
        deleteGraphicObject(iObj);
        deleteDataObject(iObj);
        return 0;
    }

    setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, nx);
    setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, ny);

    /* Add the color matrix dimensions as a property ? */
    if (nc > 0)
    {
        setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, nz);
        setGraphicObjectProperty(iObj, __GO_DATA_MODEL_COLORS__, zcol, jni_double_vector, nc);
    }
    else
    {
        setGraphicObjectProperty(iObj, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, nz);
    }

    return iObj;
}

/********************** 14/05/2002 *****
 **ConstructGrayplot
 * This function is used to build Grayplot and Matplot objects.
 * It would probably be better to put the code related to Matplot objects
 * in a separate build function, as it would avoid having to perform several tests
 * on the type parameter. This is done so because Matplot objects were previously
 * internally represented by sciGrayplot structures.
 */
int ConstructGrayplot(int iParentsubwinUID, double *pvecx, double *pvecy, double *pvecz, int n1, int n2, int type)
{
    int iObj = 0;

    int const objectTypes[3] = { __GO_GRAYPLOT__, __GO_MATPLOT__, __GO_MATPLOT__ };

    int typeParent = -1;
    int *piTypeParent = &typeParent;

    int iGrayplotID = 0;
    int result = 0;
    int dataMapping = 0;
    int gridSize[4];

    int numElements = 0;

    getGraphicObjectProperty(iParentsubwinUID, __GO_TYPE__, jni_int, (void **)&piTypeParent);
    if (typeParent != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return 0;
    }

    iObj = createGrayplot(iParentsubwinUID, type, pvecx, 2, n1, n2);
    iGrayplotID = createDataObject(iObj, objectTypes[type]);

    if (iGrayplotID == 0)
    {
        deleteGraphicObject(iObj);
        return 0;
    }

    /* 0: scaled; only used for Grayplot */
    if (type == 0)
    {
        dataMapping = 0;
        setGraphicObjectProperty(iObj, __GO_DATA_MAPPING__, &dataMapping, jni_int, 1);
    }

    /* The x and y vectors are column ones */

    /*
     * For the Grayplot object, the number of rows and columns respectively
     * correspond to the grid's x and y dimensions whereas for Matplot objects,
     * they respectively correspond to the grid's y and x dimensions.
     */
    if (type == 0)
    {
        gridSize[0] = n1;
        gridSize[1] = 1;
        gridSize[2] = n2;
        gridSize[3] = 1;
    }
    else
    {
        gridSize[0] = n2;
        gridSize[1] = 1;
        gridSize[2] = n1;
        gridSize[3] = 1;
    }

    /* Allocates the coordinates arrays */
    result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    if (result == 0)
    {
        deleteGraphicObject(iObj);
        deleteDataObject(iObj);
        return 0;
    }

    /* Only for Grayplot objects, for Matplot objects, x and y coordinates are automatically computed */
    if (type == 0)
    {
        setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, n1);
        setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, n2);
    }

    if (type == 0)
    {
        numElements = n1 * n2;
    }
    else
    {
        numElements = (n1 - 1) * (n2 - 1);
    }

    setGraphicObjectProperty(iObj, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, numElements);

    setGraphicObjectRelationship(iParentsubwinUID, iObj);
    return iObj;
}

/********************** 14/05/2002 *****
 **ConstructGrayplot
 * This function is used to build Grayplot and Matplot objects.
 * It would probably be better to put the code related to Matplot objects
 * in a separate build function, as it would avoid having to perform several tests
 * on the type parameter. This is done so because Matplot objects were previously
 * internally represented by sciGrayplot structures.
 */
int ConstructImplot(int iParentsubwinUID, double *pvecx, unsigned char *pvecz, int n1, int n2, int plottype)
{
    int iObj = 0;

    int typeParent = -1;
    int *piTypeParent = &typeParent;

    int iGrayplotID = 0;
    int result = 0;
    int dataMapping = 0;
    int gridSize[4];

    int parentVisible = 0;
    int *piParentVisible = &parentVisible;
    double *clipRegion = NULL;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int numElements = 0;
    double pdblScale[2];

    getGraphicObjectProperty(iParentsubwinUID, __GO_TYPE__, jni_int, (void **)&piTypeParent);

    if (typeParent != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return 0;
    }

    iObj = createGraphicObject(__GO_MATPLOT__);
    iGrayplotID = createDataObject(iObj, __GO_MATPLOT__);

    if (iGrayplotID == 0)
    {
        deleteGraphicObject(iObj);
        return 0;
    }

    gridSize[0] = n2;
    gridSize[1] = 1;
    gridSize[2] = n1;
    gridSize[3] = 1;

    /* Allocates the coordinates arrays */
    result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    if (result == 0)
    {
        deleteGraphicObject(iObj);
        deleteDataObject(iObj);
        return 0;
    }

    /* Only for Matplot1 objects */
    if (pvecx)
    {
        setGraphicObjectProperty(iObj, __GO_MATPLOT_TRANSLATE__, pvecx, jni_double_vector, 2);
        pdblScale[0] = (pvecx[2] - pvecx[0]) / (n2 - 1.0);
        pdblScale[1] = (pvecx[3] - pvecx[1]) / (n1 - 1.0);
        setGraphicObjectProperty(iObj, __GO_MATPLOT_SCALE__, pdblScale, jni_double_vector, 2);
    }

    setGraphicObjectProperty(iObj, __GO_DATA_MODEL_MATPLOT_BOUNDS__, pvecx, jni_double_vector, 4);

    numElements = (n1 - 1) * (n2 - 1);
    if (plottype != -1)
    {
        setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_MATPLOT_DATA_INFOS__, &plottype, jni_int, 1);
    }

    setGraphicObjectProperty(iObj, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, pvecz, jni_double_vector, numElements);

    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */
    setGraphicObjectRelationship(iParentsubwinUID, iObj);

    getGraphicObjectProperty(iParentsubwinUID, __GO_VISIBLE__, jni_bool, (void **)&piParentVisible);
    setGraphicObjectProperty(iObj, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

    /*
     * Clip state and region
     * To be checked for consistency
     */
    getGraphicObjectProperty(iParentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipRegion);
    setGraphicObjectProperty(iObj, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);
    releaseGraphicObjectProperty(__GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(iParentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, (void **)&piClipRegionSet);
    setGraphicObjectProperty(iObj, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(iParentsubwinUID, __GO_CLIP_STATE__, jni_int, (void **)&piClipState);
    setGraphicObjectProperty(iObj, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* Initializes the default Contour values */
    cloneGraphicContext(iParentsubwinUID, iObj);

    return iObj;
}

/********************** 21/05/2002 *****
 **ConstructFec
 * This function creates Fec
 * @author Djalel.ABDEMOUCHE
 * @see sciSetCurrentObj
 */
int ConstructFec(int iParentsubwinUID, double *pvecx, double *pvecy, double *pnoeud,
                 double *pfun, int Nnode, int Ntr, int Nvertex, double *zminmax, int *colminmax, int *colout, BOOL with_mesh)
{
    int iObj = 0;
    int iFecId = 0;
    int result = 0;

    int parentType = -1;
    int *piParentType = &parentType;

    int parentVisible = 0;
    int *piParentVisible = &parentVisible;

    double *clipRegion = NULL;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int iClipState = 0;
    int *piClipState = &iClipState;

    getGraphicObjectProperty(iParentsubwinUID, __GO_TYPE__, jni_int, (void **)&piParentType);

    /* test using sciGetEntityType replaced by a test on the type string */
    if (parentType != __GO_AXES__)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return 0;
    }

    iObj = createFec(iParentsubwinUID, zminmax, 2, colminmax, 2, colout, 2, with_mesh);
    iFecId = createDataObject(iObj, __GO_FEC__);

    if (iFecId == 0)
    {
        deleteGraphicObject(iObj);
        return 0;
    }

    /* Allocates the coordinates array */
    result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_NUM_VERTICES__, &Nnode, jni_int, 1);

    if (result == 0)
    {
        deleteGraphicObject(iObj);
        deleteDataObject(iObj);
        return 0;
    }

    result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, &Nvertex, jni_int, 1);
    if (result == 0)
    {
        deleteGraphicObject(iObj);
        deleteDataObject(iObj);
        return 0;
    }

    /* Allocates the triangle indices and values array */
    result = setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_NUM_INDICES__, &Ntr, jni_int, 1);

    if (result == 0)
    {
        deleteGraphicObject(iObj);
        deleteDataObject(iObj);
        return 0;
    }

    setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, Nnode);
    setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, Nnode);

    /* Fec-specific property: triangle indices plus special values (triangle number and flag) */
    setGraphicObjectPropertyAndNoWarn(iObj, __GO_DATA_MODEL_FEC_ELEMENTS__, pnoeud, jni_double_vector, Ntr);

    /* Function values */
    setGraphicObjectProperty(iObj, __GO_DATA_MODEL_VALUES__, pfun, jni_double_vector, Nnode);

    setGraphicObjectRelationship(iParentsubwinUID, iObj);
    return iObj;
}

/**sciConstructCompound
 * constructs a Compound of entities
 * do only an association with a parent and a handle reservation !
 * check for valid handle can be done using CheckForCompound
 */
int ConstructCompound(long *handelsvalue, int number) /* Conflicting types with definition */
{
    int iCompoundUID = 0;
    int iParentAxesUID = 0;
    int iFirstMovedObjectUID = 0;

    int i = 0;
    int parentVisible = 0;
    int *piParentVisible = &parentVisible;

    iCompoundUID = createGraphicObject(__GO_COMPOUND__);

    /* Add the Compound's handle */
    /* The Compound's parent Axes is considered to be the Compound's first child's own parent */
    iFirstMovedObjectUID = getObjectFromHandle((long)handelsvalue[0]);
    iParentAxesUID = getParentObject(iFirstMovedObjectUID);

    /* Set the parent-child relationship between the Compound and each aggregated object */
    for (i = 0; i < number; i++)
    {
        int iMovedObjectUID = getObjectFromHandle((long)handelsvalue[i]);

        setGraphicObjectRelationship(iCompoundUID, iMovedObjectUID);
    }

    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(iParentAxesUID, iCompoundUID);

    getGraphicObjectProperty(iParentAxesUID, __GO_VISIBLE__, jni_bool, (void **)&piParentVisible);
    setGraphicObjectProperty(iCompoundUID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

    releaseGraphicObjectProperty(__GO_PARENT__, &iParentAxesUID, jni_int, 1);

    return iCompoundUID;
}

/**ConstructCompoundSeq
 * constructs a Compound of with the last n entities created in the current subwindow
 on entry the subwin children list is
 s1->s2->...->sn->sn+1->...->sN
 with sn the least recent of the last n entities created and s1 the most recent one
 (that is, the last entity created), and N the subwin's initial number of children
 on exit it is
 A->sn+1->sn+2->...->sN
 with A a Compound object whose children list is:
 s1->s2->...->sn-1->sn
*/
int ConstructCompoundSeq(int number)
{
    int* children = NULL;
    int iParentFigure = 0;
    int* piParent = &iParentFigure;
    int numberChildren = 0;
    int *piNumberChildren = &numberChildren;
    int i = 0;
    int visible = 0;
    int *piVisible = &visible;

    int iObj = 0;
    int iSubwinUID = getCurrentSubWin();

    /* Creates the Compound object A */
    iObj = createGraphicObject(__GO_COMPOUND__);

    /* Add the Compound's handle */
    getGraphicObjectProperty(iSubwinUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piNumberChildren);

    getGraphicObjectProperty(iSubwinUID, __GO_CHILDREN__, jni_int_vector, (void **)&children);

    /*
     * Remove the last "number" created objects (located at the children list's head)
     * and add them to the compound in the same order
     */
    if (children && piNumberChildren)
    {
        for (i = 0; i < number; i++)
        {
            /*
             * Set the parent-child relationship between the Compound and each aggregated object.
             * Children are added to the Compound from the least recent to the most recent, to
             * preserve their former ordering.
             */
            setGraphicObjectRelationship(iObj, children[number - i - 1]);
        }
        releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, numberChildren);
    }
    else
    {
        return -1;
    }

    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(iSubwinUID, iObj);

    /*
     * visibility is obtained from the parent Figure, whereas it is retrieved from the
     * parent Axes in ConstructCompound.
     * To be made consistent.
     */
    getGraphicObjectProperty(iObj, __GO_PARENT_FIGURE__, jni_int, (void **)&piParent);
    getGraphicObjectProperty(iParentFigure, __GO_VISIBLE__, jni_bool, (void **)&piVisible);
    setGraphicObjectProperty(iObj, __GO_VISIBLE__, &visible, jni_bool, 1);

    return iObj;
}

int ConstructLight(char* fname, int iSubwin, int type, BOOL visible, double * position, double * direction, double * ambient_color, double * diffuse_color, double * specular_color)
{
    int iLight = 0;
    int * piType = &type;
    int hType = 0;
    int * pihType = &hType;
    int * piVisible = &visible;

    if (iSubwin == 0)
    {
        iSubwin = getOrCreateDefaultSubwin();
        if (iSubwin == 0)
        {
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            return 0;
        }
    }
    else
    {
        //check handle type
        getGraphicObjectProperty(iSubwin, __GO_TYPE__, jni_int, (void **)&pihType);
        if (hType != __GO_AXES__)
        {
            Scierror(999, _("The parent has to be a SUBWIN\n"));
            return 0;
        }
    }

    iLight = createLight(iSubwin, type, visible, position, direction, ambient_color, diffuse_color, specular_color);
    return iLight;
}
