/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "createGraphicObject.h"
#include "getScilabJavaVM.h"
}

#include "CallGraphicController.hxx"
#include "GraphicObjectBuilder.hxx"
#include "DataController.hxx"

using namespace org_scilab_modules_graphic_objects;

char *createGraphicObject(int _iType)
{
    return CallGraphicController::askGraphicObject(getScilabJavaVM(), _iType);
}

char *cloneGraphicObject(char const* _sId)
{
    return CallGraphicController::cloneGraphicObject(getScilabJavaVM(), _sId);
}

char const* createDataObject(char const* _sId, int _iType)
{
    return DataController::createDataObject(_sId, _iType);
}

void buildFigureMenuBar(char const* _sFigureId)
{
    CallGraphicController::buildFigureMenuBar(getScilabJavaVM(), _sFigureId);
}

char * constructRectangles(char * pparentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline)
{
    try
    {
	return GraphicObjectBuilder::constructRectangles(getScilabJavaVM(), pparentsubwinUID, x, y, height, width, foreground, background, isfilled, isline);
    }
    catch (std::exception & e)
    {
	printf("exception: %s\n", e.what());
	return NULL;
    }
}
