/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "ScilabView.hxx"

extern "C"
{
#include "FigureList.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#else
#include <string.h>
#endif
#include "FigureList.h"
}

int sciGetNbFigure(void)
{
    return ScilabView::getNbFigure();
}

void sciGetFiguresId(int ids[])
{
    return ScilabView::getFiguresId(ids);
}

int getValidDefaultFigureId()
{
    return ScilabView::getValidDefaultFigureId();
}

BOOL sciIsExistingFigure(int id)
{
    return (ScilabView::existsFigureId(id) == true ? TRUE : FALSE);
}

BOOL sciHasFigures()
{
    return (ScilabView::isEmptyFigureList() == true ? FALSE : TRUE);
}

int getFigureFromIndex(int figNum)
{
    return ScilabView::getFigureFromIndex(figNum);
}

void registerToController()
{
    ScilabView::registerToController();
}
void unregisterToController()
{
    ScilabView::unregisterToController();
}

int search_path(char* _pstPath)
{
    return ScilabView::search_path(_pstPath);
}

char* get_path(int uid)
{
    std::string path = ScilabView::get_path(uid);
    return strdup(path.c_str());
}

