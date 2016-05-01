/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "DataModel.hxx"

extern "C" {
#include "BOOL.h"

#include <string.h>
}

DataModel *DataModel::m_me = NULL;

BOOL DataModel::setGraphicObjectProperty(int iUID, int _iName, void const* _dblValue, int numElements)
{
    Data3D* dataObject = NULL;
    int property = 0;
    int returnValue = 0;

    dataObject = m_dataMap[iUID];
    if (dataObject == NULL)
    {
        return FALSE;
    }

    property = dataObject->getPropertyFromName(_iName);

    if (property == UNKNOWN_DATA_PROPERTY)
    {
        return FALSE;
    }

    returnValue = dataObject->setDataProperty(property, _dblValue, numElements);

    return (BOOL) returnValue;
}

void DataModel::getGraphicObjectProperty(int iUID, int _iName, void **_pvData)
{
    Data3D* dataObject = NULL;
    int property = 0;

    dataObject = m_dataMap[iUID];

    if (dataObject == NULL)
    {
        *_pvData = NULL;
        return;
    }

    property = dataObject->getPropertyFromName(_iName);
    if (property == UNKNOWN_DATA_PROPERTY)
    {
        *_pvData = NULL;
        return;
    }

    dataObject->getDataProperty(property, _pvData);
}

int DataModel::createDataObject(int iUID, int _iType)
{
    Data3D* newObject = NULL;

    /*
     * To be implemented as the Java graphicObject class' similar function
     * (getTypeFromName) in order to avoid strcmp calls.
     */

    switch (_iType)
    {
        case __GO_GRAYPLOT__ :
            newObject = new NgonGridData();
            break;
        case __GO_MATPLOT__ :
            newObject = new NgonGridMatplotData();
            break;
        case __GO_FAC3D__ :
            newObject = new NgonGeneralData();
            break;
        case __GO_PLOT3D__ :
            newObject = new NgonGridData();
            break;
        case __GO_POLYLINE__ :
            newObject = new NgonPolylineData();
            break;
        case __GO_FEC__ :
            newObject = new MeshFecData();
            break;
        default :
            return 0;
    }

    m_dataMap[iUID] = newObject;

    return iUID;
}

void DataModel::deleteDataObject(int iUID)
{
    std::map<int, Data3D*>::iterator it = m_dataMap.find(iUID);
    if (it != m_dataMap.end() && it->second != NULL)
    {
        delete it->second;
        m_dataMap.erase(it);
    }

    /*Data3D* newObject = m_dataMap[std::string(_pstID)];
      if (newObject != NULL)
      {
      delete newObject;
      }

      m_dataMap.erase(std::string(_pstID));*/
}

