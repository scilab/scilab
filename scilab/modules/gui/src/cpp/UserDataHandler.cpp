/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UserDataHandler.hxx"
extern "C"
{
#include "stack2.h"
#include "returnProperty.h"
}

namespace org_scilab_modules_gui_uiwidget
{

UserDataHandler::UserDataMap UserDataHandler::map;

void UserDataHandler::put(const int uid, const UserData * data)
{
    UserDataMapIt it = map.find(uid);
    if (it != map.end())
    {
        // map already contains a UserData
        // we delete it and replace it with new UserData
        delete it->second;
        it->second = data;
    }
    else
    {
        // map doesn't contain a UserData
        // so we insert it
        map.insert(std::pair<int, const UserData *>(uid, data));
    }
}

void UserDataHandler::put(const int uid, const int * data, const int size)
{
    const UserData * ud = new UserData(data, data + size);
    put(uid, ud);
}

void UserDataHandler::put(const int uid, const int stackPos)
{
    const int iUserDataSize = GetDataSize(stackPos) * 2; /* GetDataSize returns the size of the variable in double words */
    const int * piUserData = (int *)GetData(stackPos);
    put(uid, piUserData, iUserDataSize);
}

int UserDataHandler::get(const int uid, void * pvApiCtx, const int stackPos)
{
    int status = 0;
    UserDataMapIt it = map.find(uid);
    if (it != map.end() && !it->second->empty())
    {
        // UserData has been found and it is not empty
        status = sciReturnUserData(pvApiCtx, &((*it->second)[0]), it->second->size());
    }
    else
    {
        // Nothing so we return an empty matrix
        status = sciReturnEmptyMatrix(pvApiCtx);
    }

    return status;
}

void UserDataHandler::remove(const int uid)
{
    UserDataMapIt it = map.find(uid);
    if (it != map.end())
    {
        // UserData exists so we delete and remove it from the map
        delete it->second;
        map.erase(it);
    }
}
}

void removeUIWidgetUserData(const int uid)
{
    org_scilab_modules_gui_uiwidget::UserDataHandler::remove(uid);
}
