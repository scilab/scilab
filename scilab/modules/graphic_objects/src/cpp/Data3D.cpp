/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Data3D.hxx"

extern "C" {
#include "BOOL.h"

#include "graphicObjectProperties.h"
}

Data3D::Data3D(void)
{

}

Data3D::~Data3D(void)
{

}

int Data3D::getPropertyFromName(int propertyName)
{
    return UNKNOWN_DATA_PROPERTY;
}

int Data3D::setDataProperty(int property, void const* value, int numElements)
{
    return 0;
}

void Data3D::getDataProperty(int property, void **_pvData)
{
    if (property == UNKNOWN_DATA_PROPERTY)
    {
        *_pvData = NULL;
    }
    else
    {
        *_pvData = NULL;
    }
}

