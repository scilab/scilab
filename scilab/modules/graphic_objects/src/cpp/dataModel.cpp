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

BOOL DataModel::setGraphicObjectProperty(char *_pstID, char* _pstName, void* _dblValue, int numElements)
{
    Data3D* dataObject;
    int property;
    int returnValue;

    dataObject = (*m_dataMap)[std::string(_pstID)];

    property = dataObject->getPropertyFromName(_pstName);

    returnValue = dataObject->setDataProperty(property, _dblValue, numElements);

    return returnValue;
}

void* DataModel::getGraphicObjectProperty(char *_pstID, char* _pstName)
{
    Data3D* dataObject;
    int property;

    dataObject = (*m_dataMap)[std::string(_pstID)];

    property = dataObject->getPropertyFromName(_pstName);

    return dataObject->getDataProperty(property);
}

int* DataModel::getGraphicObjectIntProperty(char *_pstID, char* _pstName)
{
    Data3D* dataObject;
    int property;

    property = (*m_dataMap)[std::string(_pstID)]->getPropertyFromName(_pstName);

    dataObject = (*m_dataMap)[std::string(_pstID)];

    property = dataObject->getPropertyFromName(_pstName);

    return (int*) dataObject->getDataProperty(property);
}

char* DataModel::createDataObject(char* _pstID, char* _sType)
{
    Data3D* newObject;

    /*
     * To be implemented as the Java graphicObject class' similar function
     * (getTypeFromName) in order to avoid strcmp calls.
     */
    if (strcmp(_sType, __GO_MATPLOT__) == 0)
    {
        newObject = new NgonGridData();
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

/* To be implemented */
void DataModel::deleteDataObject(char* _pstID)
{

}

