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
}

class DataController
{

public :
    static BOOL setGraphicObjectProperty(char const* _pstID, int _iName, void const* _dblValue, int numElements)
    {
        return DataModel::get()->setGraphicObjectProperty(_pstID, _iName, _dblValue, numElements);
    }

    static void getGraphicObjectProperty(char const* _pstID, int _iName, void **_pvData)
    {
        return DataModel::get()->getGraphicObjectProperty(_pstID, _iName, _pvData);
    }

    static char const* createDataObject(char const* _pstID, int _iType)
    {
        return DataModel::get()->createDataObject(_pstID, _iType);
    }

    static void deleteDataObject(char const* _pstID)
    {
        DataModel::get()->deleteDataObject(_pstID);
    }

};
