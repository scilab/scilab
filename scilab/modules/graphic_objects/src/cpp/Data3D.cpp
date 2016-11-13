/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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

