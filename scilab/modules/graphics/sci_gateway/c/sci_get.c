/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2006 - INRIA - Vincent Couvert
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_get.c                                                        */
/* desc : interface for sci_get routine                                   */
/*------------------------------------------------------------------------*/
#include "gw_graphics.h"
/*--------------------------------------------------------------------------*/

#include "stricmp.h"
#include "MALLOC.h"

#include "HandleManagement.h"

#include "GetHashTable.h"
#include "BuildObjects.h"
#include "localization.h"
#include "Scierror.h"

#include "HandleManagement.h"
#include "CurrentObject.h"
#include "CurrentSubwin.h"
#include "getConsoleIdentifier.h"
#include "returnProperty.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "SetPropertyStatus.h"
#include "GetScreenProperty.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "getHandleProperty.h"
/*--------------------------------------------------------------------------*/
void convertDoubleToHandle(void* _pvCtx, int _iPos);
void ScierrorGetProperty(int _iError, char* _pstProp, char* fname);
/*--------------------------------------------------------------------------*/
char *IntToStyle(int _iStyle)
{
    switch (_iStyle)
    {
        case __GO_UI_CHECKBOX__ :
            return "checkbox";
        case __GO_UI_EDIT__ :
            return "edit";
        case __GO_UI_FRAME__ :
            return "frame";
        case __GO_UI_IMAGE__ :
            return "image";
        case __GO_UI_LISTBOX__ :
            return "listbox";
        case __GO_UI_POPUPMENU__ :
            return "popupmenu";
        case __GO_UI_PUSHBUTTON__ :
            return "pushbutton";
        case __GO_UI_RADIOBUTTON__ :
            return "radiobutton";
        case __GO_UI_SLIDER__ :
            return "slider";
        case __GO_UI_TABLE__ :
            return "table";
        case __GO_UI_TEXT__ :
            return "text";
        default :
            return "????";
    }
}
/*--------------------------------------------------------------------------*/
static char *getTypeNameFromInt(int _iType)
{
    switch (_iType)
    {
        case __GO_ARC__ :
            return "Arc";
        case __GO_AXES__ :
            return "Axes";
        case __GO_AXESMODEL__ :
            return "AxesModel";
        case __GO_AXIS__ :
            return "Axis";
        case __GO_CHAMP__ :
            return "Champ";
        case __GO_COMPOUND__ :
            return "Compound";
        case __GO_FAC3D__ :
            return "Fac3d";
        case __GO_FEC__ :
            return "Fec";
        case __GO_FIGURE__ :
            return "Figure";
        case __GO_FIGUREMODEL__ :
            return "FigureModel";
        case __GO_GRAYPLOT__ :
            return "Grayplot";
        case __GO_LABEL__ :
            return "Label";
        case __GO_LEGEND__ :
            return "Legend";
        case __GO_MATPLOT__ :
            return "Matplot";
        case __GO_PLOT3D__ :
            return "Plot3d";
        case __GO_POLYLINE__ :
            return "Polyline";
        case __GO_RECTANGLE__ :
            return "Rectangle";
        case __GO_SEGS__ :
            return "Segs";
        case __GO_TEXT__ :
            return "Text";
        case __GO_UICONTROL__ :
            return "uicontrol";
        case __GO_UIMENU__ :
            return "uimenu";
        case __GO_UICONTEXTMENU__ :
            return "uicontextmenu";
        case __GO_CONSOLE__ :
            return "Console";
        case __GO_SHOWHIDDENHANDLES__ :
            return "ShowHiddenHandles";
        case __GO_WAITBAR__ :
            return "Waitbar";
        case __GO_DATATIP__:
            return "Datatip";
        case __GO_LIGHT__ :
            return "Light";
        default :
            return "????";
    }
}
/*--------------------------------------------------------------------------*/
int sciGet(void* _pvCtx, int iObjUID, char *marker)
{
    /* find the function in the hashtable relative to the property name */
    /* and call it */
    return callGetProperty(_pvCtx, iObjUID, marker) == -1;
}

