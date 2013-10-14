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
#include <string.h>

#include "BOOL.h"
#include "CurrentFigure.h"
}

int getCurrentFigure(void)
{
    return ScilabView::getCurrentFigure();
}

void setCurrentFigure(int UID)
{
    ScilabView::setCurrentFigure(UID);
}

BOOL isCurrentFigure(int UID)
{
    if (UID == ScilabView::getCurrentFigure())
    {
        return TRUE;
    }
    return FALSE;
}
