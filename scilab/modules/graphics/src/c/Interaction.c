/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    Comment:
 *    This file contains all functions used to INTERACT with the graphic
 *    window (zoom by pressing button, callbacks...)
 --------------------------------------------------------------------------*/
#include <string.h>
#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "JavaInteraction.h"
#include "axesScale.h"
#include "math_graphics.h"
#include "HandleManagement.h"

#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"


/*-------------------------------------------------------------------------------------*/
static int moveObj(char* pobjUID, double displacement[], int displacementSize);
/*-------------------------------------------------------------------------------------*/

/**sciAddCallback
 * exec the callback associated with pthis
 */
int
sciAddCallback (sciPointObj * pthis, char *code, int len, int mevent )
{
    /* Destruction de l'ancienne callback si elle existait */
    sciDelCallback (pthis);

    switch (sciGetEntityType (pthis))
    {
        case SCI_SUBWIN:
            if ((pSUBWIN_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pSUBWIN_FEATURE (pthis)->callback, code, len);
                pSUBWIN_FEATURE (pthis)->callbacklen = len;
                pSUBWIN_FEATURE (pthis)->callbackevent = mevent;
            }
            break;
        case SCI_ARC:
            if ((pARC_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pARC_FEATURE (pthis)->callback, code, len);
                pARC_FEATURE (pthis)->callbacklen = len;
                pARC_FEATURE (pthis)->callbackevent = mevent;

            }
            break;
        case SCI_RECTANGLE:
            if ((pRECTANGLE_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pRECTANGLE_FEATURE (pthis)->callback, code, len);
                pRECTANGLE_FEATURE (pthis)->callbacklen = len;
                pRECTANGLE_FEATURE (pthis)->callbackevent = mevent;

            }
            break;
        case SCI_SEGS:
            if ((pSEGS_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pSEGS_FEATURE (pthis)->callback, code, len);
                pPOLYLINE_FEATURE (pthis)->callbacklen = len;
                pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;

            }
            break;
        case SCI_FEC:
            if ((pFEC_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pFEC_FEATURE (pthis)->callback, code, len);
                pPOLYLINE_FEATURE (pthis)->callbacklen = len;
                pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;

            }
            break;
        case SCI_GRAYPLOT:
            if ((pGRAYPLOT_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pGRAYPLOT_FEATURE (pthis)->callback, code, len);
                pPOLYLINE_FEATURE (pthis)->callbacklen = len;
                pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;

            }
            break;
        case SCI_POLYLINE:
            if ((pPOLYLINE_FEATURE (pthis)->callback = CALLOC (len + 1, sizeof (char))) == NULL)
            {
                Scierror(9999, _("%s: No more memory.\n"), "sciAddCallback");
                return -1;
            }
            else
            {
                strncpy(pPOLYLINE_FEATURE (pthis)->callback, code, len);
                pPOLYLINE_FEATURE (pthis)->callbacklen = len;
                pPOLYLINE_FEATURE (pthis)->callbackevent = mevent;

            }
            break;
        case SCI_UICONTROL:
        case SCI_UIMENU:
            break;
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_FIGURE:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */

        default:
            Scierror(999, _("No Callback is associated with this Entity.\n"));
            return -1;
            break;
    }
    return -1;
}




/**sciGetCallback
 * returns the scilab callback code
 */
