/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "ScilabView.hxx"

extern "C"
{
#include "FigureList.h"
#include "os_string.h"
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

