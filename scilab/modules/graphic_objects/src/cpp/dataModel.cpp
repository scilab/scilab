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

#include "DataModel.hxx"

extern "C" {
#include "BOOL.h"

#include <string.h>
}

DataModel *DataModel::m_me = NULL;

BOOL DataModel::setGraphicObjectProperty(char const* _pstID, char const* _pstName, void const* _dblValue, int numElements)
{
    Data3D* dataObject = NULL;
    int property = 0;
    int returnValue = 0;

    dataObject = (*m_dataMap)[std::string(_pstID)];

    property = dataObject->getPropertyFromName(_pstName);

    returnValue = dataObject->setDataProperty(property, _dblValue, numElements);

    return (BOOL) returnValue;
}

void DataModel::getGraphicObjectProperty(char const* _pstID, char const* _pstName, void **_pvData)
{
    Data3D* dataObject = NULL;
    int property = 0;

    dataObject = (*m_dataMap)[std::string(_pstID)];

    property = dataObject->getPropertyFromName(_pstName);

    dataObject->getDataProperty(property, _pvData);
}

void DataModel::getGraphicObjectIntProperty(char const* _pstID, char const* _pstName, void **_pvData)
{
    Data3D* dataObject = NULL;
    int property = 0;

    property = (*m_dataMap)[std::string(_pstID)]->getPropertyFromName(_pstName);

    dataObject = (*m_dataMap)[std::string(_pstID)];

    property = dataObject->getPropertyFromName(_pstName);

    dataObject->getDataProperty(property, _pvData);
}

char const* DataModel::createDataObject(char const* _pstID, char const* _sType)
{
    Data3D* newObject = NULL;

    /*
     * To be implemented as the Java graphicObject class' similar function
     * (getTypeFromName) in order to avoid strcmp calls.
     */
    if (strcmp(_sType, __GO_GRAYPLOT__) == 0)
    {
        newObject = new NgonGridData();
    }
    else if (strcmp(_sType, __GO_MATPLOT__) == 0)
    {
        newObject = new NgonGridMatplotData();
    }
    else if (strcmp(_sType, __GO_FAC3D__) == 0)
    {
        newObject = new NgonGeneralData();
    }
    else if (strcmp(_sType, __GO_PLOT3D__) == 0)
    {
        newObject = new NgonGridData();
    }
    else if (strcmp(_sType, __GO_POLYLINE__) == 0)
    {
        newObject = new NgonPolylineData();
    }
    else if (strcmp(_sType, __GO_FEC__) == 0)
    {
        newObject = new TriangleMeshFecData();
    }
    else
    {
        return NULL;
    }

    (*m_dataMap)[std::string(_pstID)] = newObject;

    return _pstID;
}

void DataModel::deleteDataObject(char const* _pstID)
{
    m_dataMap->erase(std::string(_pstID));
}

