/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Fabrice Leray
* Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
* Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

/*------------------------------------------------------------------------*/
/* file: sci_xset.cpp                                                      */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/
#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "configvariable.hxx"
#include "configgraphicvariable.hxx"
#include "overload.hxx"
#include "internal.hxx"
#include "string.hxx"

extern "C"
{
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"

#include "createGraphicObject.h"
#include "BuildObjects.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"
#include "AxesModel.h"
#include "getGraphicObjectProperty.h"
#include "deleteGraphicObject.h"
#include "configvariable_interface.h"
#include "sciprint.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_xset(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    wchar_t* pwcsWhat = NULL;
    std::list<types::Double*> lpDblInputs;
    int iSubwinUID = 0;

    if (in.size() == 0)
    {
        return Overload::call(L"%_xset", in, _iRetCount, out);
    }

    if (in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "xset", 1, 6);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "xset", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xset", 1);
        return types::Function::Error;
    }

    types::String* pStr = in[0]->getAs<types::String>();

    if (pStr->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xset", 1);
        return types::Function::Error;
    }

    pwcsWhat = pStr->get(0);

    if (ConfigGraphicVariable::bPropertyFound(pwcsWhat) == false)
    {
        char* pstWhat = wide_string_to_UTF8(pwcsWhat);
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), "xset", pstWhat);
        FREE(pstWhat);
        return types::Function::Error;
    }

    // Only in case of "fpf" and "auto clear", the second argument is a string
    // Only "default" case have one input argument
    if (ConfigGraphicVariable::getPropertyValue(pwcsWhat) != 15 && // fpf
            ConfigGraphicVariable::getPropertyValue(pwcsWhat) != 2  && // auto clear
            ConfigGraphicVariable::getPropertyValue(pwcsWhat) != 10)   // default
    {
        for (unsigned int i = 1 ; i < in.size() ; i++)
        {
            if (in[i]->isDouble() == false)
            {
                Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), "xset", i + 1);
            }
        }
    }

    switch (ConfigGraphicVariable::getPropertyValue(pwcsWhat))
    {
        case 15 : // fpf
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            if (in[1]->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            types::String* pStrValue = in[1]->getAs<types::String>();
            if (pStrValue->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            ConfigGraphicVariable::setFPF(pStrValue->get(0));
        }
        break;
        case 2 : // auto clear
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            if (in[1]->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            types::String* pStrValue = in[1]->getAs<types::String>();
            if (pStrValue->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int bAutoClear = 0;
            if (wcscmp(pStrValue->get(0), L"on") == 0)
            {
                bAutoClear = 1;
            }

            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTO_CLEAR__, &bAutoClear, jni_bool, 1);
        }
        break;
        case 5 : // clipping
        {
            int clipState = 2;
            double dvalues[4];
            if (in.size() == 2)
            {
                types::Double* pDblArg = in[1]->getAs<types::Double>();
                if (pDblArg->getSize() != 4)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A %d-element vector expected.\n"), "xset", 2, 4);
                    return types::Function::Error;
                }

                memcpy(dvalues, pDblArg->get(), 4 * sizeof(double));
            }
            else if (in.size() != 5)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "xset", 2, 5);
                return types::Function::Error;
            }
            else
            {
                for (int i = 0; i < 4 ; i++)
                {
                    dvalues[i] = in[i + 1]->getAs<types::Double>()->get(0);
                }
            }

            iSubwinUID = getOrCreateDefaultSubwin();
            setGraphicObjectProperty(iSubwinUID, __GO_CLIP_BOX__, dvalues, jni_double_vector, 4);
            setGraphicObjectProperty(iSubwinUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);
        }
        break;
        case 8 : // colormap
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            types::Double* pDblArg = in[1]->getAs<types::Double>();
            getOrCreateDefaultSubwin();
            setGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP__, pDblArg->get(), jni_double_vector, pDblArg->getSize());
        }
        break;
        case 21 : // mark size
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int markSize = (int)in[1]->getAs<types::Double>()->get(0);
            int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */

            iSubwinUID = getOrCreateDefaultSubwin();
            setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
        }
        break;
        case 20 : // mark
        {
            if (in.size() != 3)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 3);
                return types::Function::Error;
            }

            int markStyle = (int) in[1]->getAs<types::Double>()->get(0);
            int markSize = (int) in[2]->getAs<types::Double>()->get(0);
            int markSizeUnit = 1; /* force switch to tabulated mode : old syntax / 0 : point, 1 : tabulated */
            int markMode = 1;

            iSubwinUID = getOrCreateDefaultSubwin();
            setGraphicObjectProperty(iSubwinUID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1); /* force switch to tabulated mode : old syntax */
            setGraphicObjectProperty(iSubwinUID, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
            setGraphicObjectProperty(iSubwinUID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
        }
        break;
        case 13 : // font size
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }
            double fontSize = in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, &fontSize, jni_double, 1);
        }
        break;
        case 10 : // default
        {
            if (in.size() != 1)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 1);
                return types::Function::Error;
            }

            unsigned short* defcolors = ConfigGraphicVariable::getDefaultColormap();

            int piFigurePosition[2] = {200, 200};
            int piFigureSize[2]     = {500, 500};
            int piAxesSize[2]       = {498, 366};
            int piViewPort[2]       = {0, 0};
            int piEmptyMatrix[4]    = {1, 0, 0, 0};

            // init variables
            int iZero   = 0;
            BOOL bTrue  = TRUE;
            BOOL bFalse = FALSE;
            int m       = NUMCOLORS_SCI;
            int i       = 0;
            int iCopy   = 3;
            int defaultBackground = -2;

            // reset format
            ConfigGraphicVariable::setFPF(L"");

            double* pdblColorMap = new double[m * 3];
            if (pdblColorMap == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "xset");
                return types::Function::Error;
            }

            // Create figure if it not exist.
            int iFigureUID = getCurrentFigure();
            if (iFigureUID == 0)
            {
                iFigureUID = createNewFigureWithAxes();
                setCurrentFigure(iFigureUID);
                delete[] pdblColorMap;
                return types::Function::OK;
            }

            // Create new axes and set it in current figure
            int iSubWinUID = getCurrentSubWin();
            if (iSubWinUID != 0)
            {
                int iChildrenCount  = 0;
                int* childrencount  = &iChildrenCount;
                int* childrenUID    = 0;
                int iHidden         = 0;
                int *piHidden       = &iHidden;

                getGraphicObjectProperty(iFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&childrencount);
                getGraphicObjectProperty(iFigureUID, __GO_CHILDREN__, jni_string_vector, (void **)&childrenUID);

                for (i = 0; i < childrencount[0]; ++i)
                {
                    getGraphicObjectProperty(childrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
                    if (iHidden == 0)
                    {
                        deleteGraphicObject(childrenUID[i]);
                    }
                }
            }

            cloneAxesModel(iFigureUID);

            // Set default figure properties
            setGraphicObjectProperty(iFigureUID, __GO_POSITION__, piFigurePosition, jni_int_vector, 2);
            setGraphicObjectProperty(iFigureUID, __GO_SIZE__, piFigureSize, jni_int_vector, 2);
            setGraphicObjectProperty(iFigureUID, __GO_AXES_SIZE__, piAxesSize, jni_int_vector, 2);
            setGraphicObjectProperty(iFigureUID, __GO_AUTORESIZE__, &bTrue, jni_bool, 1);
            setGraphicObjectProperty(iFigureUID, __GO_VIEWPORT__, piViewPort, jni_int_vector, 2);
            setGraphicObjectProperty(iFigureUID, __GO_NAME__, _("Figure n°%d"), jni_string, 1);
            setGraphicObjectProperty(iFigureUID, __GO_INFO_MESSAGE__, "", jni_string, 1);
            setGraphicObjectProperty(iFigureUID, __GO_PIXEL_DRAWING_MODE__, &iCopy, jni_int, 1);
            setGraphicObjectProperty(iFigureUID, __GO_ANTIALIASING__, &iZero, jni_int, 1);
            setGraphicObjectProperty(iFigureUID, __GO_IMMEDIATE_DRAWING__, &bTrue, jni_bool, 1);
            setGraphicObjectProperty(iFigureUID, __GO_BACKGROUND__, &defaultBackground, jni_int, 1);
            setGraphicObjectProperty(iFigureUID, __GO_VISIBLE__, &bTrue, jni_bool, 1);
            setGraphicObjectProperty(iFigureUID, __GO_ROTATION_TYPE__, &iZero, jni_int, 1);
            setGraphicObjectProperty(iFigureUID, __GO_EVENTHANDLER__, "", jni_string, 1);
            setGraphicObjectProperty(iFigureUID, __GO_EVENTHANDLER_ENABLE__, &bFalse, jni_bool, 1);
            setGraphicObjectProperty(iFigureUID, __GO_USER_DATA__, piEmptyMatrix, jni_int_vector, 4);
            setGraphicObjectProperty(iFigureUID, __GO_RESIZEFCN__, "", jni_string, 1);
            setGraphicObjectProperty(iFigureUID, __GO_TAG__, "", jni_string, 1);

            for (i = 0; i < m; i++)
            {
                pdblColorMap[i]         = (double)(defcolors[3 * i] / 255.0);
                pdblColorMap[i + m]     = (double)(defcolors[3 * i + 1] / 255.0);
                pdblColorMap[i + 2 * m] = (double)(defcolors[3 * i + 2] / 255.0);
            }

            setGraphicObjectProperty(iFigureUID, __GO_COLORMAP__, pdblColorMap, jni_double_vector, 3 * m);
            setGraphicObjectProperty(iFigureUID, __GO_PARENT__, "", jni_string, 1);
            delete[] pdblColorMap;
        }
        break;
        case 6 : // clipgrf
        {
            int clipState = 1;
            /* special treatement for xset("cligrf") */
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, &clipState, jni_int, 1);
        }
        break;
        case 4 : // clipoff
        {
            int clipState = 0;
            /* special treatement for xset("clipoff") */
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, &clipState, jni_int, 1);
        }
        break;
        case 16 : // hidden3d
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int hiddenColor = (int) in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);
        }
        break;
        case 12 : // font
        {
            if (in.size() != 3)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 3);
                return types::Function::Error;
            }

            int fontStyle = (int) in[1]->getAs<types::Double>()->get(0);
            double fontSize = in[2]->getAs<types::Double>()->get(0);

            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, &fontSize, jni_double, 1);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_STYLE__, &fontStyle, jni_int, 1);
        }
        break;
        case 11 : // window
        case 30 : // figure
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            // Find if window already exists, if not create a new one
            int iID = (int)in[1]->getAs<types::Double>()->get(0);
            int iFigureUID = getFigureFromIndex(iID);
            int iAxesUID = 0;
            int* piAxesUID = &iAxesUID;

            if (iFigureUID == 0)
            {
                iFigureUID = createNewFigureWithAxes();
                setGraphicObjectProperty(iFigureUID, __GO_ID__, &iID, jni_int, 1);
            }

            setCurrentFigure(iFigureUID);
            getGraphicObjectProperty(iFigureUID, __GO_SELECTED_CHILD__, jni_int, (void**)&piAxesUID);
            setCurrentSubWin(iAxesUID);
        }
        break;
        case 14 : // foreground
        case 7 : // color
        case 23 : // pattern
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int iColor = (int) in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_COLOR__, &iColor, jni_int, 1);
        }
        break;
        case 3 : // background
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int iColor = (int) in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_BACKGROUND__, &iColor, jni_int, 1);
        }
        break;
        case 25 : // thickness
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            sciSetLineWidth(getOrCreateDefaultSubwin(), (int)in[1]->getAs<types::Double>()->get(0));
        }
        break;
        case 19 : // line style
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int lineStyle = (int) in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
        }
        break;
        case 9 : // dashes
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int lineStyle = (int) in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, &lineStyle, jni_int, 1);
        }
        break;
        case 33 : // wresize
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int iAutoResizeMode = (int)in[1]->getAs<types::Double>()->get(0);
            setGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTORESIZE__, &iAutoResizeMode, jni_bool, 1);
        }
        break;
        case 32 : // wpos
        {
            int figurePosition[2];
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            getOrCreateDefaultSubwin();

            figurePosition[0] = (int)in[1]->getAs<types::Double>()->get(0);
            figurePosition[1] = (int)in[1]->getAs<types::Double>()->get(1);
            setGraphicObjectProperty(getCurrentFigure(), __GO_POSITION__, figurePosition, jni_int_vector, 2);
        }
        break;
        case 31 : // wpdim
        case 28 : // wdim
        {
            int figureSize[2] = {0, 0};
            if (in.size() != 2 && in.size() != 3)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "xset", 2, 3);
                return types::Function::Error;
            }

            figureSize[0] = (int)in[1]->getAs<types::Double>()->get(0);
            if (in.size() == 3)
            {
                figureSize[1] = (int)in[2]->getAs<types::Double>()->get(0);
            }

            /* Xwindows limits dimensions to 2^16 */
            if ((figureSize[0] > 65535) || (figureSize[1] > 65535))
            {
                figureSize[0] = std::min(figureSize[0], 65535);
                figureSize[1] = std::min(figureSize[1], 65535);
                if (ConfigVariable::getWarningMode())
                {
                    sciprint(_("%s: window dimensions have been set less than 2^16.\n"), "xset");
                }
            }

            getOrCreateDefaultSubwin();
            setGraphicObjectProperty(getCurrentFigure(), __GO_SIZE__, figureSize, jni_int_vector, 2);
        }
        break;
        case 27 : // viewport
        {
            if (in.size() != 3)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 3);
                return types::Function::Error;
            }

            int viewport[4] = {0, 0, 0, 0};
            viewport[0] = in[1]->getAs<types::Double>()->get(0);
            viewport[1] = in[2]->getAs<types::Double>()->get(0);

            getOrCreateDefaultSubwin();
            setGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, viewport, jni_int_vector, 2);
        }
        break;
        case 18 : // line mode
        {
            if (in.size() != 2)
            {
                Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "xset", 2);
                return types::Function::Error;
            }

            int iSubwinUID = getOrCreateDefaultSubwin();
            int iZero = 0;
            int iOne = 1;

            if (in[1]->getAs<types::Double>()->get(0) == 0)
            {
                setGraphicObjectProperty(iSubwinUID, __GO_LINE_MODE__, &iZero, jni_bool, 1);
            }
            else
            {
                setGraphicObjectProperty(iSubwinUID, __GO_LINE_MODE__, &iOne, jni_bool, 1);
            }
        }
        break;
        default :
        {
            char* pstWhat = wide_string_to_UTF8(pwcsWhat);
            Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), "xset", pstWhat);
            FREE(pstWhat);
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
