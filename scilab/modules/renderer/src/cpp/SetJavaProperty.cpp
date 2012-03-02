/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Interface functions between between SetProperty functions and the C++/Java part of module
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetJavaProperty.h"
#include "GiwsException.hxx"


extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "getScilabJavaVM.h"
}

#include "CallRenderer.hxx"

using namespace org_scilab_modules_renderer;

/*---------------------------------------------------------------------------------*/
BOOL sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height)
{
    abort();
    return FALSE;
}

/*---------------------------------------------------------------------------------*/
void sciJavaAddTextToDraw(sciPointObj * pText, sciPointObj * parentSubwin)
{

}

/*---------------------------------------------------------------------------------*/
void sciJavaRemoveTextToDraw(sciPointObj * pText, sciPointObj * parentSubwin)
{
}

/*---------------------------------------------------------------------------------*/
void sciSetJavaAutoResizeMode(sciPointObj * pFigure, BOOL resizeMode)
{
    //getFigureDrawer(pFigure)->setAutoResizeMode(resizeMode == TRUE);
}

/*---------------------------------------------------------------------------------*/
void sciSetJavaViewport(sciPointObj * pFigure, const int viewport[4])
{
    //getFigureDrawer(pFigure)->setViewport(viewport);
}

/*---------------------------------------------------------------------------------*/
void sciSetJavaBackground(sciPointObj * pFigure, int backColor)
{
    //getFigureDrawer(pFigure)->setBackgroundColor(backColor);
}

/*---------------------------------------------------------------------------------*/
void sciSetJavaUseSingleBuffer(sciPointObj * pFigure, BOOL useSingleBuffer)
{
    //getFigureDrawer(pFigure)->setUseSingleBuffer(useSingleBuffer == TRUE);
}

/*---------------------------------------------------------------------------------*/
void sciSetJavaTitle(sciPointObj * pFigure, const char *title)
{
    try
    {
        //getFigureDrawer(pFigure)->setTitle(title);
    } catch(const GiwsException::JniException & e)
    {
        Scierror(999,
                 "A native error occurred during the creation of the window.\nDescription: %s\nException Name: %s\nPlease report a bug on http://bugzilla.scilab.org/ with your example and the result of getdebuginfo().\n",
                 e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
    } catch(const std::exception & e)
    {
        sciprint("Error: %s\n", e.what());
        Scierror(999,
                 "An error occurred during the creation of the window.\nPlease report a bug on http://bugzilla.scilab.org/ with your example and the result of getdebuginfo().\n");
    }
}

/*---------------------------------------------------------------------------------*/
void sciJavaUpdateSubwinScale(char * pSubwinUID)
{
    CallRenderer::updateSubwinScale(getScilabJavaVM(), pSubwinUID);
}

/*---------------------------------------------------------------------------------*/
void sciJavaUpdateTextBoundingBox(char * pTextUID)
{
    CallRenderer::updateTextBounds(getScilabJavaVM(), pTextUID);
}

/*---------------------------------------------------------------------------------*/
