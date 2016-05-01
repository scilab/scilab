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

#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <map>
#include <string>

#include "Data3D.hxx"

#include "NgonGeneralData.hxx"
#include "NgonGridData.hxx"
#include "NgonGridMatplotData.hxx"
#include "NgonPolylineData.hxx"
#include "MeshData.hxx"
#include "MeshFecData.hxx"

#include "graphicObjectProperties.h"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

class DataModel
{
private :
    DataModel() {}

public :
    static DataModel *get()
    {
        if (m_me == NULL)
        {
            m_me = new DataModel();
        }

        return m_me;
    }

    static void destroy()
    {
        if (m_me)
        {
            delete m_me;
            m_me = NULL;
        }
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

    std::map<int, Data3D*> m_dataMap;
};

#endif