char *sciGetCallback(sciPointObj * pthis)
{
    switch (sciGetEntityType (pthis))
    {
        case SCI_SUBWIN:
            return (char *)(pSUBWIN_FEATURE(pthis)->callback);
            break;
        case SCI_ARC:
            return (char *)(pARC_FEATURE(pthis)->callback);
            break;
        case SCI_SEGS:
            return (char *)(pSEGS_FEATURE(pthis)->callback);
            break;
        case SCI_FEC:
            return (char *)(pFEC_FEATURE(pthis)->callback);
            break;
        case SCI_GRAYPLOT:
            return (char *)(pGRAYPLOT_FEATURE(pthis)->callback);
            break;
        case SCI_POLYLINE:
            return (char *)(pPOLYLINE_FEATURE(pthis)->callback);
            break;
        case SCI_RECTANGLE:
            return (char *)(pRECTANGLE_FEATURE(pthis)->callback);
            break;
        case SCI_TEXT:
            return (char *)(pTEXT_FEATURE(pthis)->callback);
            break;
        case SCI_SURFACE:
            return (char *)(pSURFACE_FEATURE(pthis)->callback);
            break;
        case SCI_AXES:
            return (char *)(pAXES_FEATURE(pthis)->callback);
            break;
        case SCI_UIMENU:
            return (char *)(pUIMENU_FEATURE(pthis)->callback);
            break;
        case SCI_UICONTROL:
            return (char *)(pUICONTROL_FEATURE(pthis)->callback);
            break;
        case SCI_LEGEND:
        case SCI_FIGURE:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        default:
            Scierror(999, _("No Callback is associated with this Entity.\n"));
            return (char *)NULL;
            break;
    }
}

/* 07/11/2002*/
/**sciGetCallbackMouseEvent
 * @author Djalel Abdemouche
 */
int sciGetCallbackMouseEvent(sciPointObj * pthis)
{
    switch (sciGetEntityType (pthis))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pthis)->callbackevent;
            break;
        case SCI_ARC:
            return pARC_FEATURE(pthis)->callbackevent;
            break;
        case SCI_SEGS:
            return pSEGS_FEATURE(pthis)->callbackevent;
            break;
        case SCI_FEC:
            return pFEC_FEATURE(pthis)->callbackevent;
            break;
        case SCI_GRAYPLOT:
            return pGRAYPLOT_FEATURE(pthis)->callbackevent;
            break;
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE(pthis)->callbackevent;
            break;
        case SCI_RECTANGLE:
            return pRECTANGLE_FEATURE(pthis)->callbackevent;
            break;
        case SCI_TEXT:
            return pTEXT_FEATURE(pthis)->callbackevent;
            break;
        case SCI_SURFACE:
            return pSURFACE_FEATURE(pthis)->callbackevent;
            break;
        case SCI_AXES:
            return pAXES_FEATURE(pthis)->callbackevent;
            break;
        case SCI_UIMENU:
        case SCI_LEGEND:
        case SCI_FIGURE:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        default:
            Scierror(999, _("No Callback is associated with this Entity.\n"));
            return 100;
            break;
    }

}

/**sciSetCallbackMouseEvent
 * @author Djalel Abdemouche
 */
int sciSetCallbackMouseEvent(sciPointObj * pthis, int mevent)
{
    switch (sciGetEntityType (pthis))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_ARC:
            pARC_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_SEGS:
            pSEGS_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_FEC:
            pFEC_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_GRAYPLOT:
            pGRAYPLOT_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_POLYLINE:
            pPOLYLINE_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_RECTANGLE:
            pRECTANGLE_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_TEXT:
            pTEXT_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_SURFACE:
            pSURFACE_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_AXES:
            pAXES_FEATURE(pthis)->callbackevent = mevent;
            break;
        case SCI_LEGEND:
        case SCI_FIGURE:
        case SCI_AGREG:
        case SCI_UIMENU:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        default:
            Scierror (999, _("No Callback is associated with this Entity.\n"));
            return 100;
            break;
    }
    return 100;
}



/**sciDelCallback
 * returns the length of the scilab callback code
 */
