/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __USERDATAHANDLER_HXX__
#define __USERDATAHANDLER_HXX__

#include <map>
#include <vector>

extern "C"
{
    void removeUIWidgetUserData(const int uid);
}

namespace org_scilab_modules_gui_uiwidget
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to handle UserData in uiwidget elements
 * UserData should be automatically removed when objects in Java world are garbage collected (overload of finalize())
 */
class UserDataHandler
{

    typedef std::vector<int> UserData;
    typedef std::map<int, const UserData *> UserDataMap;
    typedef UserDataMap::iterator UserDataMapIt;

    static UserDataMap map;

public:

    /**
     * Put a UserData in the map
     * @param uid the uid of the Java object.
     * @param data the date to set
     */
    static void put(const int uid, const UserData * data);

    /**
     * Put a Scilab data in the map
     * @param uid the uid of the Java object.
     * @param data the date to set
     * @param size the data total size
     */
    static void put(const int uid, const int * data, const int size);

    /**
     * Put a data according to its position in the Scilab's stack into the map
     * @param uid the uid of the Java object.
     * @param stackPos the stack position
     */
    static void put(const int uid, const int stackPos);

    /**
     * Get a data in the map and put it in the Scilab's stack.
     * @param uid the uid of the Java object.
     * @param pvApiCtx api context
     * @param stackPos the stack position
     * @return an int corresponding to the success (or not) of the operation
     */
    static int get(const int uid, void * pvApiCtx, const int stackPos);

    /**
     * Remove the UserData associated with the uid
     * @param uid the uid of the Java object.
     */
    static void remove(const int uid);
};
}

#endif // __USERDATAHANDLER_HXX__
