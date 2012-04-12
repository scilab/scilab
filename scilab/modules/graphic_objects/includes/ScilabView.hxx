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
#include <string.h>

#include "dynlib_graphic_objects.h"

extern "C"
{
    void ScilabNativeView__createObject(char* pstId);
    void ScilabNativeView__deleteObject(char* pstId);
    void ScilabNativeView__updateObject(char* pstId, char* pstProperty);
}

class GRAPHIC_OBJECTS_IMPEXP ScilabView
{
private :
    ScilabView() {}
    ~ScilabView() {}
    struct cmp_str
    {
        bool operator()(char const *a, char const *b)
        {
            return strcmp(a, b) < 0;
        }
    };

    // Define type for easy manipulation.
    typedef std::map<char*, long, cmp_str>  __handleList;
    typedef __handleList::iterator          __handleList_iterator;
    typedef std::map<char*, int, cmp_str>    __figureList;
    typedef __figureList::iterator          __figureList_iterator;

    static __figureList                     m_figureList;
    static __handleList                     m_handleList;
    static long                             m_topHandleValue;
    static char*                            m_currentFigure;
    static char*                            m_currentObject;
    static char*                            m_currentSubWin;
    static char*                            m_figureModel;
    static char*                            m_axesModel;

public :
    static void   createObject(char* pstId);
    static void   deleteObject(char* pstId);
    static void   updateObject(char* pstId, char* pstProperty);

    static int    getNbFigure(void);
    static void   getFiguresId(int ids[]);
    static void   registerToController(void);
    static void   unregisterToController(void);
    static bool   existsFigureId(int id);
    static char*  getFigureFromIndex(int figureNumber);
    static bool   isEmptyFigureList(void);

    static char*  getCurrentFigure(void);
    static void   setCurrentFigure(char *UID);

    static char*  getCurrentObject(void);
    static void   setCurrentObject(char *UID);

    static char*  getCurrentSubWin(void);
    static void   setCurrentSubWin(char *UID);

    static long   getObjectHandle(char *UID);
    static char*  getObjectFromHandle(long handle);

    static char*  getFigureModel(void);
    static void   setFigureModel(char *UID);

    static char*  getAxesModel(void);
    static void   setAxesModel(char *UID);

};

#endif /* !__SCILAB_VIEW_HXX__ */