int
sciDelCallback (sciPointObj * pthis)
{
    switch (sciGetEntityType (pthis))
    {
        case SCI_SUBWIN:
            pSUBWIN_FEATURE (pthis)->callbacklen = 0;
            pSUBWIN_FEATURE (pthis)->callbackevent = 100;
            FREE(pSUBWIN_FEATURE (pthis)->callback);
            pSUBWIN_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_ARC:
            pARC_FEATURE (pthis)->callbacklen = 0;
            pARC_FEATURE (pthis)->callbackevent = 100;
            FREE(pARC_FEATURE (pthis)->callback);
            pARC_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_RECTANGLE:
            pRECTANGLE_FEATURE (pthis)->callbacklen = 0;
            pRECTANGLE_FEATURE (pthis)->callbackevent = 100;
            FREE(pRECTANGLE_FEATURE (pthis)->callback);
            pRECTANGLE_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_SEGS:
            pSEGS_FEATURE (pthis)->callbacklen = 0;
            pSEGS_FEATURE (pthis)->callbackevent = 100;
            FREE(pSEGS_FEATURE (pthis)->callback);
            pSEGS_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_FEC:
            pFEC_FEATURE (pthis)->callbacklen = 0;
            pFEC_FEATURE (pthis)->callbackevent = 100;
            FREE(pFEC_FEATURE (pthis)->callback);
            pFEC_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_GRAYPLOT:
            pGRAYPLOT_FEATURE (pthis)->callbacklen = 0;
            pGRAYPLOT_FEATURE (pthis)->callbackevent = 100;
            FREE(pGRAYPLOT_FEATURE (pthis)->callback);
            pGRAYPLOT_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_POLYLINE:
            pPOLYLINE_FEATURE (pthis)->callbacklen = 0;
            pPOLYLINE_FEATURE (pthis)->callbackevent = 100;
            FREE(pPOLYLINE_FEATURE (pthis)->callback);
            pPOLYLINE_FEATURE (pthis)->callback = NULL;
            break;
        case SCI_UIMENU:
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_FIGURE:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        default:
            Scierror(999, _("No Callback is associated with this Entity.\n"));
            return -1;
            break;
    }
    return 0;
}


/************************************ End of callback Functions ************************************/


/*----------------------------------------------------------------------------------------*/
/* Objmove                                                                                         */
/* move a handle in the graphic window                                                             */
/*----------------------------------------------------------------------------------------*/

