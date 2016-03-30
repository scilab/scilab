/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - Scilab Enterprises - Bruno JOFRET
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
/* file: sci_xget.cpp                                                       */
/* desc : interface for xget routine                                      */
/*------------------------------------------------------------------------*/

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "configgraphicvariable.hxx"
#include "overload.hxx"
#include "string.hxx"

extern "C"
{
#include <string.h>
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "returnProperty.h"
#include "HandleManagement.h"
#include "CurrentSubwin.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"
#include "sci_malloc.h"
#include "getHandleProperty.h"
#include "CurrentFigure.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_xget(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double dValue = 0;
    wchar_t* pwcsWhat = NULL;
    void* pvApiCtx = NULL;

    if (in.size() == 0)
    {
        return Overload::call(L"%_xget", in, _iRetCount, out);
    }

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "xget", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "xget", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xget", 1);
        return types::Function::Error;
    }

    types::String* pStr = in[0]->getAs<types::String>();

    if (pStr->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "xget", 1);
        return types::Function::Error;
    }

    pwcsWhat = pStr->get(0);

    if (ConfigGraphicVariable::bPropertyFound(pwcsWhat) == false)
    {
        char* pstWhat = wide_string_to_UTF8(pwcsWhat);
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), "xget", pstWhat);
        FREE(pstWhat);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), "xget", 2);
            return types::Function::Error;
        }

        types::Double* pDbl = in[1]->getAs<types::Double>();

        //CheckScalar
        if (pDbl->isScalar())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "xget", 2);
            return types::Function::Error;
        }

        dValue = pDbl->get(0);
    }

    switch (ConfigGraphicVariable::getPropertyValue(pwcsWhat))
    {
        case 15 : // fpf
        {
            out.push_back(new types::String(ConfigGraphicVariable::getFPF().c_str()));
        }
        break;
        case 2 : // auto clear
        {
            int iAutoClear = 0;
            int* piAutoClear = &iAutoClear;
            getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTO_CLEAR__, jni_bool, (void **)&piAutoClear);
            if (iAutoClear == 1)
            {
                out.push_back(new types::String(L"on"));
            }
            else
            {
                out.push_back(new types::String(L"off"));
            }
        }
        break;
        case 8 : // colormap
        {
            int iObjUID = 0;
            // Force figure creation if none exists.
            getOrCreateDefaultSubwin();
            iObjUID = getCurrentFigure();

            out.push_back((types::InternalType*)get_color_map_property(pvApiCtx, iObjUID));
        }
        break;
        case 20 : // mark
        {
            int iObjUID = getOrCreateDefaultSubwin();
            int iMarkStyle = 0;
            int* piMarkStyle = &iMarkStyle;
            int iMarkSize = 0;
            int* piMarkSize = &iMarkSize;
            types::Double* pDbl = new types::Double(1, 2);

            getGraphicObjectProperty(iObjUID, __GO_MARK_STYLE__, jni_int, (void**)&piMarkStyle);
            getGraphicObjectProperty(iObjUID, __GO_MARK_SIZE__, jni_int, (void**)&piMarkSize);

            pDbl->set(0, (double)iMarkStyle);
            pDbl->set(1, (double)iMarkSize);
            out.push_back(pDbl);
        }
        break;
        case 21 : // mark size
        {
            int iObjUID = getOrCreateDefaultSubwin();
            out.push_back((types::InternalType*)get_mark_size_property(pvApiCtx, iObjUID));
        }
        break;
        case 19 : // line style
        {
            out.push_back((types::InternalType*)get_line_style_property(pvApiCtx, getOrCreateDefaultSubwin()));
        }
        break;
        case 5 : // clipping
        {
            types::Double* pDblClipBox = new types::Double(1, 4);
            double *clipBox = NULL;
            int iObjUID = getOrCreateDefaultSubwin();
            getGraphicObjectProperty(iObjUID, __GO_CLIP_BOX__, jni_double_vector, (void **)&clipBox);
            memcpy(pDblClipBox->get(), clipBox, 4 * sizeof(double));
            out.push_back(pDblClipBox);
        }
        break;
        case 12 : // font
        {
            int iObjUID = getOrCreateDefaultSubwin();
            double dblFontSize = 0;
            double* pdblFontSize = &dblFontSize;
            int iFontStyle = 0;
            int* piFontStyle = &iFontStyle;
            types::Double* pDbl = new types::Double(1, 2);

            getGraphicObjectProperty(iObjUID, __GO_FONT_SIZE__, jni_double, (void **)&pdblFontSize);
            getGraphicObjectProperty(iObjUID, __GO_FONT_STYLE__, jni_int, (void**)&piFontStyle);

            pDbl->set(0, (double)iFontStyle);
            pDbl->set(1, dblFontSize);
            out.push_back(pDbl);
        }
        break;
        case 13 : // font size
        {
            double dblFontSize = 0;
            double* pdblFontSize = &dblFontSize;
            getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, jni_double, (void **)&pdblFontSize);

            out.push_back(new types::Double(dblFontSize));
        }
        break;
        case 9 : // dashes
        {
            int iLineStyle = 0;
            int* piLineStyle = &iLineStyle;

            getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, jni_int, (void**)&piLineStyle);

            out.push_back(new types::Double((double)iLineStyle));
        }
        break;
        case 16 : // hidden3d
        {
            out.push_back((types::InternalType*)get_hidden_color_property(pvApiCtx, getOrCreateDefaultSubwin()));
        }
        break;
        case 30 : // window
        case 11 : // figure
        {
            int iFigureId = 0;
            int* piFigureId = &iFigureId;

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_ID__, jni_int, (void**)&piFigureId);

            out.push_back(new types::Double((double)iFigureId));
        }
        break;
        case 25 : // thickness
        {
            out.push_back((types::InternalType*)get_thickness_property(pvApiCtx, getOrCreateDefaultSubwin()));
        }
        break;
        case 28 : // wdim
        case 31 : // wpdim
        {
            int *piFigureSize = NULL;
            types::Double* pDblFigureSize = new types::Double(1, 2);

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_SIZE__, jni_int_vector, (void **) &piFigureSize);

            pDblFigureSize->set(0, (double) piFigureSize[0]);
            pDblFigureSize->set(1, (double) piFigureSize[1]);
            out.push_back(pDblFigureSize);
        }
        break;
        case 32 : // wpos
        {
            int *piFigurePosition = NULL;
            types::Double* pDblFigurePos = new types::Double(1, 2);

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_POSITION__, jni_int_vector, (void **) &piFigurePosition);

            pDblFigurePos->set(0, (double) piFigurePosition[0]);
            pDblFigurePos->set(1, (double) piFigurePosition[1]);
            out.push_back(pDblFigurePos);
        }
        break;
        case 27 : // viewport
        {
            int *piViewport = NULL;
            types::Double* pDblViewport = new types::Double(1, 2);

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, jni_int_vector, (void **)&piViewport);

            pDblViewport->set(0, (double) piViewport[0]);
            pDblViewport->set(1, (double) piViewport[1]);
            out.push_back(pDblViewport);
        }
        break;
        case 3 : // background
        {
            out.push_back((types::InternalType*)get_background_property(pvApiCtx, getOrCreateDefaultSubwin()));
        }
        break;
        case 7 : // color
        case 14 : // foreground
        case 23 : // pattern
        {
            out.push_back((types::InternalType*)get_foreground_property(pvApiCtx, getOrCreateDefaultSubwin()));
        }
        break;
        case 17 : // lastpattern
        {
            int iNumColors = 0;
            int* piNumColors = &iNumColors;

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP_SIZE__, jni_int, (void**)&piNumColors);

            out.push_back(new types::Double((double)iNumColors));
        }
        break;
        case 18 : // line mode
        {
            int iLineMode = 0;
            int* lineMode = &iLineMode;

            getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_MODE__, jni_bool, (void **)&lineMode);

            out.push_back(new types::Double((double)iLineMode));
        }
        break;
        case 29 : // white
        {
            int iNumColors = 0;
            int* piNumColors = &iNumColors;

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP_SIZE__, jni_int, (void**)&piNumColors);

            /* White is lqst colormap index + 2 */
            out.push_back(new types::Double((double)(iNumColors + 2)));
        }
        break;
        case 33 : // wresize
        {
            // autoresize property
            int iAutoResize = 0;
            int* piAutoResize =  &iAutoResize;

            getOrCreateDefaultSubwin();
            getGraphicObjectProperty(getCurrentFigure(), __GO_AUTORESIZE__, jni_bool, (void **)&piAutoResize);

            out.push_back(new types::Double((double)iAutoResize));
        }
        break;
        case 6 : // clipgrf
        {
            /* clip_state : 0 = off, 1 = on */
            int iClipState = 0;
            int* piClipState = &iClipState;

            getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, jni_int, (void**)&piClipState);

            out.push_back(new types::Double((double)iClipState));
        }
        break;
        case 4 : // clipoff
        {
            int iClipState = 0;
            int* piClipState = &iClipState;

            getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, jni_int, (void**)&piClipState);

            /* clip_state : 0 = off, 1 = on */
            if (iClipState == 0)
            {
                out.push_back(new types::Double(1));
            }
            else
            {
                out.push_back(new types::Double(0));
            }
        }
        break;
        default :
        {
            char* pstWhat = wide_string_to_UTF8(pwcsWhat);
            Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), "xget", pstWhat);
            FREE(pstWhat);
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
