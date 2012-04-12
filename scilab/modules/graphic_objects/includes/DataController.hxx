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
    static BOOL setGraphicObjectProperty(char const* _pstID, char const* _pstName, void const* _dblValue, int numElements)
    {
        return DataModel::get()->setGraphicObjectProperty(_pstID, _pstName, _dblValue, numElements);
    }

    static void getGraphicObjectProperty(char const* _pstID, char const* _pstName, void **_pvData)
    {
        return DataModel::get()->getGraphicObjectProperty(_pstID, _pstName, _pvData);
    }

    static void getGraphicObjectIntProperty(char const* _pstID, char const* _pstName, void **_pvData)
    {
        return DataModel::get()->getGraphicObjectIntProperty(_pstID, _pstName, _pvData);
    }

    static char const* createDataObject(char const* _pstID, char const* _sType)
    {
        return DataModel::get()->createDataObject(_pstID, _sType);
    }

    static void deleteDataObject(char const* _pstID)
    {
        DataModel::get()->deleteDataObject(_pstID);
    }

};
