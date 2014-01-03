/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "DataModel.hxx"

extern "C" {
#include "BOOL.h"
}

class DataController
{

public :
    static BOOL setGraphicObjectProperty(int iUID, int _iName, void const* _dblValue, int numElements)
    {
        return DataModel::get()->setGraphicObjectProperty(iUID, _iName, _dblValue, numElements);
    }

    static void getGraphicObjectProperty(int iUID, int _iName, void **_pvData)
    {
        return DataModel::get()->getGraphicObjectProperty(iUID, _iName, _pvData);
    }

    static int createDataObject(int iUID, int _iType)
    {
        return DataModel::get()->createDataObject(iUID, _iType);
    }

    static void deleteDataObject(int iUID)
    {
        DataModel::get()->deleteDataObject(iUID);
    }

};
