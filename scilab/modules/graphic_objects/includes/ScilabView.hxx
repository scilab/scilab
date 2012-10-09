/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILAB_VIEW_HXX__
#define __SCILAB_VIEW_HXX__

#include <map>
#include <string>

#include "dynlib_graphic_objects.h"

extern "C"
{
    void ScilabNativeView__createObject(char const* pstId);
    void ScilabNativeView__deleteObject(char const* pstId);
    void ScilabNativeView__updateObject(char const* pstId, int iProperty);
}

class GRAPHIC_OBJECTS_IMPEXP ScilabView
{
private :
    ScilabView() {}
    ~ScilabView() {}

    // Define type for easy manipulation.
    typedef std::map<std::string, long>  __handleList;
    typedef __handleList::iterator          __handleList_iterator;
    typedef std::map<long, std::string>  __uidList;
    typedef __uidList::iterator          __uidList_iterator;
    typedef std::map<std::string, int>    __figureList;
    typedef __figureList::iterator          __figureList_iterator;

    static __figureList                     m_figureList;
    static __handleList                     m_handleList;
    static __uidList                        m_uidList;
    static long                             m_topHandleValue;
    static std::string                      m_currentFigure;
    static std::string                      m_currentObject;
    static std::string                      m_currentSubWin;
    static std::string                      m_figureModel;
    static std::string                      m_axesModel;

public :
    static void   createObject(char const* pstId);
    static void   deleteObject(char const* pstId);
    static void   updateObject(char const* pstId, int iProperty);

    static int    getNbFigure(void);
    static void   getFiguresId(int ids[]);
    static void   registerToController(void);
    static void   unregisterToController(void);
    static bool   existsFigureId(int id);
    static char const*  getFigureFromIndex(int figureNumber);
    static bool   isEmptyFigureList(void);

    static char const*  getCurrentFigure(void);
    static void   setCurrentFigure(char const* UID);

    static char const*  getCurrentObject(void);
    static void   setCurrentObject(char const* UID);

    static char const*  getCurrentSubWin(void);
    static void   setCurrentSubWin(char const* UID);

    static long   getObjectHandle(char const* UID);
    static char const*  getObjectFromHandle(long handle);

    static char const*  getFigureModel(void);
    static void   setFigureModel(char const* UID);

    static char const* getAxesModel(void);
    static void setAxesModel(char const* UID);

};

#endif /* !__SCILAB_VIEW_HXX__ */

