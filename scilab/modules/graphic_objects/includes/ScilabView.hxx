/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SCILAB_VIEW_HXX__
#define __SCILAB_VIEW_HXX__

#include <map>
#include <string>

#include "dynlib_graphic_objects.h"

//disable warnings about exports STL items
#pragma warning (disable : 4251)

extern "C"
{
    void ScilabNativeView__createObject(int id);
    void ScilabNativeView__deleteObject(int id);
    void ScilabNativeView__updateObject(int id, int iProperty);
    void ScilabNativeView__setCurrentFigure(int id);
    void ScilabNativeView__setCurrentSubWin(int id);
    void ScilabNativeView__setCurrentObject(int id);
    int ScilabNativeView__getValidDefaultFigureId();
}

class GRAPHIC_OBJECTS_IMPEXP ScilabView
{
private :
    ScilabView() {}
    ~ScilabView() {}

    // Define type for easy manipulation.
    typedef std::map<int, long>  __handleList;
    typedef __handleList::iterator          __handleList_iterator;
    typedef std::map<long, int>  __uidList;
    typedef __uidList::iterator          __uidList_iterator;
    typedef std::map<int, int>    __figureList;
    typedef __figureList::iterator          __figureList_iterator;
    typedef __figureList::reverse_iterator          __figureList_reverse_iterator;

    static __figureList m_figureList;
    static __handleList m_handleList;
    static __uidList    m_uidList;
    static long         m_topHandleValue;
    static int          m_currentFigure;
    static int          m_currentObject;
    static int          m_currentSubWin;
    static int          m_figureModel;
    static int          m_axesModel;

public :
    static void createObject(int iUID);
    static void deleteObject(int iUID);
    static void updateObject(int iUID, int iProperty);

    static int  getNbFigure(void);
    static void getFiguresId(int ids[]);
    static void registerToController(void);
    static void unregisterToController(void);
    static bool existsFigureId(int id);
    static int  getFigureFromIndex(int figureNumber);
    static bool isEmptyFigureList(void);
    static int  getValidDefaultFigureId();

    static int  getCurrentFigure(void);
    static void setCurrentFigure(int iUID);

    static int  getCurrentObject(void);
    static void setCurrentObject(int iUID);

    static int  getCurrentSubWin(void);
    static void setCurrentSubWin(int iUID);

    static long getObjectHandle(int iUID);
    static int  getObjectFromHandle(long handle);

    static int  getFigureModel(void);
    static void setFigureModel(int iUID);

    static int  getAxesModel(void);
    static void setAxesModel(int iUID);

};

#endif /* !__SCILAB_VIEW_HXX__ */

