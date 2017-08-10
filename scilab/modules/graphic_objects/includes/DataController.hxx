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