static int moveObj(char* pobjUID, double displacement[], int displacementSize)
{
    int i = 0;
    double x = displacement[0];
    double y = displacement[1];
    double z = (displacementSize == 3 ? displacement[2] : 0.0);

    int iNum = 0;
    int *piNum = &iNum;
    double* pdblData = NULL;
    double* pdblDataY = NULL;
    double* pdblDataZ = NULL;
    char* pstType = NULL;

    double* dataX = NULL;
    double* dataY = NULL;
    double* dataZ = NULL;

    int iNumX = 0;
    int iNumY = 0;
    int iNumZ = 0;
    int *piNumX = &iNumX;
    int *piNumY = &iNumY;
    int *piNumZ = &iNumZ;

    int iChildrenCount = 0;
    int *piChildrenCount = &iChildrenCount;
    char **pstChildrenUID = NULL;

    // Get type
    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_string, (void **)&pstType);

    // If a Figure, neither the object nor its children are moved.
    if ((strcmp(pstType, __GO_FIGURE__) == 0))
    {
        Scierror(999, _("This object can not be moved.\n"));
        return -1;
    }

    // Iterate on children.
    getGraphicObjectProperty(pobjUID, __GO_CHILDREN_COUNT__, jni_int, (void**)&piChildrenCount);

    if (iChildrenCount != 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_CHILDREN__, jni_string_vector, (void **) &pstChildrenUID);
        for (i = 0 ; i < iChildrenCount ; ++i)
        {
            moveObj(pstChildrenUID[i], displacement, displacementSize);
        }
    }

    // Arc.
    if (strcmp(pstType, __GO_ARC__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, pdblData, jni_double_vector, 3);

        return 0;
    }
    // Fac3d.
    else if (strcmp(pstType, __GO_FAC3D__) == 0)
    {
        int iNumVPG = 0;
        int* piNumVPG = &iNumVPG;

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**)&piNumVPG);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_GONS__, jni_int, (void**)&piNum);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

        for (i = 0; i < iNumVPG * iNum; i++)
        {
            dataX[i] += x;
            dataY[i] += y;
            dataZ[i] += z;
        }

        /* Model data has been updated by direct pointer access, trigger update within the renderer. */
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, iNumVPG * iNum);

        return 0;
    }
    // Fec.
    else if (strcmp(pstType, __GO_FEC__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**)&piNum);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void **)&pdblData);

        for (i = 0; i < iNum; i++)
        {
            pdblData[3 * i] += x;
            pdblData[3 * i + 1] += y;
            pdblData[3 * i + 2] += z;
        }

        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COORDINATES__, pdblData, jni_double_vector, 3 * iNum);

        return 0;
    }
    // Grayplot.
    else if (strcmp(pstType, __GO_GRAYPLOT__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        for (i = 0; i < iNumX; i++)
        {
            dataX[i] += x;
        }

        for (i = 0; i < iNumY; i++)
        {
            dataY[i] += y;
        }

        if (displacementSize == 3)
        {
            double zShift = 0.0;
            double* pdZshift = &zShift;

            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_int, (void**)&pdZshift);
            zShift += z;
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, &zShift, jni_double, 1);
        }
        else
        {
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNumY);
        }

        return 0;
    }
    // Matplot.
    else if (strcmp(pstType, __GO_MATPLOT__) == 0)
    {
        double zShift = 0.0;
        double* pdZShift = &zShift;
        //double* bounds = NULL;
        int type;
        int* itype = &type;

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_MATPLOT_TYPE__, jni_int, (void**)&itype);

        /* Only type 1 Matplot objects can be moved. */
        if (type == 1)
        {
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);

            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

            for (i = 0; i < iNumX; i++)
            {
                dataX[i] += x;
            }

            for (i = 0; i < iNumY; i++)
            {
                dataY[i] += y;
            }

            /* Bounds must be updated. */
            //getGraphicObjectProperty(pobjUID, __GO_MATPLOT_BOUNDS__, jni_double_vector, (void **)&bounds);

            //bounds[0] += x;
            //bounds[1] += x;
            //bounds[2] += y;
            //bounds[3] += y;

            if (displacementSize == 3)
            {
                getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_int, (void**)&pdZShift);
                zShift += z;
                setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, &zShift, jni_double, 1);
            }
            else
            {
                setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNumY);
            }

            return 0;
        }
    }
    // Polyline.
    else if (strcmp(pstType, __GO_POLYLINE__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, jni_int, (void**)&piNum);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        for (i = 0; i < iNum; i++)
        {
            dataX[i] += x;
            dataY[i] += y;
        }

        if (displacementSize == 3)
        {
            int zCoordinatesSet = 1;

            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

            for (i = 0; i < iNum; i++)
            {
                dataZ[i] += z;
            }

            /* The z coordinates flag must be set explicitely for now. */
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_int, 1);

            /* Model data has been updated by direct pointer access, trigger update within the renderer. */
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, iNum);
        }
        else
        {
            /* Trigger update, see above. */
            setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNum);
        }

        return 0;
    }
    // Plot3d.
    else if (strcmp(pstType, __GO_PLOT3D__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**)&piNumZ);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);

        for (i = 0; i < iNumX; i++)
        {
            dataX[i] += x;
        }

        for (i = 0; i < iNumY; i++)
        {
            dataY[i] += y;
        }

        for (i = 0; i < iNumZ; i++)
        {
            dataZ[i] += z;
        }

        /*
         * Perform only one set call to trigger geometry data update within the renderer module.
         * Model data has been updated above by direct pointer access.
         */
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, iNumZ);
        return 0;
    }
    // Champ.
    else if (strcmp(pstType, __GO_CHAMP__) == 0)
    {
        int *champDimensions = NULL;

        getGraphicObjectProperty(pobjUID, __GO_CHAMP_DIMENSIONS__, jni_int_vector, (void **)&champDimensions);

        getGraphicObjectProperty(pobjUID, __GO_BASE_X__, jni_double_vector, (void **)&pdblData);
        getGraphicObjectProperty(pobjUID, __GO_BASE_Y__, jni_double_vector, (void **)&pdblDataY);

        for (i = 0; i < champDimensions[0]; i++)
        {
            pdblData[i] += x;
        }

        for (i = 0; i < champDimensions[1]; i++)
        {
            pdblDataY[i] += y;
        }

        setGraphicObjectProperty(pobjUID, __GO_BASE_X__, pdblData, jni_double_vector, champDimensions[0]);
        setGraphicObjectProperty(pobjUID, __GO_BASE_Y__, pdblDataY, jni_double_vector, champDimensions[1]);

        if (displacementSize == 3)
        {
            getGraphicObjectProperty(pobjUID, __GO_BASE_Z__, jni_double_vector, (void **)&pdblDataZ);

            for (i = 0; i < champDimensions[0]*champDimensions[1]; i++)
            {
                pdblDataZ[i] += z;
            }

            setGraphicObjectProperty(pobjUID, __GO_BASE_Z__, pdblDataZ, jni_double_vector, champDimensions[0]*champDimensions[1]);
        }
        return 0;
    }
    // Label.
    else if (strcmp(pstType, __GO_LABEL__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_POSITION__, pdblData, jni_double_vector, 3);

        return 0;
    }
    // Rectangle.
    else if (strcmp(pstType, __GO_RECTANGLE__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, pdblData, jni_double_vector, 3);

        return 0;
    }
    // Segs.
    else if (strcmp(pstType, __GO_SEGS__) == 0)
    {
        double* pdblDirection = NULL;

        getGraphicObjectProperty(pobjUID, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNum);
        getGraphicObjectProperty(pobjUID, __GO_BASE__, jni_double_vector, (void **)&pdblData);
        getGraphicObjectProperty(pobjUID, __GO_DIRECTION__, jni_double_vector, (void **)&pdblDirection);

        for (i = 0; i < iNum; i++)
        {
            pdblData[3 * i] += x;
            pdblData[3 * i + 1] += y;
            pdblData[3 * i + 2] += z;
        }

        for (i = 0; i < iNum; i++)
        {
            pdblDirection[3 * i] += x;
            pdblDirection[3 * i + 1] += y;
            pdblDirection[3 * i + 2] += z;
        }

        setGraphicObjectProperty(pobjUID, __GO_BASE__, pdblData, jni_double_vector, 3 * iNum);
        setGraphicObjectProperty(pobjUID, __GO_DIRECTION__, pdblDirection, jni_double_vector, 3 * iNum);

        return 0;
    }
    // Text.
    else if (strcmp(pstType, __GO_TEXT__) == 0)
    {
        getGraphicObjectProperty(pobjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblData);
        pdblData[0] += x;
        pdblData[1] += y;
        pdblData[2] += z;
        setGraphicObjectProperty(pobjUID, __GO_POSITION__, pdblData, jni_double_vector, 3);

        return 0;
    }

    if ((strcmp(pstType, __GO_AXES__) == 0) || (strcmp(pstType, __GO_COMPOUND__) == 0))
    {
        // Children already moved: Done.
        return 0;
    }

    // Default error.
    Scierror(999, _("This object can not be moved.\n"));
    return -1;
    /*
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNumX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNumY);

        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        // X
        for (i = 0 ; i < iNumX ; ++i)
        {
            dataX[i] += x;
        }
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, dataX, jni_double_vector, iNumX);

        // Y
        for (i = 0 ; i < iNumY ; ++i)
        {
            dataY[i] += y;
        }
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, iNumY);

        // Z if needed
        if (displacementSize == 3)
        {
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_Z__, jni_int, (void**)&piNumZ);
            getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);
            for (i = 0 ; i < iNumZ ; ++i)
            {
                dataZ[i] += z;
            }
        }

        return 0;
    */

