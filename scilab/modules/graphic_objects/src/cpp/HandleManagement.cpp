/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <stdlib.h>

#include "ScilabView.hxx"

extern "C"
{
#include "HandleManagement.h"
}

long getHandle(char const* UID)
{
    return ScilabView::getObjectHandle(UID);
}

char const* getObjectFromHandle(long handle)
{
    return ScilabView::getObjectFromHandle(handle);
}

void * sciGetParent(void *pobj)
{
    std::cerr << "[ERROR] Should not call sciGetParent" << std::endl;
    abort();
    return NULL;

}

void * sciGetSons(void *pobj)
{
    std::cerr << "[ERROR] Should not call sciGetSons" << std::endl;
    abort();
    return NULL;
}

void *sciGetCurrentSubWin(void)
{
    std::cerr << "[ERROR] Should not call sciGetCurrentSubWin" << std::endl;
    abort();
    return NULL;
}

void *sciGetCurrentObj(void)
{
    std::cerr << "[ERROR] Should not call sciGetCurrentObj" << std::endl;
    abort();
    return NULL;
}

void sciSetCurrentObj(void *pobj)
{
    std::cerr << "[ERROR] Should not call sciSetCurrentObj" << std::endl;
    abort();
}
