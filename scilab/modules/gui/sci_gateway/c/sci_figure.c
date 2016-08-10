/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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

#include <stdio.h>
#include "gw_gui.h"
#include "sci_malloc.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "createGraphicObject.h"
#include "BuildObjects.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "FigureList.h"
#include "FigureModel.h"
#include "HandleManagement.h"
#include "SetHashTable.h"
#include "os_string.h"
#include "sciprint.h"
#include "addColor.h"

/*--------------------------------------------------------------------------*/
int setDefaultProperties(int _iFig, BOOL bDefaultAxes);
int getStackArgumentAsBoolean(void* _pvCtx, int* _piAddr);
void initBar(int iFig, BOOL menubar, BOOL toolbar, BOOL infobar);
/*--------------------------------------------------------------------------*/
int sci_figure(char * fname, void* pvApiCtx)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iFig = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    int iId = 0;
    int iPos = 0;
    int i = 0;
    int iAxes = 0;
    int iPropertyOffset = 0;
    BOOL bDoCreation = TRUE;
    BOOL bVisible = TRUE; // Create a visible figure by default
    BOOL bDockable = TRUE; // Create a dockable figure by default
    BOOL bDefaultAxes = TRUE; // Create an Axes by default
    double* figureSize = NULL;
    double* axesSize = NULL;
    double* position = NULL;
    double val[4];
    BOOL bMenuBar = TRUE;
    BOOL bToolBar = TRUE;
    BOOL bInfoBar = TRUE;
    BOOL bResize = TRUE;
    int iMenubarType = 1; // Create a 'figure' menubar by default
    int iToolbarType = 1; // Create a 'figure' toolbar by default
    double dblId = 0;
    BOOL status = FALSE;

    //figure(num) -> scf(num)
    //figure() -> scf()

    //figure(x, "...", ...)

    // figure()
    if (iRhs == 0) // Auto ID
    {
        iId = getValidDefaultFigureId();
        iFig = createNewFigureWithAxes();
        setGraphicObjectProperty(iFig, __GO_ID__, &iId, jni_int,  1);
        iAxes = setDefaultProperties(iFig, TRUE);
        initBar(iFig, bMenuBar, bToolBar, bInfoBar);
        createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    if (iRhs == 1)
    {
        //figure(x);
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarMatrixType(pvApiCtx, piAddr) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddr, &dblId))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iId = (int)(dblId + 0.5); //avoid 1.999 -> 1

        //get current fig from id
        iFig = getFigureFromIndex(iId);
        if (iFig == 0) // Figure does not exists, create a new one
        {
            iFig = createNewFigureWithAxes();
            setGraphicObjectProperty(iFig, __GO_ID__, &iId, jni_int,  1);
            iAxes = setDefaultProperties(iFig, TRUE);
        }

        initBar(iFig, bMenuBar, bToolBar, bInfoBar);
        createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

    // Prepare property analysis
    if (iRhs % 2 == 0)
    {
        //get highest value of winsid to create the new windows @ + 1
        iId = getValidDefaultFigureId();
        iPos = 0;
    }
    else
    {
        iPos = 1;
        //figure(x, ...);
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isVarMatrixType(pvApiCtx, piAddr) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), fname, 1);
            return 0;
        }

        if (getScalarDouble(pvApiCtx, piAddr, &dblId))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        iId = (int)(dblId + 0.5); //avoid 1.999 -> 1
        //get current fig from id
        iFig = getFigureFromIndex(iId);
        if (iFig != 0) // Figure already exists
        {
            bDoCreation = FALSE;
        }
    }

    if (bDoCreation)
    {
        int* piAddrProp = NULL;
        char* pstProName = NULL;
        int* piAddrData = NULL;
        for (i = iPos + 1 ; i <= iRhs ; i += 2)
        {
            //get property name
            sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrProp);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
                return 1;
            }

            if (getAllocatedSingleString(pvApiCtx, piAddrProp, &pstProName))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i);
                return 1;
            }

            if (stricmp(pstProName, "dockable") != 0 &&
                    stricmp(pstProName, "toolbar") != 0 &&
                    stricmp(pstProName, "menubar") != 0 &&
                    stricmp(pstProName, "default_axes") != 0 &&
                    stricmp(pstProName, "visible") != 0 &&
                    stricmp(pstProName, "figure_size") != 0 &&
                    stricmp(pstProName, "axes_size") != 0 &&
                    stricmp(pstProName, "position") != 0 &&
                    stricmp(pstProName, "menubar_visible") != 0 &&
                    stricmp(pstProName, "toolbar_visible") != 0 &&
                    stricmp(pstProName, "resize") != 0 &&
                    stricmp(pstProName, "infobar_visible") != 0)
            {
                freeAllocatedSingleString(pstProName);
                continue;
            }

            //get address of value on stack
            sciErr = getVarAddressFromPosition(pvApiCtx, i + 1, &piAddrData);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
                freeAllocatedSingleString(pstProName);
                return 1;
            }

            //check property value to compatibility
            if (stricmp(pstProName, "dockable") == 0)
            {
                bDockable = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bDockable == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "dockable", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "toolbar") == 0)
            {
                char* pstVal = NULL;
                if (isStringType(pvApiCtx, piAddrData) == FALSE || isScalar(pvApiCtx, piAddrData) == FALSE)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddrData, &pstVal))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i + 1);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }

                if (stricmp(pstVal, "none") == 0)
                {
                    iToolbarType = 0;
                }
                else if (stricmp(pstVal, "figure") == 0)
                {
                    iToolbarType = 1;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "toolbar", "none", "figure");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstVal);
                    return 1;
                }

                freeAllocatedSingleString(pstVal);
            }
            else if (stricmp(pstProName, "menubar") == 0)
            {
                char* pstVal = NULL;
                if (isStringType(pvApiCtx, piAddrData) == FALSE || isScalar(pvApiCtx, piAddrData) == FALSE)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i + 1);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }

                if (getAllocatedSingleString(pvApiCtx, piAddrData, &pstVal))
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i + 1);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }

                if (stricmp(pstVal, "none") == 0)
                {
                    iMenubarType = 0;
                }
                else if (stricmp(pstVal, "figure") == 0)
                {
                    iMenubarType = 1;
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "menubar", "none", "figure");
                    freeAllocatedSingleString(pstProName);
                    freeAllocatedSingleString(pstVal);
                    return 1;
                }

                freeAllocatedSingleString(pstVal);
            }
            else if (stricmp(pstProName, "default_axes") == 0)
            {
                bDefaultAxes = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bDefaultAxes == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "default_axes", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "visible") == 0)
            {
                bVisible = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bVisible == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "visible", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "figure_size") == 0)
            {
                int iRows = 0;
                int iCols = 0;
                if (isDoubleType(pvApiCtx, piAddrData) == FALSE)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A double vector expected.\n"), fname, i + 1);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }

                getMatrixOfDouble(pvApiCtx, piAddrData, &iRows, &iCols, &figureSize);
                if (iRows * iCols != 2)
                {
                    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "figure_size", 2);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "axes_size") == 0)
            {
                int iRows = 0;
                int iCols = 0;
                if (isDoubleType(pvApiCtx, piAddrData) == FALSE)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A double vector expected.\n"), fname, i + 1);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }

                getMatrixOfDouble(pvApiCtx, piAddrData, &iRows, &iCols, &axesSize);
                if (iRows * iCols != 2)
                {
                    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "axes_size", 2);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "position") == 0)
            {
                int iRows = 0;
                int iCols = 0;
                double* pdbl = NULL;
                if (isDoubleType(pvApiCtx, piAddrData))
                {
                    getMatrixOfDouble(pvApiCtx, piAddrData, &iRows, &iCols, &pdbl);
                    if (iRows * iCols != 4)
                    {
                        Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "position", 4);
                        freeAllocatedSingleString(pstProName);
                        return 1;
                    }

                    position = pdbl;
                    axesSize = (pdbl + 2);
                }
                else if (isStringType(pvApiCtx, piAddrData) && isScalar(pvApiCtx, piAddrData))
                {
                    char* pstVal = NULL;
                    int iVal = 0;

                    if (getAllocatedSingleString(pvApiCtx, piAddrData, &pstVal))
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i + 1);
                        freeAllocatedSingleString(pstProName);
                        return 1;
                    }

                    iVal = sscanf(pstVal, "%lf|%lf|%lf|%lf", &val[0], &val[1], &val[2], &val[3]);
                    freeAllocatedSingleString(pstVal);
                    if (iVal != 4)
                    {
                        Scierror(999, _("Wrong value for '%s' property: string or 1 x %d real row vector expected.\n"), "position", 4);
                        freeAllocatedSingleString(pstProName);
                        return 1;
                    }

                    position = val;
                    axesSize = (val + 2);
                }
                else
                {
                    Scierror(999, _("Wrong value for '%s' property: string or 1 x %d real row vector expected.\n"), "position", 4);
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "resize") == 0)
            {
                bResize = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bResize == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "resize", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "menubar_visible") == 0)
            {
                bMenuBar = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bMenuBar == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "menubar_visible", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "toolbar_visible") == 0)
            {
                bToolBar = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bToolBar == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "toolbar_visible", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            else if (stricmp(pstProName, "infobar_visible") == 0)
            {
                bInfoBar = getStackArgumentAsBoolean(pvApiCtx, piAddrData);
                if (bInfoBar == -1)
                {
                    Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected."), "infobar_visible", "on", "off");
                    freeAllocatedSingleString(pstProName);
                    return 1;
                }
            }
            freeAllocatedSingleString(pstProName);
        }

        iFig = createFigure(bDockable, iMenubarType, iToolbarType, bDefaultAxes, bVisible);
        setGraphicObjectProperty(iFig, __GO_ID__, &iId, jni_int, 1);
        iAxes = setDefaultProperties(iFig, bDefaultAxes);
    }

    //set(iFig, iPos, iPos + 1)
    for (i = iPos + 1 ; i <= iRhs ; i += 2)
    {
        int isMatrixOfString = 0;
        int* piAddrProp = NULL;
        char* pstProName = NULL;
        int* piAddrData = NULL;
        int iRows = 0;
        int iCols = 0;
        void* _pvData = NULL;
        int iType = 0;

        //get property name
        sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddrProp);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 1;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddrProp, &pstProName))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, i);
            return 1;
        }

        if (bDoCreation && (
                    stricmp(pstProName, "dockable") == 0 ||
                    stricmp(pstProName, "toolbar") == 0 ||
                    stricmp(pstProName, "menubar") == 0 ||
                    stricmp(pstProName, "default_axes") == 0 ||
                    stricmp(pstProName, "visible") == 0 ||
                    stricmp(pstProName, "figure_size") == 0 ||
                    stricmp(pstProName, "axes_size") == 0 ||
                    stricmp(pstProName, "position") == 0 ||
                    stricmp(pstProName, "resize") == 0 ||
                    stricmp(pstProName, "menubar_visible") == 0 ||
                    stricmp(pstProName, "toolbar_visible") == 0 ||
                    stricmp(pstProName, "infobar_visible") == 0))
        {
            // Already set creating new figure
            // but let the set_ function fail if figure already exists
            freeAllocatedSingleString(pstProName);
            continue;
        }

        //get address of value on stack
        sciErr = getVarAddressFromPosition(pvApiCtx, i + 1, &piAddrData);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
            freeAllocatedSingleString(pstProName);
            return 1;
        }

        getVarType(pvApiCtx, piAddrData, &iType);

        if ((strcmp(pstProName, "user_data") == 0) || (stricmp(pstProName, "userdata") == 0))
        {
            /* in this case set_user_data_property
             * directly uses the  third position in the stack
             * to get the variable which is to be set in
             * the user_data property (any data type is allowed) S. Steer */
            _pvData = (void*)piAddrData;         /*position in the stack */
            iRows = -1;   /*unused */
            iCols = -1;   /*unused */
            iType = -1;
        }
        else
        {
            switch (iType)
            {
                case sci_matrix :
                    getMatrixOfDouble(pvApiCtx, piAddrData, &iRows, &iCols, (double**)&_pvData);
                    break;
                case sci_boolean :
                    getMatrixOfBoolean(pvApiCtx, piAddrData, &iRows, &iCols, (int**)&_pvData);
                    break;
                case sci_handles :
                    getMatrixOfHandle(pvApiCtx, piAddrData, &iRows, &iCols, (long long**)&_pvData);
                    break;
                case sci_strings :
                    if (   strcmp(pstProName, "tics_labels") != 0 && strcmp(pstProName, "auto_ticks") != 0 &&
                            strcmp(pstProName, "axes_visible") != 0 && strcmp(pstProName, "axes_reverse") != 0 &&
                            strcmp(pstProName, "text") != 0 && stricmp(pstProName, "string") != 0 &&
                            stricmp(pstProName, "tooltipstring") != 0) /* Added for uicontrols */
                    {
                        if (getAllocatedSingleString(pvApiCtx, piAddrData, (char**)&_pvData))
                        {
                            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 3);
                            freeAllocatedSingleString(pstProName);
                            return 1;
                        }
                        iRows = (int)strlen((char*)_pvData);
                        iCols = 1;
                    }
                    else
                    {
                        isMatrixOfString = 1;
                        if (getAllocatedMatrixOfString(pvApiCtx, piAddrData, &iRows, &iCols, (char***)&_pvData))
                        {
                            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                            freeAllocatedSingleString(pstProName);
                            return 1;
                        }
                    }
                    break;
                case sci_list :
                    iCols = 1;
                    getListItemNumber(pvApiCtx, piAddrData, &iRows);
                    _pvData = (void*)piAddrData;         /* In this case l3 is the list position in stack */
                    break;
                default :
                    _pvData = (void*)piAddrData;         /* In this case l3 is the list position in stack */
                    break;
            }
        }

        callSetProperty(pvApiCtx, iFig, _pvData, iType, iRows, iCols, pstProName);

        // If backgroundcolor is set :
        // * add it to colormap => performed by callSetProperty
        // * set background to index => performed by callSetProperty
        // * copy value into axes background property
        if (stricmp(pstProName, "backgroundcolor") == 0 && iAxes > 0)
        {
            int iBackground = 0;
            int *piBackground = &iBackground;

            getGraphicObjectProperty(iFig, __GO_BACKGROUND__, jni_int, (void **)&piBackground);
            setGraphicObjectProperty(iAxes, __GO_BACKGROUND__, piBackground, jni_int, 1);
        }

        freeAllocatedSingleString(pstProName);
        if (iType == sci_strings)
        {
            //free allacted data
            if (isMatrixOfString == 1)
            {
                freeAllocatedMatrixOfString(iRows, iCols, (char**)_pvData);
            }
            else
            {
                freeAllocatedSingleString((char*)_pvData);
            }
        }
    }

    if (position)
    {
        int pos[2];
        pos[0] = (int)position[0];
        pos[1] = (int)position[1];
        setGraphicObjectProperty(iFig, __GO_POSITION__, pos, jni_int_vector, 2);
    }

    //axes_size
    if (axesSize)
    {
        int axes[2];
        axes[0] = (int)axesSize[0];
        axes[1] = (int)axesSize[1];
        setGraphicObjectProperty(iFig, __GO_AXES_SIZE__, axes, jni_int_vector, 2);
    }
    else //no size, use default axes_size
    {
        int* piAxesSize = NULL;
        getGraphicObjectProperty(getFigureModel(), __GO_AXES_SIZE__, jni_int_vector, (void **)&piAxesSize);
        setGraphicObjectProperty(iFig, __GO_AXES_SIZE__, piAxesSize, jni_int_vector, 2);
        releaseGraphicObjectProperty(__GO_AXES_SIZE__, piAxesSize, jni_int_vector, 2);
    }

    initBar(iFig, bMenuBar, bToolBar, bInfoBar);

    if (axesSize == NULL && figureSize) //figure_size
    {
        int figure[2];
        figure[0] = (int)figureSize[0];
        figure[1] = (int)figureSize[1];
        setGraphicObjectProperty(iFig, __GO_SIZE__, figure, jni_int_vector, 2);
    }


    setGraphicObjectProperty(iFig, __GO_RESIZE__, (void*)&bResize, jni_bool, 1);

    //return new created fig
    createScalarHandle(pvApiCtx, iRhs + 1, getHandle(iFig));
    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