#if 0
    switch (sciGetEntityType (pobj))
    {
        case SCI_SUBWIN:
        {
            sciSons * psonstmp = sciGetSons(pobj);
            while ((psonstmp != NULL) && (psonstmp->pointobj != NULL))
            {
                moveObj(psonstmp->pointobj, displacement, displacementSize);
                psonstmp = psonstmp->pnext;
            }
        }
        break;
        case SCI_ARC:
            pARC_FEATURE(pobj)->x += x;
            pARC_FEATURE(pobj)->y += y;
            if (displacementSize == 3) pARC_FEATURE(pobj)->z += z;
            break;
        case SCI_RECTANGLE:
            pRECTANGLE_FEATURE(pobj)->x += x;
            pRECTANGLE_FEATURE(pobj)->y += y;
            if (displacementSize == 3) pRECTANGLE_FEATURE(pobj)->z += z;
            break;
        case SCI_AGREG:
        {
            sciSons * psonstmp = sciGetSons(pobj);
            while ((psonstmp != NULL) && (psonstmp->pointobj != NULL))
            {
                moveObj(psonstmp->pointobj, displacement, displacementSize);
                psonstmp = psonstmp->pnext;
            }
        }
        break;
        case SCI_TEXT:
            pTEXT_FEATURE(pobj)->x += x;
            pTEXT_FEATURE(pobj)->y += y;
            if (displacementSize == 3) pTEXT_FEATURE(pobj)->z += z;
            break;
        case SCI_SEGS:
            n = pSEGS_FEATURE(pobj)->Nbr1;
            for (i = 0; i < n; i++)
            {
                pSEGS_FEATURE(pobj)->vx[i] += x;
                pSEGS_FEATURE(pobj)->vy[i] += y;
            }
            if (displacementSize == 3)
            {
                if  (pSEGS_FEATURE(pobj)->vz == (double *)NULL)
                {
                    if ((pSEGS_FEATURE(pobj)->vz = MALLOC (n * sizeof (double))) == NULL)
                    {
                        return -1;
                    }
                    for (i = 0; i < n; i++)
                    {
                        pSEGS_FEATURE(pobj)->vz[i] = z;
                    }
                }
                else
                    for (i = 0; i < n; i++)
                    {
                        pSEGS_FEATURE(pobj)->vz[i] += z;
                    }
            }
            break;
        case SCI_POLYLINE:
            n = pPOLYLINE_FEATURE(pobj)->n1;
            for (i = 0; i < n; i++)
            {
                pPOLYLINE_FEATURE(pobj)->pvx[i] += x;
                pPOLYLINE_FEATURE(pobj)->pvy[i] += y;
            }
            if (displacementSize == 3)
            {
                if  (pPOLYLINE_FEATURE(pobj)->pvz == (double *)NULL)
                {
                    if ((pPOLYLINE_FEATURE(pobj)->pvz = MALLOC (n * sizeof (double))) == NULL)
                    {
                        return -1;
                    }
                    for (i = 0; i < n; i++)
                    {
                        pPOLYLINE_FEATURE(pobj)->pvz[i] = z;
                    }
                }
                else
                {
                    for (i = 0; i < n; i++)
                    {
                        pPOLYLINE_FEATURE(pobj)->pvz[i] += z;
                    }
                }
            }
            break;
        case SCI_FEC:
            for (i = 0; i < pFEC_FEATURE(pobj)->Nnode; i++)
            {
                pFEC_FEATURE(pobj)->pvecx[i] += x;
                pFEC_FEATURE(pobj)->pvecy[i] += y;
            }
            break;
        case SCI_GRAYPLOT:
            if (pGRAYPLOT_FEATURE(pobj)->type == 2)
            {
                /* Matplot 1, it is not possible to move a Matplot*/
                pGRAYPLOT_FEATURE(pobj)->pvecx[0] += x;
                pGRAYPLOT_FEATURE(pobj)->pvecx[2] += x;
                pGRAYPLOT_FEATURE(pobj)->pvecx[1] += y;
                pGRAYPLOT_FEATURE(pobj)->pvecx[3] += y;
            }
            else if (pGRAYPLOT_FEATURE(pobj)->type == 0)
            {
                /* Grayplot */
                for (i = 0; i < pGRAYPLOT_FEATURE(pobj)->nx; i++)
                {
                    pGRAYPLOT_FEATURE(pobj)->pvecx[i] += x;
                }
                for (i = 0; i < pGRAYPLOT_FEATURE(pobj)->ny; i++)
                {
                    pGRAYPLOT_FEATURE(pobj)->pvecy[i] += y;
                }
            }
            else
            {
                Scierror(999, _("This object can not be moved.\n"));
                return -1;
            }

            break;
        case SCI_SURFACE:
            switch (pSURFACE_FEATURE (pobj)->typeof3d)
            {
                case SCI_FAC3D:
                    n = pSURFACE_FEATURE (pobj)->dimzx * pSURFACE_FEATURE (pobj)->dimzy;
                    for (i = 0; i < n; i++)
                    {
                        pSURFACE_FEATURE(pobj)->pvecx[i] += x;
                        pSURFACE_FEATURE(pobj)->pvecy[i] += y;
                    }
                    if (displacementSize == 3)
                    {
                        if  (pSURFACE_FEATURE(pobj)->pvecz == (double *)NULL)
                        {
                            if ((pSURFACE_FEATURE(pobj)->pvecz = MALLOC (n * sizeof (double))) == NULL) return -1;
                            for (i = 0; i < n; i++)
                                pSURFACE_FEATURE(pobj)->pvecz[i] = z;
                        }
                        else
                            for (i = 0; i < n; i++)
                                pSURFACE_FEATURE(pobj)->pvecz[i] += z;
                    }
                    break;
                case SCI_PLOT3D:
                    for (i = 0; i < pSURFACE_FEATURE (pobj)->dimzx; i++)
                        pSURFACE_FEATURE(pobj)->pvecx[i] += x;
                    for (i = 0; i < pSURFACE_FEATURE (pobj)->dimzy; i++)
                        pGRAYPLOT_FEATURE(pobj)->pvecy[i] += y;
                    if (displacementSize == 3)
                    {
                        n = pSURFACE_FEATURE (pobj)->dimzx * pSURFACE_FEATURE (pobj)->dimzy;
                        if  (pSURFACE_FEATURE(pobj)->pvecz == (double *)NULL)
                        {
                            if ((pSURFACE_FEATURE(pobj)->pvecz = MALLOC (n * sizeof (double))) == NULL) return -1;
                            for (i = 0; i < n; i++)
                                pSURFACE_FEATURE(pobj)->pvecz[i] = z;
                        }
                        else
                            for (i = 0; i < n; i++)
                                pSURFACE_FEATURE(pobj)->pvecz[i] += z;
                    }
                    break;
                case SCI_CONTOUR:
                case SCI_PARAM3D:
                case SCI_PARAM3D1: /* Nothing to be done */
                    break;
            }
            break;
        case SCI_LABEL:
        {
            double pos[3];
            sciGetTextPos(pobj, pos) ;
            sciSetTextPos(pobj, pos[0] + x, pos[1] + y, pos[2] + z) ;
            pLABEL_FEATURE(pobj)->auto_position = FALSE;
            break;
        }
        case SCI_FIGURE:
        case SCI_AXES:
        case SCI_LEGEND:
        case SCI_UIMENU:
        default:
            Scierror(999, _("This object can not be moved.\n"));
            return -1;
            break;
    }

    /* update the object */
    forceMove(pobjUID, x, y, z);
