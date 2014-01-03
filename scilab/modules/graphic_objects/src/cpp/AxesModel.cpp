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

#include <string.h>
#include "ScilabView.hxx"

extern "C"
{
#include "AxesModel.h"
}

int getAxesModel(void)
{
    return ScilabView::getAxesModel();
}
void setAxesModel(int UID)
{
    ScilabView::setAxesModel(UID);
}

BOOL isAxesModel(int UID)
{
    if (UID == getAxesModel())
    {
        return TRUE;
    }
    return FALSE;
}
