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

#include <string.h>
#include "ScilabView.hxx"

extern "C"
{
#include "FigureModel.h"
}

int getFigureModel(void)
{
    return ScilabView::getFigureModel();
}

void setFigureModel(int UID)
{
    ScilabView::setFigureModel(UID);
}

BOOL isFigureModel(int UID)
{
    if (UID == getFigureModel())
    {
        return TRUE;
    }
    return FALSE;
}