#endif
    return 0;
}

int Objmove (char *pobjUID, double d[], int m, BOOL opt)
{
    int status = moveObj(pobjUID, d, m);
#if 0
    if (status < 0)
    {
        return status;
    }

    if (opt)
    {
        /* should be sci draw single obj */
        sciDrawSingleObj(pobj);
    }
    else
    {
        sciDrawObj(sciGetParentFigure(pobj));
    }
#endif
    return status;
}

/*---------------------------------------------------------------------------------*/
void interactiveZoom(sciPointObj * pObj)
{
    if (sciGetEntityType(pObj) == SCI_FIGURE)
    {
        interactiveJavaZoom(pObj);
    }
    else if (sciGetEntityType(pObj) == SCI_SUBWIN)
    {
        interactiveJavaSubwinZoom(pObj);
    }
}
/*---------------------------------------------------------------------------------*/
void interactiveRotation(sciPointObj * pFigure)
{
    interactiveJavaRotation(pFigure);
}
/*---------------------------------------------------------------------------------*/
void interactiveSubwinRotation(sciPointObj * pSubwin)
{
    /* get coordinates of first mouse click */
    interactiveJavaSubwinRotation(pSubwin);
}
/*---------------------------------------------------------------------------------*/
void showWindow(char * pFigureUID)
{
    // @TODO: Call SwingView to raise window.
}

