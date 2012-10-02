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

#include "NgonData.hxx"
#include "DataProperties.hxx"

extern "C" {
#include "BOOL.h"

#include <string.h>

#include "graphicObjectProperties.h"

}

NgonData::NgonData(void)
{
    numGons = 0;
    numVerticesPerGon = 0;
}

/* To be implemented */
NgonData::~NgonData(void)
{

}

int NgonData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
    case __GO_DATA_MODEL_NUM_GONS__ :
        return NUM_GONS;
    case __GO_DATA_MODEL_NUM_VERTICES_PER_GON__ :
        return NUM_VERTICES_PER_GON;
    default :
        return Data3D::getPropertyFromName(propertyName);
    }

}

int NgonData::setDataProperty(int property, void const* value, int numElements)
{

    if (property == NUM_GONS)
    {
        setNumGons(*((int*) value));
    }
    else if (property == NUM_VERTICES_PER_GON)
    {
        setNumVerticesPerGon(*((int*) value));
    }
    else
    {
        return Data3D::setDataProperty(property, value, numElements);
    }

    return 1;
}

void NgonData::getDataProperty(int property, void **_pvData)
{
    if (property == NUM_GONS)
    {
        ((int *) *_pvData)[0] = getNumGons();
    }
    else if (property == NUM_VERTICES_PER_GON)
    {
        ((int *) *_pvData)[0] = getNumVerticesPerGon();
    }
    else
    {
        Data3D::getDataProperty(property, _pvData);
    }

}

void NgonData::setNumGons(int numGons)
{
    this->numGons = numGons;
}

int NgonData::getNumGons(void)
{
    return numGons;
}

void NgonData::setNumVerticesPerGon(int numVerticesPerGon)
{
    this->numVerticesPerGon = numVerticesPerGon;
}

int NgonData::getNumVerticesPerGon(void)
{
    return numVerticesPerGon;
}