int getStackArgumentAsBoolean(void* _pvCtx, int* _piAddr)
{
    if (isScalar(_pvCtx, _piAddr))
    {
        if (isDoubleType(_pvCtx, _piAddr))
        {
            double dbl = 0;
            getScalarDouble(_pvCtx, _piAddr, &dbl);
            return ((int)dbl == 0 ? FALSE : TRUE);
        }
        else if (isBooleanType(_pvCtx, _piAddr))
        {
            int i = 0;
            getScalarBoolean(_pvCtx, _piAddr, &i);
            return (i == 0 ? FALSE : TRUE);
        }
        else if (isStringType(_pvCtx, _piAddr))
        {
            int ret = 0;
            char* pst = NULL;
            if (getAllocatedSingleString(_pvCtx, _piAddr, &pst))
            {
                return -1;
            }

            if (stricmp(pst, "on") == 0)
            {
                ret = TRUE;
            }

            freeAllocatedSingleString(pst);

            return ret;
        }
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
int setDefaultProperties(int _iFig, BOOL _bDefaultAxes)
{
    //get figure axes
    int iAxes = -1;
    int iDrawing = 0;
    int iColorIndex = 0;
    int iFilled = 0;
    int iAxesVisible = 0;
    int* piAxesSize = NULL;
    double pdblNewColor[COLOR_COMPONENT] = {0.8, 0.8, 0.8};

    iColorIndex = addColor(_iFig, pdblNewColor);

    setGraphicObjectProperty(_iFig, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);
    if (_bDefaultAxes)
    {
        iAxes = getOrCreateDefaultSubwin();
        //set background in figure and axes to new ( or existting ) color
        setGraphicObjectProperty(iAxes, __GO_BACKGROUND__, &iColorIndex, jni_int, 1);

        //a.filled = "off"
        setGraphicObjectProperty(iAxes, __GO_FILLED__, &iFilled, jni_bool, 1);

        //a.axes_visible = "off"
        setGraphicObjectProperty(iAxes, __GO_X_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
        setGraphicObjectProperty(iAxes, __GO_Y_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
        setGraphicObjectProperty(iAxes, __GO_Z_AXIS_VISIBLE__, &iAxesVisible, jni_bool, 1);
    }

    return iAxes;
}
/*--------------------------------------------------------------------------*/
void initBar(int iFig, BOOL menubar, BOOL toolbar, BOOL infobar)
{
    BOOL notmenubar = !menubar;
    BOOL nottoolbar = !toolbar;
    BOOL notinfobar = !infobar;

    setGraphicObjectProperty(iFig, __GO_MENUBAR_VISIBLE__, (void*)&notmenubar, jni_bool, 1);
    setGraphicObjectProperty(iFig, __GO_TOOLBAR_VISIBLE__, (void*)&nottoolbar, jni_bool, 1);
    setGraphicObjectProperty(iFig, __GO_INFOBAR_VISIBLE__, (void*)&notinfobar, jni_bool, 1);

    //set menubar, infobar, toolbar visibility
    setGraphicObjectProperty(iFig, __GO_MENUBAR_VISIBLE__, (void*)&menubar, jni_bool, 1);
    setGraphicObjectProperty(iFig, __GO_TOOLBAR_VISIBLE__, (void*)&toolbar, jni_bool, 1);
    setGraphicObjectProperty(iFig, __GO_INFOBAR_VISIBLE__, (void*)&infobar, jni_bool, 1);

}
/*--------------------------------------------------------------------------*/
