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

#include <map>
#include <string>

extern "C" {
#include "BOOL.h"
}

class DataModel
{
private :
    DataModel()
    {
        m_dataMap = new std::map<std::string, double>();
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
    BOOL setGraphicObjectProperty(char *_pstID, double _dblValue)
    {
        (*m_dataMap)[std::string(_pstID)] = _dblValue;
        return TRUE;
    }

    double getGraphicObjectProperty(char *_pstID)
    {
        return (*m_dataMap)[std::string(_pstID)];
    }

private :
    static DataModel *m_me;
    std::map<std::string, double> *m_dataMap;
};
