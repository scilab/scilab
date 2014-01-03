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

#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <map>
#include <string>

#include "Data3D.hxx"

#include "NgonGeneralData.hxx"
#include "NgonGridData.hxx"
#include "NgonGridMatplotData.hxx"
#include "NgonPolylineData.hxx"
#include "TriangleMeshData.hxx"
#include "TriangleMeshFecData.hxx"

#include "graphicObjectProperties.h"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

class DataModel
{
private :
    DataModel()
    {
        m_dataMap = new std::map<int, Data3D*>();
    }

public :
    static DataModel *get()
    {
        if (m_me == NULL)
        {
            m_me = new DataModel();
        }

        return m_me;
    }

public :
    /**
     * Sets a graphic object property
     * As some set methods allocate memory, the FALSE return value also indicates
     * a failed allocation for these methods, which overlaps with the non-existing
     * property return value (also FALSE); returning an int, with -1 for a failed
     * allocation would possibly solve this problem.
     */
    BOOL setGraphicObjectProperty(int iUID, int _iName, void const* _dblValue, int numElements);

    /** Returns a graphic object vector property */
    void getGraphicObjectProperty(int iUID, int _iName, void **_pvData);

    /** Creates a data object */
    int createDataObject(int iUID, int _iType);

    /**
     * Deletes a data object
     */
    void deleteDataObject(int iUID);

private :
    static DataModel *m_me;

    std::map<int, Data3D*> *m_dataMap;
};

#endif