/*--------------------------------------------------------------------------*/
int sci_get(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int* piAddrl1 = NULL;
    double* pdbll1 = NULL;
    int* piAddrstkAdr = NULL;
    long long* l1 = NULL;
    int* piAddrl2 = NULL;
    char* l2 = NULL;

    int m1 = 0, n1 = 0;
    long hdl = 0;

    int iObjUID = 0;

    /* Root properties */
    char **stkAdr = NULL;
    int status = SET_PROPERTY_ERROR;

    if (((checkInputArgumentType(pvApiCtx, 1, sci_mlist))) || ((checkInputArgumentType(pvApiCtx, 1, sci_tlist))))
    {
        //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
        OverLoad(1);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 0, 1);

    /*
     * The first input argument can be an ID or a marker (in this case, get returns the value of the current object */
    switch (getInputArgumentType(pvApiCtx, 1))
    {
        case sci_matrix:                    /* tclsci handle */
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            sciErr = getMatrixOfDouble(pvApiCtx, piAddrl1, &m1, &n1, &pdbll1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
                return 1;
            }

            if ((int)pdbll1[0] == 0) /* Root property */
            {
                if (nbInputArgument(pvApiCtx) == 1)
                {
                    if (sciReturnHandle(pvApiCtx, getHandle(getConsoleIdentifier())) != 0)    /* Get Console handle */
                    {
                        /* An error has occurred */
                        ReturnArguments(pvApiCtx);
                        return 0;
                    }
                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                    ReturnArguments(pvApiCtx);
                    return 0;
                }
                CheckInputArgument(pvApiCtx, 2, 2);
                if ((checkInputArgumentType(pvApiCtx, 2, sci_strings)))
                {
                    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrstkAdr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 1;
                    }

                    // Retrieve a matrix of string at position 2.
                    if (getAllocatedMatrixOfString(pvApiCtx, piAddrstkAdr, &m1, &n1, &stkAdr))
                    {
                        Scierror(202, _("%s: Wrong type for argument #%d: String matrix expected.\n"), fname, 2);
                        return 1;
                    }


                    if (m1 * n1 != 1)
                    {
                        freeAllocatedMatrixOfString(m1, n1, stkAdr);
                        Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get", 2);
                        return SET_PROPERTY_ERROR;
                    }

                    status = GetScreenProperty(pvApiCtx, stkAdr[0]);

                    if (status != SET_PROPERTY_SUCCEED) /* Return property */
                    {
                        Scierror(999, _("%s: Could not read property '%s' for root object.\n"), "get", stkAdr[0]);
                        freeAllocatedMatrixOfString(m1, n1, stkAdr);
                        return FALSE;
                    }
                    freeAllocatedMatrixOfString(m1, n1, stkAdr);
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "get", 2);
                    return FALSE;
                }
                AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                ReturnArguments(pvApiCtx);
            }
            else                    /* tclsci handle: should no more happen */
            {
                //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
                OverLoad(1);
            }
            return 0;
            break;
        case sci_handles:          /* scalar argument (hdl + string) */
            CheckInputArgument(pvApiCtx, 2, 2);
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of handle at position 1.
            sciErr = getMatrixOfHandle(pvApiCtx, piAddrl1, &m1, &n1, &l1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(202, _("%s: Wrong type for input argument #%d: Handle matrix expected.\n"), fname, 1);
                return 1;
            }

            if (m1 != 1 || n1 != 1)
            {
                //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
                OverLoad(1);
                return 0;
            }

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrl2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 2.
            if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 2);
                return 1;
            }

            hdl = (long) * l1; /* on recupere le pointeur d'objet par le handle */
            break;
        case sci_strings:          /* string argument (string) */
            CheckInputArgument(pvApiCtx, 1, 1);
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrl2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            // Retrieve a matrix of double at position 1.
            if (getAllocatedSingleString(pvApiCtx, piAddrl2, &l2))
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A string expected.\n"), fname, 1);
                return 1;
            }

            if (strcmp((l2), "default_figure") != 0 && strcmp((l2), "default_axes") != 0)
            {
                if (strcmp((l2), "current_figure") == 0 || strcmp((l2), "current_axes") == 0 || strcmp((l2), "current_entity") == 0
                        || strcmp((l2), "hdl") == 0 || strcmp((l2), "figures_id") == 0)
                {
                    hdl = 0;
                }
                else
                {
                    /* Test debug F.Leray 13.04.04 */
                    if ((strcmp((l2), "children") != 0) && (strcmp((l2), "zoom_") != 0) && (strcmp((l2), "clip_box") != 0)
                            && (strcmp((l2), "auto_") != 0))
                    {
                        getOrCreateDefaultSubwin();
                        hdl = getHandle(getCurrentObject());
                    }
                    else
                    {
                        hdl = getHandle(getCurrentSubWin());    /* on recupere le pointeur d'objet par le handle */
                    }
                }                   /* DJ.A 08/01/04 */
            }
            else
            {
                hdl = 0;
            }
            break;
        default:
            //lw = 1 + nbArgumentOnStack(pvApiCtx) - nbInputArgument(pvApiCtx);
            OverLoad(1);
            return 0;
            break;
    }
    /* (l2) est la commande, l3 l'indice sur les parametres de la commande */
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (hdl == 0)
    {
        /* No handle specified */
        if (sciGet(pvApiCtx, 0, (l2)) != 0)
        {
            /* An error has occurred */
            freeAllocatedSingleString(l2);
            ReturnArguments(pvApiCtx);
            return 0;
        }
    }
    else
    {
        iObjUID = getObjectFromHandle(hdl);
        if (iObjUID != 0)
        {
            int iType = 0;
            int* piType = &iType;
            getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void**)&piType);
            if (iType != __GO_UICONTROL__ &&
                    iType != __GO_FIGURE__ &&
                    iType != __GO_AXES__ &&
                    iType != __GO_ARC__  &&
                    iType != __GO_AXIS__ &&
                    iType != __GO_COMPOUND__ &&
                    iType != __GO_FEC__ &&
                    iType != __GO_GRAYPLOT__ &&
                    iType != __GO_MATPLOT__)
            {
                if (sciGet(pvApiCtx, iObjUID, (l2)) != 0)
                {
                    /* An error has occurred */
                    freeAllocatedSingleString(l2);
                    ReturnArguments(pvApiCtx);
                    return 0;
                }
            }
            else
            {
                char first = l2[0];
                BOOL bDataProp = FALSE;
                int ret = 0;

                //some data are in C so don't call java
                if (iType == __GO_FEC__ || iType == __GO_GRAYPLOT__ || iType == __GO_MATPLOT__)
                {
                    if (first == 'd')
                    {
                        if (stricmp(l2, "data") == 0)
                        {
                            get_data_property(pvApiCtx, iObjUID);
                            bDataProp = TRUE;
                        }
                    }
                    else if (first == 't')
                    {
                        if (stricmp(l2, "triangles") == 0)
                        {
                            get_triangles_property(pvApiCtx, iObjUID);
                            bDataProp = TRUE;
                        }
                    }
                }

                if (iType == __GO_MATPLOT__)
                {
                    if (first == 'i')
                    {
                        if (stricmp(l2, "image_type") == 0)
                        {
                            get_image_type_property(pvApiCtx, iObjUID);
                            bDataProp = TRUE;
                        }
                    }
                }

                if (bDataProp)
                {
                    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
                    ReturnArguments(pvApiCtx);
                    freeAllocatedSingleString(l2);
                    return 0;
                }


                if ((ret = getProperty(iObjUID, l2, nbInputArgument(pvApiCtx) + 1)) != 0)
                {
                    ScierrorGetProperty(ret, l2, fname);
                    freeAllocatedSingleString(l2);
                    return 0;
                }

                //global properties
                switch (first)
                {
                    case 't' :
                        if (stricmp(l2, "type") == 0)
                        {
                            int iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                            int* piAddress = istk(iAddr);

                            iType = (int)((double*)piAddress)[2];
                            if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, getTypeNameFromInt(iType)))
                            {
                                freeAllocatedSingleString(l2);
                                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                return 1;
                            }
                        }
                        break;
                    case 'c' :
                        if (stricmp(l2, "children") == 0)
                        {
                            convertDoubleToHandle(pvApiCtx, 1);
                        }
                        break;
                    case 'p' :
                        if (stricmp(l2, "parent") == 0)
                        {
                            convertDoubleToHandle(pvApiCtx, 1);
                        }
                        break;
                }

                //properties of UI_CONTROL
                if (iType == __GO_UICONTROL__)
                {
                    switch (first)
                    {
                        case 's' :
                            if (stricmp(l2, "style") == 0)
                            {
                                int iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                int* piAddress = istk(iAddr);

                                iType = (int)((double*)piAddress)[2];
                                if (createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, IntToStyle(iType)))
                                {
                                    freeAllocatedSingleString(l2);
                                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                    return 1;
                                }
                            }
                            break;
                    }
                }


                //figure
                if (iType == __GO_FIGURE__)
                {
                    switch (first)
                    {
                    }
                }

                //axes
                if (iType == __GO_AXES__)
                {
                    //special case for properties about axis x,y,z
                    if (first == 'x' || first == 'y' || first == 'z')
                    {
                        if (strcmp(l2 + 1, "_label") == 0)
                        {
                            convertDoubleToHandle(pvApiCtx, 1);
                        }
                    }

                    switch (first)
                    {
                        case 'l':
                            if (stricmp(l2, "log_flags") == 0)
                            {
                                //convert string matrix of "on"/"off" into string "n/l"

                                //init to non log view
                                char pstLog[4] = {"nnn"};

                                int i = 0;
                                int iRows = 0;
                                int iCols = 0;
                                char** pstLogs = NULL;

                                int iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                int* piAddress = istk(iAddr);

                                getAllocatedMatrixOfString(pvApiCtx, piAddress, &iRows, &iCols, &pstLogs);

                                for (i = 0 ; i < iRows * iCols ; i++)
                                {
                                    if (pstLogs[i][1] == 'n') //"on"
                                    {
                                        pstLog[i] = 'l';
                                    }
                                }

                                createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstLog);
                            }
                            break;
                        case 'd' :
                            if (stricmp(l2, "data_bounds") == 0)
                            {
                                int iAddr = 0;
                                int* piAddress = NULL;
                                char* pstView = NULL;
                                //depend of view
                                if ((ret = getProperty(iObjUID, "view", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    ScierrorGetProperty(ret, l2, fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);

                                if (getAllocatedSingleString(pvApiCtx, piAddress, &pstView))
                                {
                                    if (pstView)
                                    {
                                        freeAllocatedSingleString(pstView);
                                    }

                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                if (strcmp(pstView, "2d") == 0)
                                {
                                    //force column size to 2 instead of 3
                                    iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                    piAddress = istk(iAddr);
                                    piAddress[1] = 2;
                                    piAddress[2] = 2;
                                }
                                else //3d
                                {
                                    iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                    piAddress = istk(iAddr);
                                    piAddress[1] = 2;
                                    piAddress[2] = 3;
                                }
                            }
                            break;
                        case 's' :
                            if (stricmp(l2, "sub_ticks") == 0)
                            {
                                int iAddr = 0;
                                int* piAddress = NULL;
                                char* pstView = NULL;
                                //depend of view
                                if ((ret = getProperty(iObjUID, "view", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    ScierrorGetProperty(ret, l2, fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);

                                if (getAllocatedSingleString(pvApiCtx, piAddress, &pstView))
                                {
                                    if (pstView)
                                    {
                                        freeAllocatedSingleString(pstView);
                                    }

                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                if (strcmp(pstView, "2d") == 0)
                                {
                                    //return only 2 grid color information
                                    //force column size to 2 instead of 3
                                    iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                    piAddress = istk(iAddr);
                                    piAddress[2] = 2;
                                }
                            }
                            break;
                        case 'z' :
                            if (stricmp(l2, "zoom_box") == 0)
                            {
                                int iAddr = 0;
                                int* piAddress = NULL;
                                char* pstZoom = NULL;
                                //depend of zoom_enable
                                if ((ret = getProperty(iObjUID, "zoom_enable", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    ScierrorGetProperty(ret, l2, fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);

                                if (getAllocatedSingleString(pvApiCtx, piAddress, &pstZoom))
                                {
                                    if (pstZoom)
                                    {
                                        freeAllocatedSingleString(pstZoom);
                                    }

                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                if (strcmp(pstZoom, "off") == 0)
                                {
                                    //if zoom is disable, returns []
                                    createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1);
                                }
                            }
                            break;
                        case 'g' :
                            if (stricmp(l2, "grid") == 0)
                            {
                                int iAddr = 0;
                                int* piAddress = NULL;
                                char* pstView = NULL;
                                //depend of view
                                if ((ret = getProperty(iObjUID, "view", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    freeAllocatedSingleString(l2);
                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);

                                if (getAllocatedSingleString(pvApiCtx, piAddress, &pstView))
                                {
                                    if (pstView)
                                    {
                                        freeAllocatedSingleString(pstView);
                                    }

                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                if (strcmp(pstView, "2d") == 0)
                                {
                                    //return only 2 grid color information
                                    //force column size to 2 instead of 3
                                    iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                    piAddress = istk(iAddr);
                                    piAddress[2] = 2;
                                }
                            }
                        case 't' :
                            if (stricmp(l2, "title") == 0)
                            {
                                convertDoubleToHandle(pvApiCtx, 1);
                            }
                            break;
                    } //switch for axes
                }

                //arc
                if (iType == __GO_ARC__)
                {
                    switch (first)
                    {
                        case 'd' :
                            if (stricmp(l2, "data") == 0)
                            {
                                int iAddr = 0;
                                int* piAddress = NULL;
                                char* pstView = NULL;
                                double dblParent = 0;
                                int iParent = 0;
                                //depend of view
                                if ((ret = getProperty(iObjUID, "parent", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    ScierrorGetProperty(ret, l2, fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);
                                iParent = (int) * (double*)(piAddress + 4);

                                if ((ret = getProperty(iParent, "view", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    ScierrorGetProperty(ret, l2, fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);

                                if (getAllocatedSingleString(pvApiCtx, piAddress, &pstView))
                                {
                                    if (pstView)
                                    {
                                        freeAllocatedSingleString(pstView);
                                    }

                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                if (strcmp(pstView, "2d") == 0)
                                {
                                    //data is for 3d view
                                    //change matrix size to 1x6 and remove z information
                                    int i = 0;
                                    double* data = NULL;
                                    iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 1));
                                    piAddress = istk(iAddr);
                                    piAddress[2] = 6;
                                    data = (double*)(piAddress + 4);
                                    for (i = 2 ; i < 6 ; i++)
                                    {
                                        data[i] = data[i + 1];
                                    }
                                }
                            }
                            break;
                    }
                }


                if (iType == __GO_ARC__ ||
                        iType == __GO_AXIS__ ||
                        iType == __GO_AXES__ ||
                        iType == __GO_FEC__ ||
                        iType == __GO_GRAYPLOT__ ||
                        iType == __GO_MATPLOT__)
                {
                    switch (first)
                    {
                        case 'c' :
                            if (stricmp(l2, "clip_box") == 0)
                            {
                                int iAddr = 0;
                                int* piAddress = NULL;
                                char* pstState = NULL;
                                //depend of clip_state
                                if ((ret = getProperty(iObjUID, "clip_state", nbInputArgument(pvApiCtx) + 2)) != 0)
                                {
                                    ScierrorGetProperty(ret, l2, fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + 2));
                                piAddress = istk(iAddr);

                                if (getAllocatedSingleString(pvApiCtx, piAddress, &pstState))
                                {
                                    if (pstState)
                                    {
                                        freeAllocatedSingleString(pstState);
                                    }

                                    Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
                                    freeAllocatedSingleString(l2);
                                    return 0;
                                }

                                if (strcmp(pstState, "on") != 0)
                                {
                                    //if clip_state is disable, returns []
                                    createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1);
                                }
                            }
                            break;
                    }
                }

            }
        }
        else
        {
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            freeAllocatedSingleString(l2);
            return 0;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    freeAllocatedSingleString(l2);
    return 0;
}

void convertDoubleToHandle(void* _pvCtx, int _iPos)
{
    int iAddr = iadr(*Lstk(nbArgumentOnStack(pvApiCtx) + _iPos));
    int* piAddress = istk(iAddr);
    int iRows = 0;
    int iCols = 0;
    double* pDbl = NULL;

    getMatrixOfDouble(pvApiCtx, piAddress, &iRows, &iCols, &pDbl);

    if (iRows * iCols == 0)
    {
        createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + _iPos);
    }
    else if (iRows * iCols == 1)
    {
        long long llH = (long long)getHandle((int) * pDbl);
        createScalarHandle(pvApiCtx, nbInputArgument(pvApiCtx) + _iPos, llH);
    }
    else
    {
        int i = 0;
        long long* pllH = NULL;
        pllH = (long long*)MALLOC(sizeof(long long) * iRows * iCols);
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pllH[i] = (long long)getHandle((int)pDbl[i]);
        }

        createMatrixOfHandle(pvApiCtx, nbInputArgument(pvApiCtx) + _iPos, iRows, iCols, pllH);
        FREE(pllH);
    }
}

void ScierrorGetProperty(int _iError, char* _pstProp, char* fname)
{
    switch (_iError)
    {
        case 1:
            Scierror(999, _("%s: An exception occurred in graphic controller.\n"), fname);
            break;
        case 2:
            Scierror(999, _("Unknown property: %s.\n"), _pstProp);
            break;
        case 3:
            Scierror(999, _("%s: The handle is not or no more valid.\n"), fname);
            break;
    }
}
/*--------------------------------------------------------------------------*/
