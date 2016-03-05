/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#ifndef __SCILAB_VIEW_HXX__
#define __SCILAB_VIEW_HXX__

#include <map>
#include <list>
#include <vector>
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
    int ScilabNativeView__getFigureFromIndex(int figNum);
}

struct PathItem
{
    typedef std::list<int> __child;
    typedef __child::iterator __child_iterator;

    std::string tag;
    int uid;
    int parent;
    __child children;

    PathItem() : tag(""), uid(0), parent(0) {}
};

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
    typedef std::map<int, std::vector<int> >  __userdata;

    typedef  std::map<int, PathItem*> __pathList;
    typedef __pathList::iterator __pathList_iterator;
    typedef  std::map<std::string, int> __pathFigList;
    typedef __pathFigList::iterator __pathFigList_iterator;

    static __figureList     m_figureList;
    static __handleList     m_handleList;
    static __uidList        m_uidList;
    static __pathFigList    m_pathFigList;
    static __pathList       m_pathList;
    static __userdata       m_userdata;
    static long             m_topHandleValue;
    static int              m_currentFigure;
    static int              m_currentObject;
    static int              m_currentSubWin;
    static int              m_figureModel;
    static int              m_axesModel;

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

    static PathItem* getItem(int uid);
    static PathItem* getItem(std::string _pstTag);
    static PathItem* getItem(std::string _pstTag, std::list<int>& _ignoredList);
    static PathItem* getFigureItem(std::string _pstTag);

    static int search_path(char* _pstPath);
    static std::string get_path(int uid);

    static void setUserdata(int _id, int* _data, int _datasize);
    static int getUserdataSize(int _id);
    static int* getUserdata(int _id);

private :
    static PathItem* search_children(PathItem* _path, std::string _subPath, bool _bDeep, std::list<int>& _ignoredList);

};

#endif /* !__SCILAB_VIEW_HXX__ */

